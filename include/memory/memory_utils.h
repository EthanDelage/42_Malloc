#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H

#include <stddef.h>

size_t align_page(size_t size);
size_t align_size(size_t size);
void *map_heap_region(size_t size);
int unmap_heap_region(void *addr, size_t size);

#endif // MEMORY_UTILS_H
