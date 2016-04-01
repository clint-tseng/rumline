#ifndef __GROUP_MENU
#define __GROUP_MENU

#include <pebble.h>

#include "../app.h"
#include "../data/group.h"
#include "mark-screen.h"

struct groupmenudata
{
  App* app;
  Group* group;
  SimpleMenuLayer* menu;
  SimpleMenuSection* menu_section;
  MarkScreen* child;
};
typedef struct groupmenudata GroupMenuData;

typedef Window GroupMenu;

GroupMenu* group_menu_show(App* app, Group* g);
void group_menu_destruct(GroupMenu* w);

#endif

