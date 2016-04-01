#include <pebble.h>
#include <stdlib.h>

#include "../app.h"
#include "../pb/pbcomm.h"

#include "mark-screen.h"

void _setup_text_value(TextLayer* l)
{
  text_layer_set_background_color(l, GColorClear);
  text_layer_set_text_color(l, GColorSpringBud);
  text_layer_set_font(l, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(l, GTextAlignmentCenter);
}

void _setup_text_label(TextLayer* l)
{
  text_layer_set_background_color(l, GColorClear);
  text_layer_set_text_color(l, GColorKellyGreen);
  text_layer_set_font(l, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(l, GTextAlignmentCenter);
}

void _draw_static_labels(Window* w)
{
  Layer* window_layer = window_get_root_layer(w);
  GRect bounds = layer_get_bounds(window_layer);

  MarkScreenData* data = (MarkScreenData*) window_get_user_data(w);

  data->name = text_layer_create(GRect(0, 0, bounds.size.w, 28));
  text_layer_set_background_color(data->name, GColorClear);
  text_layer_set_text_color(data->name, GColorChromeYellow);
  text_layer_set_font(data->name, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(data->name, GTextAlignmentCenter);
  text_layer_set_text(data->name, data->mark->name);
  layer_add_child(window_layer, text_layer_get_layer(data->name));

  data->bearing_value = text_layer_create(GRect(0, 23, bounds.size.w, 84));
  _setup_text_value(data->bearing_value);
  layer_add_child(window_layer, text_layer_get_layer(data->bearing_value));

  data->bearing_label = text_layer_create(GRect(0, 63, bounds.size.w, 36));
  _setup_text_label(data->bearing_label);
  text_layer_set_text(data->bearing_label, "bearing");
  layer_add_child(window_layer, text_layer_get_layer(data->bearing_label));

  data->distance_value = text_layer_create(GRect(0, 90, bounds.size.w, 84));
  _setup_text_value(data->distance_value);
  layer_add_child(window_layer, text_layer_get_layer(data->distance_value));

  data->distance_label = text_layer_create(GRect(0, 130, bounds.size.w, 36));
  _setup_text_label(data->distance_label);
  text_layer_set_text(data->distance_label, "nm distance");
  layer_add_child(window_layer, text_layer_get_layer(data->distance_label));
}

void _mark_window_load(Window* w)
{
  MarkScreenData* data = (MarkScreenData*) window_get_user_data(w);

  window_set_background_color(w, GColorOxfordBlue);
  _draw_static_labels(w);
  mark_screen_update(w, "...", "...");

  data->app->current_mark_screen = w;
}

void _mark_window_unload(Window* w)
{
  MarkScreenData* data = (MarkScreenData*) window_get_user_data(w);

  text_layer_destroy(data->name);
  text_layer_destroy(data->bearing_value);
  text_layer_destroy(data->bearing_label);
  text_layer_destroy(data->distance_value);
  text_layer_destroy(data->distance_label);

  data->app->current_mark_screen = NULL;
}

MarkScreen* mark_screen_show(App* app, Mark* m)
{
  Window* w = window_create();

  MarkScreenData* data = malloc(sizeof (MarkScreenData));
  data->app = app;
  data->mark = m;
  window_set_user_data(w, data);

  window_set_window_handlers(w, (WindowHandlers)
  {
    .load = _mark_window_load,
    .unload = _mark_window_unload
  });
  window_stack_push(w, true);

  send_mark_to_phone(data->mark);
  return w;
}

void mark_screen_update(MarkScreen* w, char* bearing, char* distance)
{
  MarkScreenData* data = (MarkScreenData*) window_get_user_data(w);

  snprintf(data->bearing, sizeof(data->bearing), "%s°", bearing);
  strncpy(data->distance, distance, sizeof(data->distance));

  text_layer_set_text(data->bearing_value, data->bearing);
  text_layer_set_text(data->distance_value, data->distance);
}

void mark_screen_destruct(MarkScreen* w)
{
  MarkScreenData* data = (MarkScreenData*) window_get_user_data(w);
  free(data);
  window_destroy(w);
}


