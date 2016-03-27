#ifndef __LINKED_LIST
#define __LINKED_LIST

struct listitem
{
  void* data;
  struct listitem* next;
};

struct list
{
  struct listitem* head;
};

void list_init(struct list* it);

int list_length(struct list* it);
void* list_last(struct list* it);
void* list_nth(struct list* it, int n);

void list_add(struct list* it, void* data);
void list_remove(struct list* it, int n);

void list_destruct(struct list* it);

#endif

