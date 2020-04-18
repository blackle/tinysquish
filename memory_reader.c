#include "memory_reader.h"
#include "memory_reader_p.h"
#include "memory_writer_p.h"
#include <stdlib.h>

static bool memory_reader_read_size(uint32_t* size, void* private_data)
{
	MemoryReader* mr = (MemoryReader*) private_data;
	*size = mr->uncompressed_size;
	return true;
}

static bool memory_reader_read(uint8_t* byte, void* private_data)
{
	MemoryReader* mr = (MemoryReader*) private_data;
	if (mr->position == mr->size) {
		return false;
	}
	*byte = mr->data[mr->position];
	mr->position++;
	return true;
}

ReadInterface* memory_reader_new(const uint8_t* data, size_t size, size_t uncompressed_size)
{
	ReadInterface* reader = malloc(sizeof(ReadInterface));
	if (!reader) {
		return NULL;
	}
	MemoryReader* mr = malloc(sizeof(MemoryReader));
	if (!mr) {
		free(reader);
		return NULL;
	}

	mr->data = data;
	mr->size = size;
	mr->position = 0;
	mr->uncompressed_size = uncompressed_size;

	reader->read_size = memory_reader_read_size;
	reader->read = memory_reader_read;
	reader->private_data = mr;

	return reader;
}

void memory_reader_free(ReadInterface* reader)
{
	MemoryReader* mr = (MemoryReader*) reader->private_data;
	free(mr);
	free(reader);
}

ReadInterface* memory_reader_from_writer(const WriteInterface* writer)
{
	const MemoryWriter* mw = (const MemoryWriter*) writer->private_data;
	return memory_reader_new(mw->data, mw->position, mw->uncompressed_size);
}