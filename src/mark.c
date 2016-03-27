#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const int EARTH_METERS = 6371000;
const int NM = 1852;
const double PI = 3.14159265358979323846264338327;

double to_radians(double deg) { return deg / 180 * PI; }
double to_degrees(double rad) { return rad * 180 / PI; }
//double clean_degrees(double deg) { return deg + 360 % 360; }
double square(double x) { return x * x; }

struct mark
{
  short id;
  char* name;
  double lat;
  double lon;
};

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
  return (sizeof it->id) + (sizeof it->lat) + (sizeof it->lon) + (sizeof (short)) + strlen(it->name) + 1;
}

void serialize_mark(struct mark* it, void* buffer)
{
  *(short*) buffer = it->id;
  buffer += sizeof it->id;

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
  it->id = *(short*)buffer;
  buffer += sizeof it->id;

  it->lat = *(double*) buffer;
  buffer += sizeof it->lat;
  it->lon = *(double*) buffer;
  buffer += sizeof it->lon;

  size_t name_length = *(short*) buffer;
  buffer += sizeof (short);

  it->name = malloc(name_length);
  strncpy(it->name, buffer, name_length);
}

//void serialize_marks(struct mar

