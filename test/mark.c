#include <stdio.h>
#include <stdlib.h>

#include "../src/mark.h"
#include "../src/linked-list.h"

void print_bytes(void* buffer, size_t bytes)
{
  for (int i = 0; i < ((int) bytes); i++)
  {
    printf("%02x ", *(unsigned char*) (buffer + i) & 0xff);
    if (((i + 1) % 16) == 0) printf("\n");
  }
}

void print_mark(struct mark* m)
{
  printf("id: %d\nlat: %f\nlon: %f\nname: %s\n", m->id, m->lat, m->lon, m->name);
}

void test_mark()
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
  printf("bytes: %d (expects 30)\n", (int) bytes);

  void* buffer = malloc(bytes);
  serialize_mark(&from, buffer);

  printf("serialized:\n");
  print_bytes(buffer, bytes);

  printf("\n\n");

  struct mark after;
  deserialize_mark(&after, buffer);
  printf("reinflated:\n");
  print_mark(&after);
  printf("\n");
}

void test_marks()
{
  struct mark x;
  x.id = 1;
  x.lat = 47;
  x.lon = -122;
  x.name = "x mark";

  struct mark y;
  y.id = 2;
  y.lat = 49;
  y.lon = -121;
  y.name = "y mark";

  struct list* l = malloc(sizeof (struct list));
  list_init(l);
  list_add(l, &x);
  list_add(l, &y);

  size_t bytes = size_marks(l);
  printf("mark list in bytes: %d (expects 56)\n", (int) bytes);

  void* buffer = malloc(bytes);
  serialize_marks(l, buffer);

  printf("serialized:\n");
  print_bytes(buffer, bytes);

  struct list* l2 = malloc(sizeof (struct list));
  list_init(l2);
  deserialize_marks(l2, buffer);

  printf("\n\ndeserialized marks: %d (expects 2)\n", list_length(l2));
  printf("first inflated mark:\n");
  print_mark((struct mark*) list_nth(l2, 0));
  printf("\nsecond inflated mark:\n");
  print_mark((struct mark*) list_nth(l2, 1));
  printf("\n");
}

