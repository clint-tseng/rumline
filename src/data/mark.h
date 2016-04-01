#ifndef __MARK
#define __MARK

#include "linked-list.h"

struct mark
{
  short id;
  char* name;
};
typedef struct mark Mark;

Mark* mark_create();

size_t size_mark(Mark* it);
void serialize_mark(Mark* it, void* buffer);
void deserialize_mark(Mark* it, void* buffer);

size_t size_marks(List* mark_list);
void serialize_marks(List* mark_list, void* buffer);
void deserialize_marks(List* mark_list, void* buffer);

void mark_destruct(Mark* m);
void marks_destruct(List* m);

#endif

