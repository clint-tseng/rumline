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

  struct list* l = malloc(sizeof (struct list));
  list_init(l);
  list_add(l, g);
  list_add(l, g2);

  size_t groups_size = size_groups(l);
  printf("\ngroup list size: %d (expects 76)\n", (int) groups_size);

  void* b2 = malloc(groups_size);
  serialize_groups(l, b2);
  printf("\nserialized group:\n");
  print_bytes(b2, groups_size);

  struct list* l2 = malloc(sizeof (struct list));
  list_init(l2);

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

