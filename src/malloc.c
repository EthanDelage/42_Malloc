#include "malloc.h"

#include "chunk/chunk_dll.h"
#include "chunk/chunk_utils.h"
#include "memory/malloc_data.h"
#include "memory/memory_utils.h"
#include "memory/memory_zone.h"

#include <stdio.h>

malloc_data_t malloc_data = {0};

static void *allocate_large_zone(size_t size);

void *malloc(size_t size) {
    zone_type_t type = get_zone_type(size);

    printf("malloc\n");
    if (type == TINY) {

    } else if (type == SMALL) {

    } else {
        return allocate_large_zone(size);
    }
    return NULL;
}

static void *allocate_large_zone(size_t size) {
    void *map_ptr;
    chunk_header_t *chunk_header;

    size = page_align(sizeof(chunk_header_t) + size);
    map_ptr = map_heap_region(size);
    chunk_header = (chunk_header_t *)map_ptr;
    chunk_header->size = size - sizeof(chunk_header_t);
    chunk_header->in_use = 1;
    chunk_header->next = NULL;
    chunk_header->previous = NULL;
    chunk_dll_insert_sorted(&malloc_data.large, chunk_header);
    return get_chunk_data(map_ptr);
}
