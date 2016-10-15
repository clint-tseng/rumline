#ifndef __PBSTORE
#define __PBSTORE

bool pbstore_exists();
void pbstore_set(void* buffer, size_t bytes);
void* pbstore_get();

bool recents_exists();
void recents_set(short* ids, size_t bytes);
short* recents_get();

#endif

