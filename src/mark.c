#include <stdlib.h>
#include <string.h>

#include "./mark.h"
#include "./linked-list.h"

Mark* mark_create()
{
  return malloc(sizeof (Mark));
}

size_t size_mark(Mark* it)
{
  return (sizeof it->id) + (sizeof (short)) + strlen(it->name) + 1;
}

void serialize_mark(Mark* it, void* buffer)
{
  *(short*) buffer = it->id;
  buffer += sizeof it->id;

  size_t name_length = strlen(it->name) + 1;
  *(short*) buffer = name_length;
  buffer += sizeof (short);

  strcpy(buffer, it->name);
}

void deserialize_mark(Mark* it, void* buffer)
{
  it->id = *(short*) buffer;
  buffer += sizeof it->id;

  size_t name_length = *(short*) buffer;
  buffer += sizeof (short);

  it->name = malloc(name_length);
  strncpy(it->name, buffer, name_length);
}

// TODO: list serialization is really duplicative. macro?
size_t size_marks(struct list* mark_list)
{
  size_t mark_sizes = 0;
  int mark_count = list_length(mark_list);
  for (int i = 0; i < mark_count; i++) mark_sizes += size_mark((Mark*) list_nth(mark_list, i));

  return (sizeof (short)) + mark_sizes;
}

void serialize_marks(struct list* mark_list, void* buffer)
{
  int mark_count = list_length(mark_list);

  *(short*) buffer = mark_count;
  buffer += sizeof (short);

  for (int i = 0; i < mark_count; i++)
  {
    Mark* m = (Mark*) list_nth(mark_list, i);
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
    Mark* m = malloc(sizeof (Mark));
    deserialize_mark(m, buffer);
    list_add(mark_list, m);
    buffer += size_mark(m);
  }
}

void mark_destruct(Mark* m)
{
  if (m->name) free(m->name);
  free(m);
}

void marks_destruct(struct list* mark_list)
{
  int mark_count = list_length(mark_list);
  for (int i = 0; i < mark_count; i++) mark_destruct(list_nth(mark_list, i));
  list_destruct(mark_list);
}

