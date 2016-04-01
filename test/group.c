#include <stdlib.h>

#include "util.h"
#include "../src/linked-list.h"
#include "../src/mark.h"
#include "../src/group.h"

void test_group()
{
  Group* g = group_create();
  g->name = heap_string("test group");

  Mark* x = mark_create();
  x->id = 5;
  x->name = heap_string("x mark");

  Mark* y = mark_create();
  y->id = 6;
  y->name = heap_string("y mark");

  list_add(g->marks, x);
  list_add(g->marks, y);

  size_t bytes = size_group(g);
  void* buffer = malloc(bytes);

  serialize_group(g, buffer);

  printf("serialized group:\n");
  print_bytes(buffer, bytes);

  Group* g2 = group_create();
  deserialize_group(g2, buffer);

  printf("\n\ninflated group:\n");
  print_group(g2);

  List* l = list_create();
  list_add(l, g);
  list_add(l, g2);

  size_t groups_size = size_groups(l);
  printf("\ngroup list size: %d (expects 76)\n", (int) groups_size);

  void* b2 = malloc(groups_size);
  serialize_groups(l, b2);
  printf("\nserialized group:\n");
  print_bytes(b2, groups_size);

  List* l2 = list_create();
  deserialize_groups(l2, b2);
  printf("\n\ninflated group count: %d (expects 2)\n", list_length(l2));

  printf("\ninflated groups:\n");
  for (int i = 0; i < 2; i++)
  {
    printf("%d:\n", i);
    print_group((struct group*) list_nth(l2, i));
    printf("\n");
  }

  group_destruct(g);
  group_destruct(g2);

  printf("destructed both groups.\n");
}

