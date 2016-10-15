#include <stdlib.h>

#include "recents.h"
#include "linked-list.h"

Recents* recents_create() { return (Recents*) list_create(); };

void recents_push(Recents* recents, short id)
{
  ListItem* cur = recents->head;
  int idx = 0;
  while (cur->next)
  {
    if (((short) cur->data) == id) break;
    cur = cur->next;
    idx++;
  }

  if (cur)
    list_remove(recents, idx);
  else if (idx > 6)
    list_remove(recents, 6);

  ListItem* new = malloc(sizeof (ListItem));
  new->next = recents->head;
  new->data = (void*) id;
  recents->head = new;
};

short* recents_flatten(Recents* recents)
{
  short* result = malloc((size_t) (list_length(recents) * sizeof (short)));
  ListItem* cur = recents->head;
  for (short i = 0; cur; i++)
  {
    result[i] = cur->data;
    cur = cur->next
  }
  return result;
};

Recents* recents_inflate(short* ids)
{
  Recents* result = (Recents*) list_create();

  short count = (sizeof ids) / (sizeof (short));
  for (int i = 0; i < count; i++) list_add(result, (void*) ids[i]);

  return result;
};

