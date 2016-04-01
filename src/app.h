#ifndef __APP
#define __APP

#include <pebble.h>

#include "./linked-list.h"
#include "./group.h"
#include "./mark.h"

struct app
{
  List* groups;
  Window* current_mark_screen;
};
typedef struct app App;

#endif

