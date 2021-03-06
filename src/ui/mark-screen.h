#ifndef __MARK_SCREEN
#define __MARK_SCREEN

#include "../app.h"
#include "../data/mark.h"

struct markscreendata
{
  App* app;
  Mark* mark;
  TextLayer* name;
  TextLayer* clock;
  TextLayer* bearing_value;
  TextLayer* bearing_label;
  TextLayer* distance_value;
  TextLayer* distance_label;

  char bearing[7];
  char distance[7];
  char time[15];
};
typedef struct markscreendata MarkScreenData;

typedef Window MarkScreen;

MarkScreen* mark_screen_show(App* app, Mark* m);
void mark_screen_update(MarkScreen* w, char* bearing, char* distance);
void mark_screen_tick(struct tm* tick_time, TimeUnits time_changed);
void mark_screen_destruct(MarkScreen* w);

#endif

