#include "chunk/chunk_dll.h"
#include "chunk/chunk_utils.h"
#include "malloc.h"
#include "memory/chunk_header.h"
#include "memory/malloc_data.h"
#include "memory/memory_utils.h"
#include "memory/memory_zone.h"
#include "utils/printf.h"

#include <stdlib.h>

static void free_normal_zone(chunk_header_t *chunk, page_header_t **head);
static void free_large_zone(chunk_header_t *chunk);

void free(void *ptr) {
    static size_t count = 0;
    // printf("free(%p); //%zu\n", ptr, count);
    count++;
    // if (count > 50500) {
    //     show_alloc_mem();
    // }
    if (ptr == NULL) {
        return;
    }
    chunk_header_t *chunk = get_chunk_from_data(ptr);
    zone_type_t type = get_chunk_zone_type(chunk);

    if (type == INVALID || chunk->in_use == 0) {
        // show_alloc_mem();
        printf("%p\n", ptr);
        printf("free(): invalid pointer\n");
        return;
        abort();
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
    if (chunk_page->alloc_list == NULL &&
        (chunk_page->previous != NULL || chunk_page->next != NULL)) {
        if (chunk_page->previous == NULL) {
            *head = chunk_page->next;
        }
        page_list_remove(chunk_page);
        if (unmap_heap_region(chunk_page, chunk_page->size) == -1) {
            printf("munmap(): failed\n");
            abort();
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
        printf("munmap(): failed\n");
        abort();
    }
}
