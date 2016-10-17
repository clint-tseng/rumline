#include <stdlib.h>
#include <string.h>

#include "group.h"
#include "mark.h"

Group* group_create()
{
  Group* g = malloc(sizeof (Group));
  g->marks = list_create();
  return g;
}

size_t size_group(Group* g)
{
  return size_marks(g->marks) + (sizeof (short)) + strlen(g->name) + 1;
}

void serialize_group(Group* g, void* buffer)
{
  size_t list_size = size_marks(g->marks);
  serialize_marks(g->marks, buffer);
  buffer += list_size;

  size_t name_length = strlen(g->name) + 1;
  *(short*) buffer = name_length;
  buffer += sizeof (short);

  strcpy(buffer, g->name);
}

void deserialize_group(Group* g, void* buffer)
{
  deserialize_marks(g->marks, buffer);
  buffer += size_marks(g->marks);

  size_t name_length = *(short*) buffer;
  buffer += sizeof (short);

  g->name = malloc(name_length);
  strncpy(g->name, buffer, name_length);
}


// TODO: list serialization is really duplicative. macro?
size_t size_groups(List* group_list)
{
  size_t group_sizes = 0;
  int group_count = list_length(group_list);
  for (int i = 0; i < group_count; i++) group_sizes += size_group((Group*) list_nth(group_list, i));

  return (sizeof (short)) + group_sizes;
}

void serialize_groups(List* group_list, void* buffer)
{
  int group_count = list_length(group_list);

  *(short*) buffer = group_count;
  buffer += sizeof (short);

  for (int i = 0; i < group_count; i++)
  {
    Group* g = (Group*) list_nth(group_list, i);
    serialize_group(g, buffer);
    buffer += size_group(g);
  }
}

void deserialize_groups(List* group_list, void* buffer)
{
  int group_count = *(short*) buffer;
  buffer += sizeof (short);

  for (int i = 0; i < group_count; i++)
  {
    Group* g = group_create();
    deserialize_group(g, buffer);
    list_add(group_list, g);
    buffer += size_group(g);
  }
}

void group_destruct(Group* g)
{
  marks_destruct(g->marks);
  if (g->name) free(g->name);
  free(g);
}

void groups_destruct(List* group_list)
{
  int group_count = list_length(group_list);
  for (int i = 0; i < group_count; i++) group_destruct(list_nth(group_list, i));
  list_destruct(group_list);
}

// not really the best place for this? otoh it does directly manipulate list internals.
// TODO: clone and free marks.
void group_push_recent(Group* g, Mark* m)
{
  if (g->marks->head)
  {
    ListItem* cur = g->marks->head;
    int idx = 0;
    while (cur)
    {
      if (cur->data == m) break;
      cur = cur->next;
      idx++;
    }

    if (cur)
      list_remove(g->marks, idx);
    else if (idx > 6)
      list_remove(g->marks, 6);
  }

  ListItem* new = malloc(sizeof (ListItem));
  new->next = g->marks->head;
  new->data = m;
  g->marks->head = new;
}

