#ifndef __APP
#define __APP

#include "./linked-list.h"
#include "./group.h"
#include "./mark.h"

struct app
{
  struct list* groups;

  struct group* current_group;
  struct mark* current_mark;

  bool has_fix;
  double lat;
  double lon;
  bool showing_mark;
};
typedef struct app App;

#endif

