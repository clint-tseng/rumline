#include <pebble.h>
#include <stdlib.h>

#include "../app.h"
#include "main-menu.h"
#include "group-menu.h"

void _main_menu_selected(int idx, void* context)
{
  MainMenuData* data = (MainMenuData*) context;

  data->app->current_group = (Group*) list_nth(data->app->groups, idx);

  if (data->child) group_menu_destruct(data->child);
  data->child = group_menu_show(data->app);
}

void _main_window_load(Window* window)
{
  MainMenuData* data = (MainMenuData*) window_get_user_data(window);
  App* app = data->app;

  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  int group_count = list_length(app->groups);
  SimpleMenuItem* menu_items = calloc(group_count, sizeof (SimpleMenuItem));
  for (int i = 0; i < group_count; i++)
  {
    Group* g = list_nth(app->groups, i);
    menu_items[i].title = g->name;
    menu_items[i].callback = _main_menu_selected;
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
  free((SimpleMenuItem*) data->menu_section->items);
  free(data->menu_section);
  group_menu_destruct(data->child);
  simple_menu_layer_destroy(data->menu);
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


