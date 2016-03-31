#ifndef __MARK
#define __MARK

#include "./linked-list.h"

struct mark
{
  short id;
  char* name;
};

size_t size_mark(struct mark* it);
void serialize_mark(struct mark* it, void* buffer);
void deserialize_mark(struct mark* it, void* buffer);

size_t size_marks(struct list* mark_list);
void serialize_marks(struct list* mark_list, void* buffer);
void deserialize_marks(struct list* mark_list, void* buffer);

#endif

