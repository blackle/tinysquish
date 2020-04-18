#pragma once
#include "memory_reader.h"

typedef struct {
	const uint8_t* data;
	size_t size;
	size_t position;
	size_t uncompressed_size;
} MemoryReader;
