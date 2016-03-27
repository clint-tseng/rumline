#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "./mark.h"
#include "./linked-list.h"

const int EARTH_METERS = 6371000;
const int NM = 1852;
const double PI = 3.14159265358979323846264338327;

double to_radians(double deg) { return deg / 180 * PI; }
double to_degrees(double rad) { return rad * 180 / PI; }
double square(double x) { return x * x; }

double mark_distance(struct mark* from, struct mark* to)
{
  double phi_from = to_radians(from->lat);
  double phi_to = to_radians(to->lat);
  double delta_phi = to_radians(to->lat - from->lat);
  double delta_lambda = to_radians(to->lon - from->lon);

  double a = square(sin(delta_phi / 2)) +
             cos(phi_from) * cos(phi_to) * square(sin(delta_lambda / 2));
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  double d = EARTH_METERS * c;

  return d / NM;
}

double mark_bearing(struct mark* from, struct mark* to)
{
  double phi_from = to_radians(from->lat);
  double phi_to = to_radians(to->lat);
  double delta_phi = to_radians(to->lat - from->lat);

  double lambda_from = to_radians(from->lon);
  double lambda_to = to_radians(to->lon);
  double delta_lambda = to_radians(to->lon - from->lon);

  double y = sin(delta_lambda) * cos(phi_to);
  double x = cos(phi_from) * sin(phi_to) -
             sin(phi_from) * cos(phi_to) * cos(delta_lambda);

  return to_degrees(atan2(y, x));
}

size_t size_mark(struct mark* it)
{
  return (sizeof it->lat) + (sizeof it->lon) + (sizeof (short)) + strlen(it->name) + 1;
}

void serialize_mark(struct mark* it, void* buffer)
{
  *(double*) buffer = it->lat;
  buffer += sizeof it->lat;
  *(double*) buffer = it->lon;
  buffer += sizeof it->lon;

  size_t name_length = strlen(it->name) + 1;
  *(short*) buffer = name_length;
  buffer += sizeof (short);

  strcpy(buffer, it->name);
}

void deserialize_mark(struct mark* it, void* buffer)
{
  it->lat = *(double*) buffer;
  buffer += sizeof it->lat;
  it->lon = *(double*) buffer;
  buffer += sizeof it->lon;

  size_t name_length = *(short*) buffer;
  buffer += sizeof (short);

  it->name = malloc(name_length);
  strncpy(it->name, buffer, name_length);
}

size_t size_marks(struct list* mark_list)
{
  size_t mark_sizes = 0;
  int mark_count = list_length(mark_list);
  for (int i = 0; i < mark_count; i++) mark_sizes += size_mark((struct mark*) list_nth(mark_list, i));

  return (sizeof (short)) + mark_sizes;
}

void serialize_marks(struct list* mark_list, void* buffer)
{
  *(short*) buffer = list_length(mark_list);
  buffer += sizeof (short);

  int mark_count = list_length(mark_list);
  for (int i = 0; i < mark_count; i++)
  {
    struct mark* m = (struct mark*) list_nth(mark_list, i);
    serialize_mark(m, buffer);
    buffer += size_mark(m);
  }
}

void deserialize_marks(struct list* mark_list, void* buffer)
{
  int mark_count = *(short*) buffer;
  buffer += sizeof (short);

  for (int i = 0; i < mark_count; i++)
  {
    struct mark* m = malloc(sizeof (struct mark));
    deserialize_mark(m, buffer);
    list_add(mark_list, m);
    buffer += size_mark(m);
  }
}

