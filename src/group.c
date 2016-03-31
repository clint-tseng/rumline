#include <stdlib.h>
#include <string.h>

#include "./group.h"
#include "./mark.h"

void group_init(struct group* g)
{
  struct list* l = malloc(sizeof (struct list));
  list_init(l);
  g->marks = l;
}

size_t size_group(struct group* g)
{
  return size_marks(g->marks) + (sizeof (short)) + strlen(g->name) + 1;
}

void serialize_group(struct group* g, void* buffer)
{
  size_t list_size = size_marks(g->marks);
  serialize_marks(g->marks, buffer);
  buffer += list_size;

  size_t name_length = strlen(g->name) + 1;
  *(short*) buffer = name_length;
  buffer += sizeof (short);

  strcpy(buffer, g->name);
}

void deserialize_group(struct group* g, void* buffer)
{
  deserialize_marks(g->marks, buffer);
  buffer += size_marks(g->marks);

  size_t name_length = *(short*) buffer;
  buffer += sizeof (short);

  g->name = malloc(name_length);
  strncpy(g->name, buffer, name_length);
}


// TODO: list serialization is really duplicative. macro?
size_t size_groups(struct list* group_list)
{
  size_t group_sizes = 0;
  int group_count = list_length(group_list);
  for (int i = 0; i < group_count; i++) group_sizes += size_group((struct group*) list_nth(group_list, i));

  return (sizeof (short)) + group_sizes;
}

void serialize_groups(struct list* group_list, void* buffer)
{
  int group_count = list_length(group_list);

  *(short*) buffer = group_count;
  buffer += sizeof (short);

  for (int i = 0; i < group_count; i++)
  {
    struct group* g = (struct group*) list_nth(group_list, i);
    serialize_group(g, buffer);
    buffer += size_group(g);
  }
}

void deserialize_groups(struct list* group_list, void* buffer)
{
  int group_count = *(short*) buffer;
  buffer += sizeof (short);

  for (int i = 0; i < group_count; i++)
  {
    struct group* g = malloc(sizeof (struct group));
    group_init(g);
    deserialize_group(g, buffer);
    list_add(group_list, g);
    buffer += size_group(g);
  }
}


void group_free_marks(struct group* g)
{
  int mark_count = list_length(g->marks);
  for (int i = 0; i < mark_count; i++) free(list_nth(g->marks, i));
}

void group_destruct(struct group* g)
{
  list_destruct(g->marks);
  free(g); // again probably bad form.
}

void groups_destruct(struct list* group_list)
{
  int group_count = list_length(group_list);
  for (int i = 0; i < group_count; i++) group_destruct(list_nth(group_list, i));
  list_destruct(group_list);
}

