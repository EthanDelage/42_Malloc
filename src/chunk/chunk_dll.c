#include "chunk.h"

/**
 * @brief Inserts a chunk into the doubly linked list after the given position.
 *
 * This function inserts the `value` chunk immediately after the chunk pointed
 * to by `pos` in the doubly linked list. It properly updates the `next` and
 * `previous` pointers of the involved chunks to maintain list integrity.
 *
 * @param pos Pointer to the chunk after which the new chunk will be inserted.
 * @param value Pointer to the chunk to insert.
 */
void chunk_dll_insert_after(chunk_header_t *pos, chunk_header_t *value) {
    chunk_header_t *next = pos->next;

    value->previous = pos;
    value->next = next;
    pos->next = value;
    if (next != NULL) {
        next->previous = value;
    }
}

/**
 * @brief Inserts a chunk into the doubly linked list before the given position.
 *
 * This function inserts the `value` chunk immediately before the chunk pointed
 * to by `pos` in the doubly linked list. It properly updates the `next` and
 * `previous` pointers of the involved chunks to maintain list integrity.
 *
 * If the inserted chunk becomes the new head of the list, the caller is
 * responsible for updating the head pointer.
 *
 * @param pos Pointer to the chunk before which the new chunk will be inserted.
 * @param value Pointer to the chunk to insert.
 */
void chunk_dll_insert_before(chunk_header_t *pos, chunk_header_t *value) {
    chunk_header_t *previous = pos->previous;

    value->previous = previous;
    value->next = pos;
    pos->previous = value;
    if (previous != NULL) {
        previous->next = value;
    }
}

/**
 * @brief Inserts a chunk into a doubly linked list sorted by memory address.
 *
 * This function inserts the given chunk into the doubly linked list pointed to
 * by `head`, maintaining the list sorted in ascending order of chunk memory
 * addresses. The sort order is based on the address of each chunk in memory,
 * ensuring predictable traversal and efficient coalescing in memory management.
 *
 * @param head Pointer to the head pointer of the doubly linked list.
 * @param new_chunk Pointer to the chunk to insert into the list.
 */
void chunk_dll_insert_sorted(chunk_header_t **head, chunk_header_t *new_chunk) {
    chunk_header_t *current = *head;

    if (current == NULL) {
        *head = new_chunk;
        return;
    }
    if (current > new_chunk) {
        chunk_dll_insert_before(current, new_chunk);
        *head = new_chunk;
        return;
    }
    while (current->next != NULL && current->next < new_chunk) {
        current = current->next;
    }
    chunk_dll_insert_after(current, new_chunk);
}

/**
 * @brief Removes a chunk from a doubly linked list of memory chunks.
 *
 * This function unlinks the given chunk from the doubly linked list it belongs
 * to. It properly updates the `previous` and `next` pointers of adjacent chunks
 * to maintain list consistency. The removed chunk's `next` and `previous`
 * pointers are set to NULL.
 *
 * @param pos Pointer to the chunk to remove. Must be a valid, non-null node in
 * a linked list.
 */
void chunk_dll_remove(chunk_header_t *pos) {
    chunk_header_t *previous = pos->previous;
    chunk_header_t *next = pos->next;

    if (next != NULL) {
        next->previous = previous;
        pos->next = NULL;
    }
    if (previous != NULL) {
        previous->next = next;
        pos->previous = NULL;
    }
}