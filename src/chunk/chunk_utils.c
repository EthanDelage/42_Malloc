#include "chunk.h"

/**
 * @brief Splits a chunk into two if there is enough space for a second valid
 * chunk.
 *
 * This function splits the given chunk into two if its size is large enough
 * to allocate the requested `size` to the first chunk and still leave enough
 * space for a second chunk header **and at least 8 byte of usable space**.
 *
 * The original chunk is resized to `size`, and the remaining space is used to
 * create a new chunk inserted immediately after it in the doubly linked list.
 *
 * @param chunk Pointer to the chunk to be split.
 * @param size  Size to allocate for the first part of the split (not including
 * header).
 * @return Pointer to the newly created chunk (second part), or NULL if the
 * split is not possible.
 */
chunk_header_t *chunk_split(chunk_header_t *chunk, size_t size) {
    chunk_header_t *new;

    if (chunk->size < size + sizeof(chunk_header_t) + ALIGNMENT) {
        return NULL;
    }
    new = (chunk_header_t *)((uint8_t *)chunk + size + sizeof(chunk_header_t));
    new->size = chunk->size - size - sizeof(chunk_header_t);
    chunk->size = size;
    chunk_dll_insert(&chunk, new);
    return new;
}
