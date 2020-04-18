#pragma once
#include "write_interface.h"
#include <stddef.h>

WriteInterface* memory_writer_new(size_t size);
void memory_writer_free(WriteInterface* writer);

bool memory_writer_compare(const WriteInterface* writer, const uint8_t* data, size_t size);
size_t memory_writer_length(const WriteInterface* writer);
