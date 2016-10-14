#include <stdlib.h>

#include "recents.h"
#include "linked-list.h"

Recents* recents_create() { return (Recents*) list_create(); };

void recents_push(Recents* recents, int id)
{
  ListItem* cur = recents->head;
  int idx = 0;
  while (cur->next)
  {
    if (cur->data == id) break;
    cur = cur->next;
    idx++;
  }

  if (cur)
    list_remove(recents, idx);
  else if (idx > 6)
    list_remove(recents, 6);

  ListItem* new = malloc(sizeof (ListItem));
  new->next = recents->head;
  recents->head = new;
};

int* recents_flatten(Recents* recents)
{
  int* result = malloc((size_t) (list_length(recents) * sizeof (int)));
  ListItem* cur = recents->head;
  for (int i = 0; cur; i++)
  {
    result[i] = cur->data;
    cur = cur->next
  }
  return result;
};

