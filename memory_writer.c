#include "memory_writer.h"
#include "memory_writer_p.h"
#include <stdlib.h>
#include <string.h>

static bool memory_writer_write_size(uint32_t size, void* private_data)
{
	MemoryWriter* mw = (MemoryWriter*) private_data;
	mw->uncompressed_size = size;
	return true;
}

static bool memory_writer_write(uint8_t byte, void* private_data)
{
	MemoryWriter* mw = (MemoryWriter*) private_data;
	if (mw->position == mw->size) {
		return false;
	}
	mw->data[mw->position] = byte;
	mw->position++;
	return true;
}

WriteInterface* memory_writer_new(size_t size)
{
	WriteInterface* writer = malloc(sizeof(WriteInterface));
	if (!writer) {
		return NULL;
	}
	MemoryWriter* mw = malloc(sizeof(MemoryWriter));
	if (!mw) {
		free(writer);
		return NULL;
	}
	mw->data = malloc(sizeof(uint8_t) * size);
	if (!mw->data) {
		free(writer);
		free(mw);
		return NULL;
	}

	mw->size = size;
	mw->position = 0;
	mw->uncompressed_size = 0;

	writer->write_size = memory_writer_write_size;
	writer->write = memory_writer_write;
	writer->private_data = mw;

	return writer;
}

void memory_writer_free(WriteInterface* writer)
{
	MemoryWriter* mw = (MemoryWriter*) writer->private_data;
	free(mw->data);
	free(mw);
	free(writer);
}

bool memory_writer_compare(const WriteInterface* writer, const uint8_t* data, size_t size)
{
	const MemoryWriter* mw = (const MemoryWriter*) writer->private_data;
	if (size != mw->position) {
		return false;
	}
	return memcmp(mw->data, data, sizeof(uint8_t) * size) == 0;
}

size_t memory_writer_length(const WriteInterface* writer)
{
	const MemoryWriter* mw = (const MemoryWriter*) writer->private_data;
	return mw->position;
}
