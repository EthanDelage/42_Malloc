#include "chunk/chunk_dll.h"
#include "chunk/chunk_utils.h"
#include "malloc.h"
#include "memory/chunk_header.h"
#include "memory/malloc_data.h"
#include "memory/memory_utils.h"
#include "memory/memory_zone.h"
#include "utils/printf.h"

static void free_normal_zone(chunk_header_t *chunk, page_header_t **head);
static void free_large_zone(chunk_header_t *chunk);

void print_page(page_header_t *page, const char *zone_str, size_t index);

void free(void *ptr) {
    printf("free(%p);\n", ptr);
    if (ptr == NULL) {
        return;
    }
    chunk_header_t *chunk = get_chunk_from_data(ptr);
    zone_type_t type = get_chunk_zone_type(chunk);

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
    // printf("free end\n");
}

static void free_normal_zone(chunk_header_t *chunk, page_header_t **head) {
    page_header_t *chunk_page = find_chunk_page(chunk, *head);

    if (chunk->previous == NULL) {
        chunk_page->alloc_list = chunk->next;
    }
    chunk_dll_transfer_sorted(&chunk_page->free_list, chunk);
    chunk->in_use = 0;
    chunk_coalesce_surrounding(chunk);
    // print_page(chunk_page, "undefined", 0);
    if (chunk_page->alloc_list == NULL &&
        (chunk_page->previous != NULL || chunk_page->next != NULL)) {
        if (chunk_page->previous == NULL) {
            *head = chunk_page->next;
        }
        if (unmap_heap_region(chunk_page, chunk_page->size) == -1) {
            // TODO: handle error
        }
    } else {
        update_max_free_chunk_size(chunk_page);
    }
    printf("\n\n");
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
