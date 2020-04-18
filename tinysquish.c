#include "tinysquish.h"
#include <assert.h>

#define NUM_BIT_MODEL_TOTAL_BITS 8
#define PROB_INIT_VAL ((1 << NUM_BIT_MODEL_TOTAL_BITS) / 2)
typedef uint8_t Prob;

#define NUM_SYMBOLS 0x100
#define TREE_PROBS 0x80
static void init_tree_probs(Prob probs[NUM_SYMBOLS][TREE_PROBS])
{
	for (size_t i = 0; i < NUM_SYMBOLS; i++) {
		for (size_t j = 0; j < TREE_PROBS; j++) {
			probs[i][j] = PROB_INIT_VAL;
		}
	}
}

typedef struct {
	uint64_t low;
	uint32_t range;
	uint8_t cache;
	uint64_t cache_size;
} RangeEncoder;

static const RangeEncoder RANGE_ENCODER_INIT = {
	.low = 0,
	.range = 0xFFFFFFFF,
	.cache = 0,
	.cache_size = 1
};

#define TOP_MASK 0xFF000000

static bool range_encoder_shift_low(RangeEncoder* enc, WriteInterface* writer)
{
	uint32_t high_bytes = enc->low >> 32;
	uint32_t low_bytes = enc->low & 0xFFFFFFFF;
	if (low_bytes < TOP_MASK || high_bytes != 0) {
		uint8_t temp = enc->cache;
		do {
			uint8_t out_byte = temp + (high_bytes & 0xFF);
			if (!interface_write(writer, out_byte)) return false;
			temp = 0xFF;
		} while (--enc->cache_size != 0);
		enc->cache = (enc->low >> 24) & 0xFF;
	}
	enc->cache_size++;
	enc->low = low_bytes << 8;
	return true;
}

static bool range_encoder_flush_data(RangeEncoder* enc, WriteInterface* writer)
{
	for (int i = 0; i < 5; i++) {
		if (!range_encoder_shift_low(enc, writer)) return false;
	}
	return true;
}

static bool range_encoder_encode_bit(bool bit, Prob prob, RangeEncoder* enc, WriteInterface* writer)
{
	uint32_t new_bound = (enc->range >> NUM_BIT_MODEL_TOTAL_BITS) * prob;
	if (bit) {
		enc->low += new_bound;
		enc->range -= new_bound;
	} else {
		enc->range = new_bound;
	}
	while ((enc->range & TOP_MASK) == 0) {
		assert(enc->range != 0);
		enc->range <<= 8;
		if (!range_encoder_shift_low(enc, writer)) return false;
	}
	return true;
}

#define NUM_MOVE_BITS 2
static void update_prob(bool bit, Prob* prob)
{
	if (bit) *prob -= *prob >> NUM_MOVE_BITS;
	else *prob += ((1 << NUM_BIT_MODEL_TOTAL_BITS) - *prob) >> NUM_MOVE_BITS;
}

bool encode_bit_tree(uint8_t byte, Prob probs[TREE_PROBS], RangeEncoder* enc, WriteInterface* writer)
{
	unsigned m = 1;
	for (unsigned bit_index = 7; bit_index != 0;)
	{
		bit_index--;
		int bit = (byte >> bit_index) & 1;
		if (!range_encoder_encode_bit(bit, probs[m], enc, writer)) return false;
		update_prob(bit, &probs[m]);
		m = (m << 1) | bit;
	}
	return true;
}

bool tinysquish_compress(uint8_t* data, uint32_t data_size, WriteInterface* writer)
{
	if (!interface_write_size(writer, data_size)) return false;

	RangeEncoder enc = RANGE_ENCODER_INIT;
	Prob probs[NUM_SYMBOLS][TREE_PROBS];
	init_tree_probs(probs);

	uint8_t last_byte = 0;
	for (size_t i = 0; i < data_size; i++) {
		if (!encode_bit_tree(data[i], probs[last_byte], &enc, writer)) return false;
		last_byte = data[i];
	}
	range_encoder_encode_bit(1, 1, &enc, writer); //this is an extremely underhanded solution to the problem where the last bit is always wrong :<
	range_encoder_flush_data(&enc, writer);
	return true;
}

typedef struct {
	uint32_t code;
	uint32_t range;
} RangeDecoder;

static const RangeDecoder RANGE_DECODER_INIT = {
	.code = 0,
	.range = 0xFFFFFFFF
};

static bool range_decoder_init(RangeDecoder* dec, ReadInterface* reader)
{
	uint8_t byte;
	if (!interface_read(reader, &byte)) return false;
	if (byte != 0) return false;

	for (int i = 0; i < 4; i++) {
		if (!interface_read(reader, &byte)) return false;
		dec->code = (dec->code << 8) | byte;
	}
	if (dec->code == dec->range) return false;
	return true;
}

bool range_decoder_normalize(RangeDecoder* dec, ReadInterface* reader)
{
	uint8_t byte;
	if (dec->range < (1 << 24)) {
		dec->range <<= 8;
		if (!interface_read(reader, &byte)) return false;
		dec->code = (dec->code << 8) | byte;
	}
	return true;
}

bool range_decoder_decode_bit(bool* bit, Prob prob, RangeDecoder* dec, ReadInterface* reader)
{
	uint32_t bound = (dec->range >> NUM_BIT_MODEL_TOTAL_BITS) * prob;
	*bit = dec->code > bound;
	if (*bit) {
		dec->code -= bound;
		dec->range -= bound;
	} else {
		dec->range = bound;
	}
	return range_decoder_normalize(dec, reader);
}

bool decode_bit_tree(uint8_t* byte, Prob probs[TREE_PROBS], RangeDecoder* dec, ReadInterface* reader)
{
	unsigned m = 1;
	bool bit;
	for (unsigned bit_index = 0; bit_index < 7; bit_index++) {
		if (!range_decoder_decode_bit(&bit, probs[m], dec, reader)) return false;
		update_prob(bit, &probs[m]);
		m = (m << 1) + bit;
	}
  *byte = m - TREE_PROBS;
  return true;
}

bool tinysquish_decompress(ReadInterface* reader, WriteInterface* writer)
{
	uint32_t data_size;
	if (!interface_read_size(reader, &data_size)) return false;

	RangeDecoder dec = RANGE_DECODER_INIT;
	if (!range_decoder_init(&dec, reader)) return false;

	Prob probs[NUM_SYMBOLS][TREE_PROBS];
	init_tree_probs(probs);
	uint8_t last_byte = 0;
	for (size_t i = 0; i < data_size; i++) {
		uint8_t byte = 0;
		if (!decode_bit_tree(&byte, probs[last_byte], &dec, reader)) return false;
		if (!interface_write(writer, byte)) return false;
		last_byte = byte;
	}

	return true;
}
