#ifndef __MARK
#define __MARK

#include "linked-list.h"

typedef List Recents;

Recents* recents_create();
void recents_push(Recents* recents, int id);
int* recents_flatten(Recents* recents);

#endif

