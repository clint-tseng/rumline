#include <pebble.h>

#include "pbcomm.h"

void send_mark_to_phone(Mark* m)
{
  DictionaryIterator* dict;
  AppMessageResult result = app_message_outbox_begin(&dict);

  if (result == APP_MSG_OK)
  {
    dict_write_int32(dict, dictId, m->id);
    result = app_message_outbox_send();
    // TODO: do something w/ result
  }
}

