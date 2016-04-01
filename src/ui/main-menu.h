#ifndef __MAIN_MENU
#define __MAIN_MENU

#include <pebble.h>

#include "../app.h"
#include "group-menu.h"

struct mainmenudata
{
  App* app;
  SimpleMenuLayer* menu;
  SimpleMenuSection* menu_section;
  GroupMenu* child;
};
typedef struct mainmenudata MainMenuData;

typedef Window MainMenu;

MainMenu* main_menu_show(App* app);
void main_menu_destruct(MainMenu* w);

#endif

