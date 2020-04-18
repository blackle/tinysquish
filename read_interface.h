#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct {
	bool (*read_size)(uint32_t* size, void* private_data);
	bool (*read)(uint8_t* byte, void* private_data);
	void* private_data;
} ReadInterface;

bool interface_read_size(ReadInterface* reader, uint32_t* size);
bool interface_read(ReadInterface* reader, uint8_t* byte);
