#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "../src/data/mark.h"
#include "../src/data/linked-list.h"

void test_mark()
{
  Mark from;
  from.id = 1;
  from.name = heap_string("from mark");

  Mark to;
  to.id = 2;
  to.name = heap_string("to mark");

  size_t bytes = size_mark(&from);
  printf("bytes: %d (expects 14)\n", (int) bytes);

  void* buffer = malloc(bytes);
  serialize_mark(&from, buffer);

  printf("serialized:\n");
  print_bytes(buffer, bytes);

  printf("\n\n");

  Mark after;
  deserialize_mark(&after, buffer);
  printf("reinflated:\n");
  print_mark(&after);
  printf("\n");
}

void test_marks()
{
  Mark x;
  x.id = 3;
  x.name = heap_string("x mark");

  Mark y;
  y.id = 4;
  y.name = heap_string("y mark");

  List* l = list_create();
  list_add(l, &x);
  list_add(l, &y);

  size_t bytes = size_marks(l);
  printf("mark list in bytes: %d (expects 24)\n", (int) bytes);

  void* buffer = malloc(bytes);
  serialize_marks(l, buffer);

  printf("serialized:\n");
  print_bytes(buffer, bytes);

  List* l2 = list_create();
  deserialize_marks(l2, buffer);

  printf("\n\ndeserialized marks: %d (expects 2)\n", list_length(l2));
  printf("first inflated mark:\n");
  print_mark((Mark*) list_nth(l2, 0));
  printf("\nsecond inflated mark:\n");
  print_mark((Mark*) list_nth(l2, 1));
  printf("\n");
}

