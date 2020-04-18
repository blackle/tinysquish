#pragma once
#include "read_interface.h"
#include "write_interface.h"
#include <stddef.h>

ReadInterface* memory_reader_new(const uint8_t* data, size_t size, size_t uncompressed_size);
void memory_reader_free(ReadInterface* writer);

ReadInterface* memory_reader_from_writer(const WriteInterface* writer);

