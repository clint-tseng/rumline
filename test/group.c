#include <stdlib.h>

#include "./util.h"
#include "../src/linked-list.h"
#include "../src/mark.h"
#include "../src/group.h"

void test_group()
{
  struct group* g = malloc(sizeof (struct group));
  group_init(g);

  g->name = "test group";

  struct mark x;
  x.id = 5;
  x.name = "x mark";

  struct mark y;
  y.id = 6;
  y.name = "y mark";

  list_add(g->marks, &x);
  list_add(g->marks, &y);

  size_t bytes = size_group(g);
  void* buffer = malloc(bytes);

  serialize_group(g, buffer);

  printf("serialized group:\n");
  print_bytes(buffer, bytes);

  struct group* g2 = malloc(sizeof (struct group));
  group_init(g2);
  deserialize_group(g2, buffer);

  printf("\n\ninflated group:\n");
  print_group(g2);

  group_destruct(g);
  group_destruct(g2);

  printf("\ndestructed both groups.\n");
}

