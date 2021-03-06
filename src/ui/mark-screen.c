#include <pebble.h>
#include <stdlib.h>

#include "../app.h"
#include "../pb/pbcomm.h"
#include "../pb/pbstore.h"

#include "mark-screen.h"

// I really hate this but the pebble tick service does not permite a context argument.
static MarkScreenData* current_mark_screen = NULL;

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

  data->name = text_layer_create(GRect(5, 5, bounds.size.w - 40, 16));
  text_layer_set_background_color(data->name, GColorClear);
  text_layer_set_text_color(data->name, GColorChromeYellow);
  text_layer_set_overflow_mode(data->name, GTextOverflowModeTrailingEllipsis);
  text_layer_set_font(data->name, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(data->name, GTextAlignmentLeft);
  text_layer_set_text(data->name, data->mark->name);
  layer_add_child(window_layer, text_layer_get_layer(data->name));

  data->clock = text_layer_create(GRect(bounds.size.w - 35, 5, 30, 16));
  text_layer_set_background_color(data->clock, GColorClear);
  text_layer_set_text_color(data->clock, GColorChromeYellow);
  text_layer_set_font(data->clock, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(data->clock, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(data->clock));

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

  current_mark_screen = data;
  tick_timer_service_subscribe(MINUTE_UNIT, mark_screen_tick);
  mark_screen_tick(NULL, SECOND_UNIT);

  data->app->current_mark_screen = w;
}

void _mark_window_unload(Window* w)
{
  MarkScreenData* data = (MarkScreenData*) window_get_user_data(w);

  text_layer_destroy(data->name);
  text_layer_destroy(data->clock);
  text_layer_destroy(data->bearing_value);
  text_layer_destroy(data->bearing_label);
  text_layer_destroy(data->distance_value);
  text_layer_destroy(data->distance_label);

  current_mark_screen = NULL;
  tick_timer_service_unsubscribe();

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

  group_push_recent(app->recents, m);
  size_t group_size = size_group(app->recents);
  void* buffer = malloc(group_size);
  serialize_group(app->recents, buffer);
  pbstore_set(buffer, group_size, RECENTS_OFFSET);
  free(buffer);

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

void mark_screen_tick(struct tm* tick_time, TimeUnits time_changed)
{
  if (current_mark_screen != NULL)
  {
    // only ever take the xx:xx part of the string.
    clock_copy_time_string(current_mark_screen->time, 6);
    text_layer_set_text(current_mark_screen->clock, current_mark_screen->time);
  }
}

void mark_screen_destruct(MarkScreen* w)
{
  MarkScreenData* data = (MarkScreenData*) window_get_user_data(w);
  free(data);
  window_destroy(w);
}


