#include "chunk/chunk_dll.h"
#include "chunk/chunk_search.h"
#include "chunk/chunk_utils.h"
#include "malloc.h"
#include "memory/malloc_data.h"
#include "memory/memory_zone.h"
#include "memory/page_header.h"
#include "utils/libft.h"
#include "utils/printf.h"

static chunk_header_t *realloc_expand(chunk_header_t *chunk, size_t size,
                                      zone_type_t type);
static chunk_header_t *realloc_shrink(chunk_header_t *chunk, size_t size,
                                      zone_type_t type);
static chunk_header_t *realloc_normal_zone(chunk_header_t *chunk, size_t size,
                                           zone_type_t type,
                                           page_header_t *head);
static chunk_header_t *realloc_large_zone(chunk_header_t *chunk, size_t size);
static chunk_header_t *realloc_with_copy(chunk_header_t *chunk, size_t size);
static chunk_header_t *extend_chunk_allocation(chunk_header_t *chunk,
                                               size_t size,
                                               page_header_t *page_head);

void *realloc(void *ptr, size_t size) {
    printf("realloc: %p -> %zu\n", ptr, size);
    if (ptr == NULL) {
        return malloc(size);
    }
    chunk_header_t *chunk = get_chunk_from_data(ptr);
    zone_type_t type = get_chunk_zone_type(chunk);

    if (type == INVALID) {
        // TODO: handle error (invalid ptr)
        return NULL;
    }
    if (size > chunk->size) {
        chunk = realloc_expand(chunk, size, type);
    } else if (size < chunk->size) {
        chunk = realloc_shrink(chunk, size, type);
    }
    if (chunk == NULL) {
        return NULL;
    }
    return get_chunk_data(chunk);
}

static chunk_header_t *realloc_expand(chunk_header_t *chunk, size_t size,
                                      zone_type_t type) {
    if (type == TINY) {
        return realloc_normal_zone(chunk, size, type, malloc_data.tiny);
    }
    if (type == SMALL) {
        return realloc_normal_zone(chunk, size, type, malloc_data.tiny);
    }
    if (type == LARGE) {
        return realloc_large_zone(chunk, size);
    }
    return chunk;
}

static chunk_header_t *realloc_shrink(chunk_header_t *chunk, size_t size,
                                      zone_type_t type) {
    chunk_header_t *new_chunk;
    zone_type_t new_type = get_zone_type(size);

    if (type != new_type) {
        return realloc_with_copy(chunk, size);
    }
    if (type != LARGE) {
        new_chunk = chunk_split(chunk, size);
        free(new_chunk);
    }
    return chunk;
}

static chunk_header_t *realloc_normal_zone(chunk_header_t *chunk, size_t size,
                                           zone_type_t type,
                                           page_header_t *head) {
    zone_type_t new_type = get_zone_type(size);

    if (type == new_type &&
        extend_chunk_allocation(chunk, size, head) != NULL) {
        return chunk;
    }
    return realloc_with_copy(chunk, size);
}

static chunk_header_t *realloc_large_zone(chunk_header_t *chunk, size_t size) {
    if (chunk->size >= size) {
        return chunk;
    }
    return realloc_with_copy(chunk, size);
}

static chunk_header_t *realloc_with_copy(chunk_header_t *chunk, size_t size) {
    void *new_ptr = malloc(size);
    chunk_header_t *new_chunk;

    if (new_ptr == NULL) {
        return NULL;
    }
    new_chunk = get_chunk_from_data(new_ptr);
    ft_memcpy(new_ptr, get_chunk_data(chunk), chunk->size);
    free(chunk);
    return new_chunk;
}

/**
 * @brief Attempts to extend the given chunk by merging it with the adjacent
 * free chunk.
 *
 * This function checks whether the next chunk is free and whether it provides
 * enough space to satisfy the requested `size`. If so, it transfers the next
 * chunk from the free list to the allocation list, coalesces the chunks,
 * updates metadata, and returns the newly extended chunk.
 *
 * @param chunk Pointer to the current allocated chunk to extend.
 * @param size The total size (excluding header) that the extended chunk must
 * accommodate.
 * @param page_head Pointer to the head of the page list to search for the
 * chunk.
 * @return Pointer to the extended chunk if successful, or NULL if extension was
 * not possible.
 */
static chunk_header_t *extend_chunk_allocation(chunk_header_t *chunk,
                                               size_t size,
                                               page_header_t *page_head) {
    chunk_header_t *next_chunk =
        (chunk_header_t *)((uint8_t *)chunk + sizeof(chunk_header_t) +
                           chunk->size);
    page_header_t *chunk_page;
    size_t split_size;

    if (!is_chunk_in_pages(page_head, chunk)) {
        next_chunk = NULL;
    }
    if (next_chunk == NULL || next_chunk->in_use ||
        chunk->size + sizeof(chunk_header_t) + next_chunk->size < size) {
        return NULL;
    }
    chunk_page = find_chunk_page(chunk, page_head);
    if (size > chunk->size + sizeof(chunk_header_t)) {
        split_size = size - (chunk->size + sizeof(chunk_header_t));
    } else {
        split_size = 0;
    }
    chunk_split(next_chunk, split_size);
    if (next_chunk->previous == NULL) {
        chunk_page->free_list = next_chunk->next;
    }
    chunk_dll_transfer_sorted(&chunk_page->alloc_list, next_chunk);
    chunk_coalesce(chunk);
    update_max_free_chunk_size(chunk_page);
    return chunk;
}
