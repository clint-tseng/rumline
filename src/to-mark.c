#include <pebble.h>
#include <stdlib.h>
#include <math.h>

#include "./ext/strtod.h"

#include "./linked-list.h"
#include "./mark.h"
#include "./group.h"
#include "./app.h"

static App* app;

// TESTING
static void init_test_data()
{
  struct list* groups = malloc(sizeof (struct list));
  list_init(groups);

  struct group* shilshole = malloc(sizeof (struct group));
  shilshole->name = "Shilsole Bay";
  list_add(groups, shilshole);

  struct list* marks = malloc(sizeof (struct list));
  list_init(marks);
  shilshole->marks = marks;

  struct mark* start = malloc(sizeof (struct mark));
  start->name = "N - N Hamburger";
  start->lat = 47.68692;
  start->lon = -122.41055;
  list_add(marks, start);

  struct mark* meadow_point = malloc(sizeof (struct mark));
  meadow_point->name = "M - Meadow Point";
  meadow_point->lat = 47.69639;
  meadow_point->lon = -122.40992;
  list_add(marks, meadow_point);

  struct mark* west_point = malloc(sizeof (struct mark));
  west_point->name = "W - West Point";
  west_point->lat = 47.66020;
  west_point->lon = -122.44150;
  list_add(marks, west_point);

  struct mark* blinker = malloc(sizeof (struct mark));
  blinker->name = "E - Ballard Blinker";
  blinker->lat = 47.67770;
  blinker->lon = -122.41599;
  list_add(marks, blinker);

  app = malloc(sizeof (App));
  app->groups = groups;
  app->has_fix = false;
  app->showing_mark = false;
}

static void destroy_test_data()
{
  struct group* g = list_nth(app->groups, 0);
  group_free_marks(g);
  group_destruct(g);

  list_destruct(app->groups);
  free(app);
}


// MARK DISPLAY

static Window* mark_window;
static TextLayer* mark_text_name;
static TextLayer* mark_text_bearing_value;
static TextLayer* mark_text_bearing_label;
static TextLayer* mark_text_distance_value;
static TextLayer* mark_text_distance_label;
static char text_bearing[7];
static char text_distance[7];

