#include <pebble.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "./pbstore.h"

#include "./linked-list.h"
#include "./mark.h"
#include "./group.h"
#include "./app.h"

static App* app;


// WIRE FORMAT

typedef enum {
  dictBearing = 0, dictDistance, dictId,
  cfgMessage, cfgGroupName, cfgMarkId, cfgMarkName
} dictKey;

typedef enum {
  cfgMessage_start = 0, cfgMessage_group, cfgMessage_mark, cfgMessage_end
} cfgMessages;


// MARK TO PHONE

void send_mark_to_phone(Mark* m)
{
  DictionaryIterator* dict;
  AppMessageResult result = app_message_outbox_begin(&dict);

  if (result == APP_MSG_OK)
  {
    dict_write_int32(dict, dictId, app->current_mark->id);
    result = app_message_outbox_send();
    // TODO: do something w/ result
  }
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
    text_layer_set_text(mark_text_bearing_value, text_bearing);
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

static void show_mark(Mark* m)
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

  send_mark_to_phone(m);
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
    Mark* m = list_nth(app->current_group->marks, i);
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

static void show_group_menu(Group* g)
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
    Group* g = list_nth(app->groups, i);
    menu_items[i].title = g->name;
    menu_items[i].callback = main_menu_selected;
  }

  main_menu_section = malloc(sizeof (SimpleMenuSection));
  main_menu_section->title = "Where to?";
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


// OVERALL UI

void reload_ui()
{
}


// MESSAGE HANDLER

static List* cfg_groups;
static void inbox_received_callback(DictionaryIterator* iter, void* context)
{
  Tuple* message_type_tuple = dict_find(iter, cfgMessage);

  if (message_type_tuple)
  {
    int message_type = message_type_tuple->value->int32;
    if (message_type == cfgMessage_start)
    {
      if (cfg_groups) groups_destruct(cfg_groups);
      cfg_groups = list_create();
    }
    else if (message_type == cfgMessage_group)
    {
      char* name = dict_find(iter, cfgGroupName)->value->cstring;
      Group* g = group_create();
      char* name_copy = malloc(strlen(name) + 1);
      strcpy(name_copy, name);
      g->name = name_copy;
      list_add(cfg_groups, g);
    }
    else if (message_type == cfgMessage_mark)
    {
      short id = (short) dict_find(iter, cfgMarkId)->value->int32;
      char* name = dict_find(iter, cfgMarkName)->value->cstring;
      Mark* m = mark_create();
      m->id = id;
      char* name_copy = malloc(strlen(name) + 1);
      strcpy(name_copy, name);
      m->name = name_copy;
      list_add(((Group*) list_last(cfg_groups))->marks, m);
    }
    else if (message_type == cfgMessage_end)
    {
      size_t bytes = size_groups(cfg_groups);
      void* save_buffer = malloc(bytes);
      serialize_groups(cfg_groups, save_buffer);
      pbstore_set(save_buffer, bytes);

      reload_ui();
    }
  }
  else
  {
    snprintf(text_bearing, sizeof(text_bearing), "%s°", dict_find(iter, dictBearing)->value->cstring);
    snprintf(text_distance, sizeof(text_distance), "%s", dict_find(iter, dictDistance)->value->cstring);
    app->has_fix = true;

    if (app->showing_mark) update_labels();
  }
}


// DATA

void load_data()
{
  app = malloc(sizeof (struct app));
  app->groups = list_create();

  if (pbstore_exists())
  {
    void* buffer = pbstore_get();
    deserialize_groups(app->groups, buffer);
    free(buffer);
  }
}

void destroy_data()
{
  groups_destruct(app->groups);
  free(app);
}


// APP

static void init(void)
{
  load_data();

  app_message_open(APP_MESSAGE_INBOX_SIZE_MINIMUM, APP_MESSAGE_OUTBOX_SIZE_MINIMUM);
  app_message_register_inbox_received(inbox_received_callback);
  show_main_menu();
}

static void deinit(void)
{
  destroy_data();

  window_destroy(main_window);
  window_destroy(group_window);
  window_destroy(mark_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

