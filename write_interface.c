#include "write_interface.h"

bool interface_write_size(WriteInterface* writer, uint32_t size)
{
	return (*writer->write_size)(size, writer->private_data);
}

bool interface_write(WriteInterface* writer, uint8_t byte)
{
	return (*writer->write)(byte, writer->private_data);
}
