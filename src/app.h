#ifndef __APP
#define __APP

#include <pebble.h>

#include "data/linked-list.h"
#include "data/group.h"
#include "data/mark.h"
#include "data/recents.h"

struct app
{
  List* groups;
  Window* current_mark_screen;
  Group* recents;
};
typedef struct app App;

#endif

