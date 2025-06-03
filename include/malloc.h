#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>

__attribute__((visibility("default"))) void free(void *ptr);
__attribute__((visibility("default"))) void *malloc(size_t size);
__attribute__((visibility("default"))) void *realloc(void *ptr, size_t size);
__attribute__((visibility("default"))) void show_alloc_mem();

#endif // MALLOC_H
