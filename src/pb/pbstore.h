#ifndef __PBSTORE
#define __PBSTORE

// K
static const size_t MAIN_OFFSET = 0;
static const size_t RECENTS_OFFSET = 1000000;

// procs
bool pbstore_exists(size_t offset);
void pbstore_set(void* buffer, size_t bytes, size_t offset);
void* pbstore_get(size_t offset);

#endif

