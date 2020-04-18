#include "probability_model.h"
#include <string.h>

typedef struct {
	uint32_t hash;
	uint8_t byte;
} HashTableEntry;

struct ProbabilityModel_struct {
	Prob bernoulli_probs[TREE_PROBS];
	Prob markov_probs[HISTORY_UNITS][NUM_SYMBOLS][TREE_PROBS];
	uint8_t history[HISTORY_LEN];
	size_t position;
	HashTableEntry hash_table[HASH_SLOTS];
};

ProbabilityModel* probability_model_new()
{
	ProbabilityModel* model = malloc(sizeof(ProbabilityModel));

	memset(model->history, 0, sizeof(uint8_t) * HISTORY_LEN);
	for (size_t i = 0; i < HISTORY_UNITS; i++) {
		for (size_t j = 0; j < NUM_SYMBOLS; j++) {
			for (size_t k = 0; k < TREE_PROBS; k++) {
				model->markov_probs[i][j][k] = PROB_INIT_VAL;
				model->bernoulli_probs[k] = PROB_INIT_VAL;
			}
		}
	}
	memset(model->hash_table, 0, sizeof(HashTableEntry) * HASH_SLOTS);
	model->position = 0;

	return model;
}

void probability_model_free(ProbabilityModel* model)
{
	free(model);
}

static void xorshift32(uint32_t *state, uint32_t entropy)
{
	*state ^= entropy;
	*state ^= *state << 13;
	*state ^= *state >> 17;
	*state ^= *state << 5;
}

static uint32_t probability_model_history_hash(const ProbabilityModel* model)
{
	uint32_t hash = 0;
	for (int i = 0; i < HISTORY_LEN; i++) {
		xorshift32(&hash, model->history[i]);
	}
	return hash;
}

Prob probability_model_get(ProbabilityModel* model, int tree_index, int bit_index)
{
	int prob = model->bernoulli_probs[tree_index];
	int sum = 1;
	for (int i = 0; i < HISTORY_UNITS; i++) {
		int weight = (HISTORY_UNITS - i)*(HISTORY_UNITS - i);
		sum += weight;
		prob += model->markov_probs[i][model->history[i]][tree_index]*weight;
	}
	prob /= sum;

	uint32_t history_hash = probability_model_history_hash(model);
	HashTableEntry* entry = &model->hash_table[history_hash % HASH_SLOTS];
	if (entry->hash == history_hash) {
		uint8_t byte = entry->byte;
		int byte_tree_index = (byte >> (bit_index+1)) | (1 << (6-bit_index));
		if (byte_tree_index == tree_index) {
			bool bit = (byte >> bit_index) & 1;
			Prob updated_prob = bit ? PROB_MIN : PROB_MAX;
			prob = (prob + updated_prob*2)/(1+2);
		}
	}

	return prob;
}

#define NUM_MOVE_BITS 1
static void update_prob(bool bit, Prob* prob)
{
	if (bit) *prob -= *prob >> NUM_MOVE_BITS;
	else *prob += ((1 << MODEL_BITS) - *prob) >> NUM_MOVE_BITS;
}

void probability_model_update(ProbabilityModel* model, int tree_index, bool bit)
{
	update_prob(bit, &model->bernoulli_probs[tree_index]);
	for (int i = 0; i < HISTORY_UNITS; i++) {
		update_prob(bit, &model->markov_probs[i][model->history[i]][tree_index]);
	}
}

void probability_model_insert_byte(ProbabilityModel* model, uint8_t byte)
{
	uint32_t history_hash = probability_model_history_hash(model);
	HashTableEntry* entry = &model->hash_table[history_hash % HASH_SLOTS];
	entry->hash = history_hash;
	entry->byte = byte;

	for (int i = 0; i < HISTORY_LEN-1; i++) {
		model->history[HISTORY_LEN - i - 1] = model->history[HISTORY_LEN - i - 2];
	}
	model->history[0] = byte;

	model->position++;
}