static void setup_text_value(TextLayer* l)
{
  text_layer_set_background_color(l, GColorClear);
  text_layer_set_text_color(l, GColorSpringBud);
  text_layer_set_font(l, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(l, GTextAlignmentCenter);
}

static void setup_text_label(TextLayer* l)
{
  text_layer_set_background_color(l, GColorClear);
  text_layer_set_text_color(l, GColorKellyGreen);
  text_layer_set_font(l, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(l, GTextAlignmentCenter);
}

static void draw_static_labels(Layer* window_layer, GRect* bounds)
{
  mark_text_name = text_layer_create(GRect(0, 0, bounds->size.w, 28));
  text_layer_set_background_color(mark_text_name, GColorClear);
  text_layer_set_text_color(mark_text_name, GColorChromeYellow);
  text_layer_set_font(mark_text_name, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(mark_text_name, GTextAlignmentCenter);
  text_layer_set_text(mark_text_name, app->current_mark->name);
  layer_add_child(window_layer, text_layer_get_layer(mark_text_name));

  mark_text_bearing_value = text_layer_create(GRect(0, 23, bounds->size.w, 84));
  setup_text_value(mark_text_bearing_value);
  layer_add_child(window_layer, text_layer_get_layer(mark_text_bearing_value));

  mark_text_bearing_label = text_layer_create(GRect(0, 63, bounds->size.w, 36));
  setup_text_label(mark_text_bearing_label);
  text_layer_set_text(mark_text_bearing_label, "bearing");
  layer_add_child(window_layer, text_layer_get_layer(mark_text_bearing_label));

  mark_text_distance_value = text_layer_create(GRect(0, 90, bounds->size.w, 84));
  setup_text_value(mark_text_distance_value);
  layer_add_child(window_layer, text_layer_get_layer(mark_text_distance_value));

  mark_text_distance_label = text_layer_create(GRect(0, 130, bounds->size.w, 36));
  setup_text_label(mark_text_distance_label);
  text_layer_set_text(mark_text_distance_label, "nm distance");
  layer_add_child(window_layer, text_layer_get_layer(mark_text_distance_label));
}

static void update_labels()
{
  if (app->has_fix)
  {
    struct mark here;
    here.lat = app->lat;
    here.lon = app->lon;

    snprintf(text_bearing, sizeof text_bearing, "%d°", (int) round(mark_bearing(&here, app->current_mark)));
    text_layer_set_text(mark_text_bearing_value, text_bearing);

    double distance = mark_distance(&here, app->current_mark);
    if (distance < 100)
      snprintf(text_distance, sizeof text_distance, "%.2f", distance);
    else
      snprintf(text_distance, sizeof text_distance, "%d", (int) round(distance));
    text_layer_set_text(mark_text_distance_value, text_distance);
  }
  else
  {
    text_layer_set_text(mark_text_bearing_value, "...");
    text_layer_set_text(mark_text_distance_value, "...");
  }
}

static void mark_window_load(Window* window)
{
  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  window_set_background_color(window, GColorOxfordBlue);
  draw_static_labels(window_layer, &bounds);
  update_labels();

  app->showing_mark = true;
}

static void mark_window_unload(Window* window)
{
  text_layer_destroy(mark_text_name);
  text_layer_destroy(mark_text_bearing_value);
  text_layer_destroy(mark_text_bearing_label);
  text_layer_destroy(mark_text_distance_value);
  text_layer_destroy(mark_text_distance_label);

  app->showing_mark = false;
}

static void show_mark(struct mark* m)
{
  app->current_mark = m;

  window_destroy(mark_window);
  mark_window = window_create();
  window_set_window_handlers(mark_window, (WindowHandlers)
  {
    .load = mark_window_load,
    .unload = mark_window_unload
  });
  window_stack_push(mark_window, true);
}


// GROUP MENU

static Window* group_window;
static SimpleMenuLayer* group_menu;
static SimpleMenuSection* group_menu_section;

static void group_menu_selected(int idx, void* context)
{
  show_mark(list_nth(((App*) context)->current_group->marks, idx));
}

static void group_window_load(Window* window)
{
  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  int mark_count = list_length(app->current_group->marks);
  SimpleMenuItem* menu_items = calloc(mark_count, sizeof (SimpleMenuItem));
  for (int i = 0; i < mark_count; i++)
  {
    struct mark* m = list_nth(app->current_group->marks, i);
    menu_items[i].title = m->name;
    menu_items[i].callback = group_menu_selected;
  }

  group_menu_section = malloc(sizeof (SimpleMenuSection));
  group_menu_section->title = app->current_group->name;
  group_menu_section->items = menu_items;
  group_menu_section->num_items = mark_count;

  SimpleMenuLayer* main_menu = simple_menu_layer_create(bounds, window, group_menu_section, 1, app);
  layer_add_child(window_layer, simple_menu_layer_get_layer(main_menu));
}

static void group_window_unload(Window* window)
{
  free((SimpleMenuItem*) group_menu_section->items);
  free(group_menu_section);
  simple_menu_layer_destroy(group_menu);
}

static void show_group_menu(struct group* g)
{
  app->current_group = g;

  window_destroy(group_window);
  group_window = window_create();
  window_set_window_handlers(group_window, (WindowHandlers)
  {
    .load = group_window_load,
    .unload = group_window_unload,
  });
  window_stack_push(group_window, true);
}

// MAIN MENU

static Window* main_window;
static SimpleMenuLayer* main_menu;
static SimpleMenuSection* main_menu_section;

static void main_menu_selected(int idx, void* context)
{
  show_group_menu(list_nth(((App*) context)->groups, idx));
}

static void main_window_load(Window* window)
{
  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  int group_count = list_length(app->groups);
  SimpleMenuItem* menu_items = calloc(group_count, sizeof (SimpleMenuItem));
  for (int i = 0; i < group_count; i++)
  {
    struct group* g = list_nth(app->groups, i);
    menu_items[i].title = g->name;
    menu_items[i].callback = main_menu_selected;
  }

  main_menu_section = malloc(sizeof (SimpleMenuSection));
  main_menu_section->title = "to mark";
  main_menu_section->items = menu_items;
  main_menu_section->num_items = group_count;

  SimpleMenuLayer* main_menu = simple_menu_layer_create(bounds, window, main_menu_section, 1, app);
  layer_add_child(window_layer, simple_menu_layer_get_layer(main_menu));

}

static void main_window_unload(Window* window)
{
  free((SimpleMenuItem*) main_menu_section->items);
  free(main_menu_section);
  simple_menu_layer_destroy(main_menu);
}

static void show_main_menu()
{
  main_window = window_create();
  window_set_window_handlers(main_window, (WindowHandlers)
  {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(main_window, true);
}


// LAT/LONG UPDATE HANDLER
static void inbox_received_callback(DictionaryIterator* iter, void* context)
{
  Tuple* lat_tuple = dict_find(iter, 0);
  if (lat_tuple) app->lat = pdos_strtod(lat_tuple->value->cstring, NULL);
  Tuple* lon_tuple = dict_find(iter, 1);
  if (lon_tuple) app->lon = pdos_strtod(lon_tuple->value->cstring, NULL);

  if (lat_tuple && lon_tuple) app->has_fix = true;
  if (app->showing_mark) update_labels();
}


// APP

static void init(void)
{
  init_test_data();
  show_main_menu();
  app_message_register_inbox_received(inbox_received_callback);
}

static void deinit(void)
{
  destroy_test_data();
  window_destroy(main_window);
  window_destroy(group_window);
  window_destroy(mark_window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", main_window);

  app_event_loop();
  deinit();
}

