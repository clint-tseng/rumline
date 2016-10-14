#ifndef __PBCOMM
#define __PBCOMM

#include "data/mark.h"

typedef enum {
  dictBearing = 0, dictDistance, dictId,
  cfgMessage, cfgGroupName, cfgMarkId, cfgMarkName
} dictKey;

typedef enum {
  cfgMessage_start = 0, cfgMessage_group, cfgMessage_mark, cfgMessage_end
} cfgMessages;


void send_mark_to_phone(Mark* m);

#endif

