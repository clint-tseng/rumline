#include <pebble.h>
#include <stdlib.h>

bool pbstore_exists(size_t offset) { return persist_exists(offset); }

void pbstore_set(void* buffer, size_t bytes, size_t offset)
{
  persist_write_int(offset, (int) bytes);

  unsigned int i = 0;
  for (; ((i + 1) * PERSIST_DATA_MAX_LENGTH) < bytes; i++)
    persist_write_data(offset + i + 1, (buffer + (i * PERSIST_DATA_MAX_LENGTH)), PERSIST_DATA_MAX_LENGTH);

  if ((i * PERSIST_DATA_MAX_LENGTH) < bytes)
    persist_write_data(offset + i + 1, (buffer + (i * PERSIST_DATA_MAX_LENGTH)), bytes - (i * PERSIST_DATA_MAX_LENGTH));
};

void* pbstore_get(size_t offset)
{
  size_t bytes = (size_t) persist_read_int(offset);
  void* buffer = malloc(bytes);

  unsigned int i = 0;
  for (; ((i + 1) * PERSIST_DATA_MAX_LENGTH) < bytes; i++)
    persist_read_data(offset + i + 1, (buffer + (i * PERSIST_DATA_MAX_LENGTH)), PERSIST_DATA_MAX_LENGTH);

  if ((i * PERSIST_DATA_MAX_LENGTH) < bytes)
    persist_read_data(offset + i + 1, (buffer + (i * PERSIST_DATA_MAX_LENGTH)), bytes - (i * PERSIST_DATA_MAX_LENGTH));

  return buffer;
};

