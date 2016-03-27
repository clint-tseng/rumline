#ifndef __MARK
#define __MARK

#include "./linked-list.h"

struct mark
{
  short id;
  char* name;
  double lat;
  double lon;
};

double to_radians(double deg);
double to_degrees(double rad);

double mark_distance(struct mark* from, struct mark* to);
double mark_bearing(struct mark* from, struct mark* to);

size_t size_mark(struct mark* it);
void serialize_mark(struct mark* it, void* buffer);
void deserialize_mark(struct mark* it, void* buffer);

size_t size_marks(struct list* mark_list);
void serialize_marks(struct list* mark_list, void* buffer);

#endif

