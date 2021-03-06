#include <pebble.h>
#include <stdlib.h>

#include "../app.h"
#include "main-menu.h"
#include "group-menu.h"

void _main_menu_selected(int idx, void* context)
{
  MainMenuData* data = (MainMenuData*) context;

  if (data->child) group_menu_destruct(data->child);
  if (idx == 0)
    data->child = group_menu_show(data->app, data->app->recents);
  else
    data->child = group_menu_show(data->app, (Group*) list_nth(data->app->groups, idx - 1));
}

void _main_window_load(Window* window)
{
  MainMenuData* data = (MainMenuData*) window_get_user_data(window);
  App* app = data->app;

  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  int group_count = list_length(app->groups) + 1;
  SimpleMenuItem* menu_items = calloc(group_count, sizeof (SimpleMenuItem));
  for (int i = 0; i < group_count; i++)
  {
    Group* g;
    if (i == 0)
      g = app->recents;
    else
      g = list_nth(app->groups, i - 1);
    menu_items[i].title = g->name;
    menu_items[i].callback = _main_menu_selected;

    if (i > 0)
    {
      char* subtitle = calloc(9, sizeof (char));
      int mark_count = list_length(g->marks);
      snprintf(subtitle, 9, "%d mark%c", mark_count, (mark_count == 1) ? ' ' : 's');
      menu_items[i].subtitle = subtitle;
    }
  }

  SimpleMenuSection* section = malloc(sizeof (SimpleMenuSection));
  section->title = "Where to?";
  section->items = menu_items;
  section->num_items = group_count;

  SimpleMenuLayer* menu = simple_menu_layer_create(bounds, window, section, 1, data);

  data->menu = menu;
  data->menu_section = section;
  layer_add_child(window_layer, simple_menu_layer_get_layer(menu));
}

void _main_window_unload(Window* window)
{
  MainMenuData* data = (MainMenuData*) window_get_user_data(window);

  for (int i = 0; i < list_length(data->app->groups); i++)
    free((char*) data->menu_section->items[i].subtitle);

  free((SimpleMenuItem*) data->menu_section->items);
  free(data->menu_section);
  simple_menu_layer_destroy(data->menu);

  if (data->child)
  {
    group_menu_destruct(data->child);
    data->child = NULL;
  }
}

MainMenu* main_menu_show(App* app)
{
  Window* w = window_create();

  MainMenuData* data = malloc(sizeof (MainMenuData));
  data->child = NULL;
  window_set_user_data(w, data);

  data->app = app;

  window_set_window_handlers(w, (WindowHandlers)
  {
    .load = _main_window_load,
    .unload = _main_window_unload,
  });
  window_stack_push(w, true);

  return w;
}

void main_menu_destruct(MainMenu* w)
{
  MainMenuData* m = (MainMenuData*) window_get_user_data(w);
  free(m);
  window_destroy(w);
}


