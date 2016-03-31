#ifndef __GROUP
#define __GROUP

#include "./linked-list.h"

struct group
{
  char* name;
  struct list* marks;
};

void group_init(struct group* g);

size_t size_group(struct group* g);
void serialize_group(struct group* g, void* buffer);
void deserialize_group(struct group* g, void* buffer);

size_t size_groups(struct list* group_list);
void serialize_groups(struct list* group_list, void* buffer);
void deserialize_groups(struct list* group_list, void* buffer);

void group_free_marks(struct group* g);
void group_destruct(struct group* g);

void groups_destruct(struct list* group_list);

#endif

