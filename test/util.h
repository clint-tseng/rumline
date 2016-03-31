#ifndef __TEST_UTIL
#define __TEST_UTIL

#include "../src/mark.h"
#include "../src/group.h"

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
  printf("id: %d\nname: %s\n", m->id, m->name);
}

void print_group(struct group* g)
{
  printf("name: %s\n", g->name);

  printf("marks:");
  int mark_count = list_length(g->marks);
  for (int i = 0; i < mark_count; i++)
  {
    printf("\n%d:\n", i);
    print_mark(list_nth(g->marks, i));
  }
}

#endif

