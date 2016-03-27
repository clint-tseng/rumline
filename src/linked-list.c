// cheapo linked list impl.
#include <stdlib.h>

struct listitem
{
  void* data;
  struct listitem* next;
};

struct list
{
  struct listitem* head;
};

void list_init(struct list* it)
{
  it->head = NULL;
}

void _listitem_init(struct listitem* it, void* data)
{
  it->next = NULL;
  it->data = data;
}

int list_length(struct list* it)
{
  if (it->head == NULL) return 0;

  struct listitem* item = it->head;
  int result = 1;
  while (item->next != NULL)
  {
    result++;
    item = item->next;
  }
  return result;
}

struct listitem* _list_last(struct list* it)
{
  if (it->head == NULL) return NULL;

  struct listitem* item = it->head;
  while (item->next != NULL) item = item->next;

  return item;
}
void* list_last(struct list* it) { return _list_last(it)->data; }

struct listitem* _list_nth(struct list* it, int n)
{
  struct listitem* item = it->head;
  while (--n >= 0)
  {
    if (item == NULL) return NULL;
    item = item->next;
  }
  return item;
}
void* list_nth(struct list* it, int n) { return _list_nth(it, n)->data; }

void list_add(struct list* it, void* data)
{
  struct listitem* item = malloc(sizeof (struct listitem));
  _listitem_init(item, data);

  if (it->head == NULL)
  {
    it->head = item;
    return;
  }

  struct listitem* last = _list_last(it);
  last->next = item;
}

void list_remove(struct list* it, int n)
{
  if (it->head == NULL) return;

  if (n == 0)
  {
    it->head = it->head->next;
    return;
  }

  struct listitem* ptr = _list_nth(it, n - 1);
  struct listitem* removed = ptr->next;
  ptr->next = ptr->next->next;

  free(removed);
}

void list_destruct(struct list* it)
{
  while (it->head != NULL) list_remove(it, 0);
  free(it);
}

