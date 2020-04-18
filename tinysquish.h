#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
	bool (*read)(uint8_t* byte, void* private_data);
	void* private_data;
} ReadInterface;

typedef struct {
	bool (*write)(uint8_t byte, void* private_data);
	void* private_data;
} WriteInterface;

bool tinysquish_compress(uint8_t* data, uint32_t data_size, WriteInterface* writer);
bool tinysquish_decompress(ReadInterface* reader, WriteInterface* writer);
