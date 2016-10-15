#include <pebble.h>
#include <stdlib.h>

bool pbstore_exists() { return persist_exists(0); }

void pbstore_set(void* buffer, size_t bytes)
{
  persist_write_int(0, (int) bytes);

  unsigned int i = 0;
  for (; ((i + 1) * PERSIST_DATA_MAX_LENGTH) < bytes; i++)
    persist_write_data(i + 1, (buffer + (i * PERSIST_DATA_MAX_LENGTH)), PERSIST_DATA_MAX_LENGTH);

  if ((i * PERSIST_DATA_MAX_LENGTH) < bytes)
    persist_write_data(i + 1, (buffer + (i * PERSIST_DATA_MAX_LENGTH)), bytes - (i * PERSIST_DATA_MAX_LENGTH));
};

void* pbstore_get()
{
  size_t bytes = (size_t) persist_read_int(0);
  void* buffer = malloc(bytes);

  unsigned int i = 0;
  for (; ((i + 1) * PERSIST_DATA_MAX_LENGTH) < bytes; i++)
    persist_read_data(i + 1, (buffer + (i * PERSIST_DATA_MAX_LENGTH)), PERSIST_DATA_MAX_LENGTH);

  if ((i * PERSIST_DATA_MAX_LENGTH) < bytes)
    persist_read_data(i + 1, (buffer + (i * PERSIST_DATA_MAX_LENGTH)), bytes - (i * PERSIST_DATA_MAX_LENGTH));

  return buffer;
};

bool recents_exists() { return persist_exists(100000); }

void recents_set(short* ids)
{
  persist_write_int(100000, (int) sizeof(*ids));
  persist_write_data(100001, ids, sizeof(*ids));
};

short* recents_get()
{
  size_t bytes = (size_t) persist_read_int(100000);
  short* buffer = malloc(bytes);
  persist_read_data(100001, buffer, bytes);
};

