#ifndef __PBSTORE
#define __PBSTORE

bool pbstore_exists(size_t offset);
void pbstore_set(void* buffer, size_t bytes, size_t offset);
void* pbstore_get(size_t offset);

#endif

