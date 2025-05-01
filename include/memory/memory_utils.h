#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H

#include <stddef.h>

size_t page_align(size_t size);
void *map_heap_region(size_t size);

#endif // MEMORY_UTILS_H
