#ifndef __MARK
#define __MARK

#include "linked-list.h"

typedef List Recents;

Recents* recents_create();
void recents_push(Recents* recents, short id);

short* recents_flatten(Recents* recents);
Recents* recents_inflate(short* ids);

#endif

