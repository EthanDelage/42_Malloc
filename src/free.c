#include <stdio.h>

#include "chunk/chunk_dll.h"
#include "chunk/chunk_utils.h"
#include "malloc.h"
#include "memory/chunk_header.h"
#include "memory/malloc_data.h"
#include "memory/memory_utils.h"
#include "memory/memory_zone.h"

static void free_normal_zone(chunk_header_t *chunk, page_header_t **head);
static void free_large_zone(chunk_header_t *chunk);

void free(void *ptr) {
    chunk_header_t *chunk = get_chunk_from_data(ptr);
    zone_type_t type = get_chunk_zone_type(chunk);

    printf("free\n");
    if (type == INVALID || chunk->in_use == 0) {
        // TODO: handle error (invalid ptr)
        return;
    }
    if (type == TINY) {
        free_normal_zone(chunk, &malloc_data.tiny);
    } else if (type == SMALL) {
        free_normal_zone(chunk, &malloc_data.small);
    } else if (type == LARGE) {
        free_large_zone(chunk);
    }
}

static void free_normal_zone(chunk_header_t *chunk, page_header_t **head) {
    page_header_t *chunk_page = find_chunk_page(chunk, *head);

    if (chunk->previous == NULL) {
        chunk_page->alloc_list = chunk->next;
    }
    chunk_dll_transfer_sorted(&chunk_page->free_list, chunk);
    chunk->in_use = 0;
    chunk_coalesce_surrounding(chunk);
    if (chunk_page->alloc_list == NULL) {
        if (chunk_page->previous == NULL) {
            *head = chunk_page->next;
        }
        if (unmap_heap_region(chunk_page, chunk_page->size) == -1) {
            // TODO: handle error
        }
    } else {
        update_max_free_chunk_size(chunk_page);
    }
}

static void free_large_zone(chunk_header_t *chunk) {
    size_t page_size = sizeof(chunk_header_t) + chunk->size;

    if (chunk->previous == NULL) {
        malloc_data.large = chunk->next;
    }
    chunk_dll_remove(chunk);
    if (unmap_heap_region(chunk, page_size) == -1) {
        // TODO: handle error
    }
}
