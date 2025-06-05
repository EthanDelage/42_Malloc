#include "malloc.h"

#include "chunk/chunk_dll.h"
#include "chunk/chunk_search.h"
#include "chunk/chunk_utils.h"
#include "memory/malloc_data.h"
#include "memory/memory_utils.h"
#include "memory/memory_zone.h"
#include "utils/printf.h"

malloc_data_t malloc_data = {0};

static void *allocate_normal_zone(size_t size, zone_type_t zone_type,
                                  page_header_t **head);
static void *allocate_large_zone(size_t size);
static page_header_t *allocate_page(zone_type_t zone_type);

void *malloc(size_t size) {
    printf("malloc: %zu\n", size);
    zone_type_t type = get_zone_type(size);
    void *ptr;

    if (type == TINY) {
        ptr = allocate_normal_zone(size, type, &malloc_data.tiny);
    } else if (type == SMALL) {
        ptr = allocate_normal_zone(size, type, &malloc_data.small);
    } else {
        ptr = allocate_large_zone(size);
    }
    printf("%p\n", ptr);
    show_alloc_mem();
    return ptr;
}

static void *allocate_normal_zone(size_t size, zone_type_t zone_type,
                                  page_header_t **head) {
    page_header_t *page = find_available_page(*head, size);
    chunk_header_t *chunk = find_free_chunk(page, size);

    if (chunk == NULL) {
        page_list_insert(head, allocate_page(zone_type));
        return allocate_normal_zone(size, zone_type, head);
    }
    chunk_split(chunk, size);
    if (chunk->previous == NULL) {
        page->free_list = chunk->next;
    }
    chunk_dll_transfer_sorted(&page->alloc_list, chunk);
    update_max_free_chunk_size(page);
    chunk->in_use = 1;
    return get_chunk_data(chunk);
}

static void *allocate_large_zone(size_t size) {
    void *map_ptr;
    chunk_header_t *chunk;

    size = align_page(sizeof(chunk_header_t) + size);
    map_ptr = map_heap_region(size);
    chunk = (chunk_header_t *)map_ptr;
    chunk->size = size - sizeof(chunk_header_t);
    chunk->in_use = 1;
    chunk->next = NULL;
    chunk->previous = NULL;
    chunk_dll_insert_sorted(&malloc_data.large, chunk);
    return get_chunk_data(map_ptr);
}

static page_header_t *allocate_page(zone_type_t zone_type) {
    size_t zone_size = get_zone_size(zone_type);
    void *map_ptr;
    page_header_t *page;
    chunk_header_t *chunk;

    map_ptr = map_heap_region(zone_size);
    page = (page_header_t *)map_ptr;
    chunk = (chunk_header_t *)((uint8_t *)map_ptr + sizeof(page_header_t));
    page->alloc_list = NULL;
    page->free_list = chunk;
    page->size = zone_size;
    page->next = NULL;
    page->previous = NULL;
    chunk->size = zone_size - (sizeof(page_header_t) + sizeof(chunk_header_t));
    page->max_free_chunk_size = chunk->size;
    chunk->in_use = 0;
    chunk->next = NULL;
    chunk->previous = NULL;
    return page;
}
