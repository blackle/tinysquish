#include "read_interface.h"

bool interface_read_size(ReadInterface* reader, uint32_t* size)
{
	return (*reader->read_size)(size, reader->private_data);
}

bool interface_read(ReadInterface* reader, uint8_t* byte)
{
	return (*reader->read)(byte, reader->private_data);
}
