#include <stdio.h>
#include <stdlib.h>
#include "../src/linked-list.h"

void print_string_list(struct list* l)
{
  int size = list_length(l);
  for (int i = 0; i < size; i++) printf("%s ", (char*) list_nth(l, i));
}

void test_linked_list()
{
  char* x = "xylophone";
  char* y = "yankee";
  char* z = "zebra";

  struct list* l = malloc(sizeof (struct list));
  list_init(l);

  printf("initial list size: %d (expects 0)\n\n", list_length(l));

  list_add(l, x);
  list_add(l, y);
  list_add(l, z);
  printf("list size after adds: %d (expects 3)\n", list_length(l));
  printf("text of first item: %s (expects xylophone)\n", (char*) (list_nth(l, 0)));
  printf("text of second item: %s (expects yankee)\n", (char*) (list_nth(l, 1)));
  printf("text of third item: %s (expects zebra)\n\n", (char*) (list_nth(l, 2)));
  printf("text of last item: %s (expects zebra)\n\n", (char*) (list_last(l)));

  list_remove(l, 1);
  printf("list size after remove: %d (expects 2)\n", list_length(l));
  printf("text of first item: %s (expects xylophone)\n", (char*) (list_nth(l, 0)));
  printf("text of second item: %s (expects zebra)\n\n", (char*) (list_nth(l, 1)));

  list_remove(l, 1);
  printf("list size after remove: %d (expects 1)\n", list_length(l));
  printf("text of first item: %s (expects xylophone)\n\n", (char*) (list_nth(l, 0)));

  list_remove(l, 0);
  printf("list size after remove: %d (expects 0)\n\n", list_length(l));

  list_add(l, x);
  list_add(l, y);
  list_add(l, z);
  printf("list: ");
  print_string_list(l);
  printf("\n");

  list_move_up(l, 0);
  printf("\nmove up 0: ");
  print_string_list(l);
  printf("\n  (expects xylophone yankee zebra)");

  list_move_up(l, 1);
  printf("\nmove up 1: ");
  print_string_list(l);
  printf("\n  (expects yankee xylophone zebra)");

  list_move_up(l, 2);
  printf("\nmove up 2: ");
  print_string_list(l);
  printf("\n  (expects yankee zebra xylophone)\n");

  list_move_down(l, 2);
  printf("\nmove down 2: ");
  print_string_list(l);
  printf("\n    (expects yankee zebra xylophone)");

  list_move_down(l, 1);
  printf("\nmove down 1: ");
  print_string_list(l);
  printf("\n    (expects yankee xylophone zebra)");

  list_move_down(l, 0);
  printf("\nmove down 0: ");
  print_string_list(l);
  printf("\n    (expects xylophone yankee zebra)\n\n");

  list_remove(l, 0);
  list_remove(l, 0);

  list_move_up(l, 0);
  list_move_down(l, 0);

  printf("one item move: ");
  print_string_list(l);
  printf("\n      (expects zebra)\n\n");

  list_destruct(l);

  printf("list destructed.\n\n");
}

