#include "chunk/chunk_search.h"

/**
 * @brief Searches for a specific chunk in the pages starting from the given
 * page header.
 *
 * Iterates through all pages and their allocated chunk lists to determine if
 * the specified chunk exists.
 *
 * @param page_head Pointer to the first page in the list.
 * @param target_chunk Pointer to the chunk to search for.
 * @return 1 if the chunk is found, 0 otherwise.
 */
int find_chunk_in_pages(page_header_t *page_head,
                        chunk_header_t *target_chunk) {
    void *page_end;

    while (page_head != NULL) {
        page_end = (uint8_t *)page_head + page_head->page_size;
        if ((void *)target_chunk > (void *)page_head &&
            (void *)target_chunk < page_end &&
            (find_chunk_in_list(page_head->alloc_list, target_chunk) == 1 ||
             find_chunk_in_list(page_head->free_list, target_chunk) == 1)) {
            return 1;
        }
        page_head = page_head->next;
    }
    return 0;
}

/**
 * @brief Searches for a specific chunk in a doubly linked list of chunks.
 *
 * Iterates through the linked list starting from `chunk_head` to find the
 * specified chunk.
 *
 * @param chunk_head Pointer to the first chunk in the list.
 * @param target_chunk Pointer to the chunk to search for.
 * @return 1 if the chunk is found, 0 otherwise.
 */
int find_chunk_in_list(chunk_header_t *chunk_head,
                       chunk_header_t *target_chunk) {
    while (chunk_head != NULL) {
        if (chunk_head == target_chunk) {
            return 1;
        }
        chunk_head = chunk_head->next;
    }
    return 0;
}

/**
 * @brief Searches for a free chunk within the list of pages that can hold at
 * least the requested size.
 *
 * This function iterates through all the pages starting from the given `head`
 * and searches the free list of each page for a chunk whose size is equal to or
 * larger than the requested `size`. If such a chunk is found, it is returned.
 *
 * @param head Pointer to the first page in the list to search through.
 * @param size The minimum size in bytes that the free chunk must be able to
 * accommodate.
 * @return Pointer to a suitable free chunk, or NULL if no suitable chunk is
 * found.
 */
chunk_header_t *find_free_chunk(page_header_t *head, size_t size) {
    chunk_header_t *free_chunk;

    while (head != NULL) {
        free_chunk = head->free_list;
        while (free_chunk != NULL) {
            if (free_chunk->size >= size) {
                return free_chunk;
            }
            free_chunk = free_chunk->next;
        }
        head = head->next;
    }
    return NULL;
}
