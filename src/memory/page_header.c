#include "memory/page_header.h"

#include "chunk/chunk_search.h"

void page_list_insert(page_header_t **head, page_header_t *new_page) {
    page_header_t *current = *head;

    if (current == NULL) {
        *head = new_page;
        return;
    }
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_page;
}

/**
 * @brief Searches a list of pages to find one with a free chunk large enough to
 * satisfy the given size.
 *
 * This function iterates through the given list of pages and returns the first
 * page containing a free chunk that is large enough to fulfill the requested
 * `min_size`.
 *
 * @param head Pointer to the head of the list of pages.
 * @param min_size The minimum size in bytes required for the allocation.
 * @return Pointer to a page with a suitable chunk, or NULL if none is found.
 */
page_header_t *find_available_page(page_header_t *head, size_t min_size) {
    while (head != NULL) {
        if (head->max_free_chunk_size >= min_size) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

/**
 * @brief Finds the page that contains the specified chunk.
 *
 * Iterates through the list of pages starting from `page_head` and checks
 * whether the given chunk belongs to any of the pages.
 *
 * @param chunk Pointer to the chunk whose containing page is being searched.
 * @param page_head Pointer to the first page in the list.
 * @return Pointer to the page that contains the chunk, or NULL if not found.
 */
page_header_t *find_chunk_page(chunk_header_t *chunk,
                               page_header_t *page_head) {
    void *page_end;

    while (page_head != NULL) {
        page_end = (uint8_t *)page_head + page_head->size;
        if ((void *)chunk > (void *)page_head && (void *)chunk < page_end) {
            return page_head;
        }
        page_head = page_head->next;
    }
    return NULL;
}

void update_max_free_chunk_size(page_header_t *page) {
    chunk_header_t *current = page->free_list;
    size_t max_free_chunk_size = 0;

    while (current != NULL) {
        if (current->size >= max_free_chunk_size) {
            max_free_chunk_size = current->size;
        }
        current = current->next;
    }
    page->max_free_chunk_size = max_free_chunk_size;
}
