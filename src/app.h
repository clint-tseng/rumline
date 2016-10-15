#ifndef __APP
#define __APP

#include <pebble.h>

#include "data/linked-list.h"
#include "data/group.h"
#include "data/mark.h"

// K
const size_t MAIN_OFFSET = 0;

// data structures
struct app
{
  List* groups;
  Window* current_mark_screen;
};
typedef struct app App;

#endif

