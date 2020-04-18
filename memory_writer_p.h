#pragma once
#include "memory_writer.h"

typedef struct {
	uint8_t* data;
	size_t size;
	size_t position;
	size_t uncompressed_size;
} MemoryWriter;
