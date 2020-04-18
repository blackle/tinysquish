#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct {
	bool (*write_size)(uint32_t size, void* private_data);
	bool (*write)(uint8_t byte, void* private_data);
	void* private_data;
} WriteInterface;

bool interface_write_size(WriteInterface* writer, uint32_t size);
bool interface_write(WriteInterface* writer, uint8_t byte);
