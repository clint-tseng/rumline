#ifndef __APP
#define __APP

#include <pebble.h>

#include "data/linked-list.h"
#include "data/group.h"
#include "data/mark.h"

// data structures
struct app
{
  List* groups;
  Window* current_mark_screen;
  Group* recents;
};
typedef struct app App;

// stubs
void destroy_data(App* app);
void load_data(App* app);

#endif

