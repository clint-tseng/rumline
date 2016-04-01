#include <pebble.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "./pbstore.h"
#include "./pbcomm.h"

#include "./linked-list.h"
#include "./mark.h"
#include "./group.h"
#include "./app.h"

#include "ui/main-menu.h"
#include "ui/mark-screen.h"


// STATE
static App* app;
static MainMenu* main_menu;


// OVERALL UI

void reload_ui()
{
}


// MESSAGE HANDLER

static List* cfg_groups;
static void inbox_received_callback(DictionaryIterator* iter, void* context)
{
  App* app = (App*) context;
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
    if (app->current_mark_screen)
    {
      mark_screen_update(app->current_mark_screen,
        dict_find(iter, dictBearing)->value->cstring,
        dict_find(iter, dictDistance)->value->cstring);
    }
  }
}


// DATA

void load_data(App* app)
{
  if (pbstore_exists())
  {
    void* buffer = pbstore_get();
    deserialize_groups(app->groups, buffer);
    free(buffer);
  }
}

void destroy_data(App* app)
{
  groups_destruct(app->groups);
  free(app);
}


// APP

static void init(void)
{
  app = malloc(sizeof (struct app));
  app->groups = list_create();
  load_data(app);

  app_message_open(APP_MESSAGE_INBOX_SIZE_MINIMUM, APP_MESSAGE_OUTBOX_SIZE_MINIMUM);
  app_message_set_context(app);
  app_message_register_inbox_received(inbox_received_callback);

  main_menu = main_menu_show(app);
}

static void deinit(void)
{
  destroy_data(app);
  main_menu_destruct(main_menu);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

