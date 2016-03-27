// cheapo linked list impl.
#include <stdlib.h>
#include "./linked-list.h"

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

  free(removed); // probably bad form
}

void list_move_up(struct list* it, int n)
{
  if (n == 0) return;

  if (n == 1)
  {
    struct listitem* tgt = _list_nth(it, 1);
    if (tgt == NULL) return;

    struct listitem* cdr = tgt->next; // x tgt cdr
    tgt->next = it->head;             // x tgt x | cdr
    it->head = tgt;                   // tgt x tgt | cdr
    tgt->next->next = cdr;            // tgt x cdr
  }
  else
  {
    struct listitem* ptr = _list_nth(it, n - 2);
    if (ptr == NULL || ptr->next == NULL || ptr->next->next == NULL) return;

    struct listitem* x = ptr->next; // ptr x tgt y
    ptr->next = x->next;            // ptr tgt y | x tgt
    x->next = ptr->next->next;      // ptr tgt y | x y
    ptr->next->next = x;            // ptr tgt x y
  }
}

void list_move_down(struct list* it, int n)
{
  if (n == (list_length(it) - 1)) return;

  if (n == 0)
  {
    struct listitem* x = it->head;
    if (x->next == NULL) return;

    struct listitem* tgt = x->next; // x tgt cdr
    x->next = tgt->next;            // x cdr | tgt cdr
    it->head = tgt;                 // tgt cdr | x cdr
    tgt->next = x;                  // tgt x cdr
  }
  else
  {
    struct listitem* ptr = _list_nth(it, n - 1);
    if (ptr == NULL || ptr->next == NULL) return;

    struct listitem* tgt = ptr->next; // ptr tgt x y
    ptr->next = tgt->next;            // ptr x | tgt | y
    tgt->next = ptr->next->next;      // ptr x | tgt y
    ptr->next->next = tgt;            // ptr x tgt y
  }
}

void list_destruct(struct list* it)
{
  while (it->head != NULL) list_remove(it, 0);
  free(it);
}

