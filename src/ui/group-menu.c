#include <pebble.h>
#include <stdlib.h>

#include "../app.h"
#include "group-menu.h"
#include "mark-screen.h"

void _group_menu_selected(int idx, void* context)
{
  GroupMenuData* data = (GroupMenuData*) context;

  if (data->child) mark_screen_destruct(data->child);
  data->child = mark_screen_show(data->app, (Mark*) list_nth(data->group->marks, idx));
}

void _group_window_load(Window* window)
{
  GroupMenuData* data = (GroupMenuData*) window_get_user_data(window);

  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  int mark_count = list_length(data->group->marks);
  SimpleMenuItem* menu_items = calloc(mark_count, sizeof (SimpleMenuItem));
  for (int i = 0; i < mark_count; i++)
  {
    Mark* m = list_nth(data->group->marks, i);
    menu_items[i].title = m->name;
    menu_items[i].callback = _group_menu_selected;
  }

  SimpleMenuSection* section = malloc(sizeof (SimpleMenuSection));
  section->title = data->group->name;
  section->items = menu_items;
  section->num_items = mark_count;

  SimpleMenuLayer* menu = simple_menu_layer_create(bounds, window, section, 1, data);

  data->menu = menu;
  data->menu_section = section;
  layer_add_child(window_layer, simple_menu_layer_get_layer(menu));
}

void _group_window_unload(Window* window)
{
  GroupMenuData* data = (GroupMenuData*) window_get_user_data(window);
  free((SimpleMenuItem*) data->menu_section->items);
  free(data->menu_section);
  mark_screen_destruct(data->child);
  simple_menu_layer_destroy(data->menu);
}

GroupMenu* group_menu_show(App* app, Group* g)
{
  Window* w = window_create();

  GroupMenuData* data = malloc(sizeof (GroupMenuData));
  data->group = g;
  data->child = NULL;
  window_set_user_data(w, data);

  data->app = app;

  window_set_window_handlers(w, (WindowHandlers)
  {
    .load = _group_window_load,
    .unload = _group_window_unload,
  });
  window_stack_push(w, true);

  return w;
}

void group_menu_destruct(GroupMenu* w)
{
  GroupMenuData* data = (GroupMenuData*) window_get_user_data(w);
  free(data);
  window_destroy(w);
}


