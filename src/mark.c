#include <stdlib.h>
#include <string.h>

#include "./mark.h"
#include "./linked-list.h"

size_t size_mark(struct mark* it)
{
  return (sizeof it->id) + (sizeof (short)) + strlen(it->name) + 1;
}

void serialize_mark(struct mark* it, void* buffer)
{
  *(short*) buffer = it->id;
  buffer += sizeof it->id;

  size_t name_length = strlen(it->name) + 1;
  *(short*) buffer = name_length;
  buffer += sizeof (short);

  strcpy(buffer, it->name);
}

void deserialize_mark(struct mark* it, void* buffer)
{
  it->id = *(short*) buffer;
  buffer += sizeof it->id;

  size_t name_length = *(short*) buffer;
  buffer += sizeof (short);

  it->name = malloc(name_length);
  strncpy(it->name, buffer, name_length);
}

size_t size_marks(struct list* mark_list)
{
  size_t mark_sizes = 0;
  int mark_count = list_length(mark_list);
  for (int i = 0; i < mark_count; i++) mark_sizes += size_mark((struct mark*) list_nth(mark_list, i));

  return (sizeof (short)) + mark_sizes;
}

void serialize_marks(struct list* mark_list, void* buffer)
{
  *(short*) buffer = list_length(mark_list);
  buffer += sizeof (short);

  int mark_count = list_length(mark_list);
  for (int i = 0; i < mark_count; i++)
  {
    struct mark* m = (struct mark*) list_nth(mark_list, i);
    serialize_mark(m, buffer);
    buffer += size_mark(m);
  }
}

void deserialize_marks(struct list* mark_list, void* buffer)
{
  int mark_count = *(short*) buffer;
  buffer += sizeof (short);

  for (int i = 0; i < mark_count; i++)
  {
    struct mark* m = malloc(sizeof (struct mark));
    deserialize_mark(m, buffer);
    list_add(mark_list, m);
    buffer += size_mark(m);
  }
}

