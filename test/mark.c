#include <stdio.h>
#include <stdlib.h>
#include "../src/mark.c"

int main(void)
{
  struct mark from;
  from.id = 1;
  from.lat = 47;
  from.lon = -122;
  from.name = "from mark";

  struct mark to;
  to.id = 2;
  to.lat = 49;
  to.lon = -121;
  to.name = "to mark";

  printf("bearing: %f (expects 18.128133)\n", mark_bearing(&from, &to));
  printf("distance: %f (expects 126.620125)\n", mark_distance(&from, &to));

  printf("\n");

  size_t bytes = size_mark(&from);
  printf("bytes: %d (expects 38)\n", (int) bytes);

  void* buffer = malloc(size_mark(&from));
  serialize_mark(&from, buffer);

  printf("serialized:\n");
  for (int i = 0; i < ((int) bytes); i++)
  {
    printf("%02x ", *(unsigned char*) (buffer + i) & 0xff);
    if (((i + 1) % 16) == 0) printf("\n");
  }

  printf("\n\n");

  struct mark after;
  deserialize_mark(&after, buffer);
  printf("reinflated:\nid: %d\nlat: %f\nlon: %f\nname: %s\n", after.id, after.lat, after.lon, after.name);
}

