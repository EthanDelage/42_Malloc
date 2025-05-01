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