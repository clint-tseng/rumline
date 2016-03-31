#ifndef __PBSTORE
#define __PBSTORE

bool pbstore_exists();
void pbstore_set(void* buffer, size_t bytes);
void* pbstore_get();

#endif

