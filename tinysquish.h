#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "read_interface.h"
#include "write_interface.h"

bool tinysquish_compress(uint8_t* data, uint32_t data_size, WriteInterface* writer);
bool tinysquish_decompress(ReadInterface* reader, WriteInterface* writer);
