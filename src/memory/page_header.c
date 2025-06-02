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
    new_page->previous = current;
}

/**
 * @brief Removes a page from a singly or doubly linked list of pages.
 *
 * This function unlinks the given page from the list it belongs to by updating
 * the adjacent page links appropriately. After removal, the `next` pointer of
 * the removed page is set to NULL.
 *
 * Note: This function assumes that the page list is managed elsewhere (e.g.,
 * via a global or external head pointer) and does not update the head of the
 * list. The caller must ensure the head is updated if `pos` is the first node.
 *
 * @param pos Pointer to the page to remove. Must be part of a valid linked
 * list.
 */
void page_list_remove(page_header_t *pos) {
    page_header_t *next = pos->next;
    page_header_t *previous = pos->previous;

    if (next != NULL) {
        next->previous = previous;
        pos->next = NULL;
    }
    if (previous != NULL) {
        previous->next = next;
        pos->previous = NULL;
    }
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
