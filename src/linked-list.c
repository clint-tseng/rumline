// cheapo linked list impl.
#include <stdlib.h>
#include "./linked-list.h"

List* list_create()
{
  List* l = malloc(sizeof (List));
  l->head = NULL; // is this actually necessary?
  return l;
}

ListItem* _listitem_create(void* data)
{
  ListItem* li = malloc(sizeof (ListItem));
  li->next = NULL; // again, is this actually necessary?
  li->data = data;
  return li;
}

int list_length(List* it)
{
  if (it->head == NULL) return 0;

  ListItem* item = it->head;
  int result = 1;
  while (item->next != NULL)
  {
    result++;
    item = item->next;
  }
  return result;
}

ListItem* _list_last(List* it)
{
  if (it->head == NULL) return NULL;

  ListItem* item = it->head;
  while (item->next != NULL) item = item->next;

  return item;
}
void* list_last(List* it) { return _list_last(it)->data; }

ListItem* _list_nth(List* it, int n)
{
  ListItem* item = it->head;
  while (--n >= 0)
  {
    if (item == NULL) return NULL;
    item = item->next;
  }
  return item;
}
void* list_nth(List* it, int n) { return _list_nth(it, n)->data; }

void list_add(List* it, void* data)
{
  ListItem* item = _listitem_create(data);

  if (it->head == NULL)
  {
    it->head = item;
    return;
  }

  ListItem* last = _list_last(it);
  last->next = item;
}

void list_remove(List* it, int n)
{
  if (it->head == NULL) return;

  if (n == 0)
  {
    it->head = it->head->next;
    return;
  }

  ListItem* ptr = _list_nth(it, n - 1);
  ListItem* removed = ptr->next;
  ptr->next = ptr->next->next;

  free(removed); // probably bad form
}

void list_move_up(List* it, int n)
{
  if (n == 0) return;

  if (n == 1)
  {
    ListItem* tgt = _list_nth(it, 1);
    if (tgt == NULL) return;

    ListItem* cdr = tgt->next; // x tgt cdr
    tgt->next = it->head;             // x tgt x | cdr
    it->head = tgt;                   // tgt x tgt | cdr
    tgt->next->next = cdr;            // tgt x cdr
  }
  else
  {
    ListItem* ptr = _list_nth(it, n - 2);
    if (ptr == NULL || ptr->next == NULL || ptr->next->next == NULL) return;

    ListItem* x = ptr->next; // ptr x tgt y
    ptr->next = x->next;            // ptr tgt y | x tgt
    x->next = ptr->next->next;      // ptr tgt y | x y
    ptr->next->next = x;            // ptr tgt x y
  }
}

void list_move_down(List* it, int n)
{
  if (n == (list_length(it) - 1)) return;

  if (n == 0)
  {
    ListItem* x = it->head;
    if (x->next == NULL) return;

    ListItem* tgt = x->next; // x tgt cdr
    x->next = tgt->next;            // x cdr | tgt cdr
    it->head = tgt;                 // tgt cdr | x cdr
    tgt->next = x;                  // tgt x cdr
  }
  else
  {
    ListItem* ptr = _list_nth(it, n - 1);
    if (ptr == NULL || ptr->next == NULL) return;

    ListItem* tgt = ptr->next; // ptr tgt x y
    ptr->next = tgt->next;            // ptr x | tgt | y
    tgt->next = ptr->next->next;      // ptr x | tgt y
    ptr->next->next = tgt;            // ptr x tgt y
  }
}

void list_destruct(List* it)
{
  while (it->head != NULL) list_remove(it, 0);
  free(it);
}

