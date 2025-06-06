#ifndef MALLOC_H
#define MALLOC_H

#include "memory/page_header.h"

#include <stddef.h>

__attribute__((visibility("default"))) void free(void *ptr);
__attribute__((visibility("default"))) void *malloc(size_t size);
__attribute__((visibility("default"))) void *realloc(void *ptr, size_t size);
__attribute__((visibility("default"))) void *calloc(size_t nmemb, size_t size);
__attribute__((visibility("default"))) void show_alloc_mem();
void *reallocarray(void *ptr, size_t nmemb, size_t size);

#endif // MALLOC_H
