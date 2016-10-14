#ifndef __LINKED_LIST
#define __LINKED_LIST

struct listitem
{
  void* data;
  struct listitem* next;
};
typedef struct listitem ListItem;

struct list
{
  struct listitem* head;
};
typedef struct list List;

List* list_create();

int list_length(List* it);
void* list_last(List* it);
void* list_nth(List* it, int n);

void list_add(List* it, void* data);
void list_remove(List* it, int n);

void list_move_up(List* it, int n);
void list_move_down(List* it, int n);

void list_destruct(List* it);

#endif

