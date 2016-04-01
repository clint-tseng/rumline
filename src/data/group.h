#ifndef __GROUP
#define __GROUP

#include "linked-list.h"

struct group
{
  char* name;
  struct list* marks;
};
typedef struct group Group;

Group* group_create();

size_t size_group(Group* g);
void serialize_group(Group* g, void* buffer);
void deserialize_group(Group* g, void* buffer);

size_t size_groups(List* group_list);
void serialize_groups(List* group_list, void* buffer);
void deserialize_groups(List* group_list, void* buffer);

void group_destruct(Group* g);
void groups_destruct(List* group_list);

#endif

