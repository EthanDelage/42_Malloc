#include "chunk/chunk_utils.h"
#include "chunk/chunk_dll.h"
#include "memory/memory_utils.h"

/**
 * @brief Splits a chunk into two if there is enough space for a second valid
 * chunk.
 *
 * This function splits the given chunk into two if its size is large enough
 * to allocate the requested `size` (after alignment using `align_size`) to the
 * first chunk and still leave enough space for a second chunk header and at
 * least 8 bytes of usable space.
 *
 * The original chunk is resized to the aligned `size`, and the remaining space
 * is used to create a new chunk inserted immediately after it in the doubly
 * linked list.
 *
 * @param chunk Pointer to the chunk to be split.
 * @param size  Requested size for the first part of the split (not including
 * header). This value will be aligned internally.
 * @return Pointer to the newly created chunk (second part), or NULL if the
 * split is not possible.
 */
chunk_header_t *chunk_split(chunk_header_t *chunk, size_t size) {
    chunk_header_t *new;

    size = align_size(size);
    if (chunk->size < size + sizeof(chunk_header_t) + ALIGNMENT) {
        return NULL;
    }
    new = (chunk_header_t *)((uint8_t *)chunk + size + sizeof(chunk_header_t));
    new->size = chunk->size - size - sizeof(chunk_header_t);
    new->in_use = chunk->in_use;
    chunk->size = size;
    chunk_dll_insert_after(chunk, new);
    return new;
}

/**
 * @brief Coalesces the given chunk with its next adjacent chunk.
 *
 * This function merges the current chunk and its next contiguous chunk into a
 * single larger free chunk. It assumes that both chunks are free and that the
 * next chunk exists and is adjacent in memory.
 *
 * After coalescing, the size of the current chunk is increased to include the
 * size of the next chunk and its header, and the doubly-linked list is updated
 * accordingly.
 *
 * @param chunk The starting chunk to coalesce with its next neighbor.
 */
void chunk_coalesce(chunk_header_t *chunk) {
    chunk_header_t *next = chunk->next;

    chunk->next = next->next;
    if (next->next != NULL) {
        next->next->previous = chunk;
    }
    chunk->size += sizeof(chunk_header_t) + next->size;
}

/**
 * @brief Coalesces a chunk with adjacent free chunks (both previous and next)
 * if contiguous in memory.
 *
 * This function checks whether the previous and/or next chunks are free and
 * directly adjacent to the given chunk. If so, it merges them into a single
 * larger chunk. It uses `chunk_coalesce()` internally for merging with the next
 * chunk.
 *
 * @param chunk Pointer to the chunk to coalesce.
 * @return Pointer to the resulting coalesced chunk (which may differ from the
 * input chunk).
 */
chunk_header_t *chunk_coalesce_surrounding(chunk_header_t *chunk) {
    chunk_header_t *res_chunk = chunk;

    if (chunk->next && is_adjacent_chunk(chunk, chunk->next) != 0) {
        chunk_coalesce(chunk);
    }
    if (chunk->previous && is_adjacent_chunk(chunk->previous, chunk) != 0) {
        res_chunk = chunk->previous;
        chunk_coalesce(chunk->previous);
    }
    return res_chunk;
}

/**
 * @brief Checks whether two chunks are adjacent in memory.
 *
 * This function determines if the second chunk directly follows the first
 * in memory, taking into account the size of the first chunk and its header.
 * It is typically used to check whether two free chunks can be coalesced
 * into a single larger chunk.
 *
 * @param first Pointer to the first chunk in memory.
 * @param second Pointer to the second chunk in memory, which will be checked
 *        if it directly follows the first chunk.
 * @return 1 if the second chunk is immediately after the first chunk in memory,
 *         0 otherwise.
 */
uint8_t is_adjacent_chunk(const chunk_header_t *first,
                          const chunk_header_t *second) {
    return (char *)first + sizeof(chunk_header_t) + first->size ==
           (char *)second;
}

/**
 * @brief Returns a pointer to the usable memory portion of a chunk.
 *
 * This function computes the memory address immediately following the chunk
 * header, which corresponds to the start of the memory block available to the
 * user.
 *
 * @param chunk Pointer to the chunk header.
 * @return Pointer to the usable memory block managed by the chunk.
 */
void *get_chunk_data(const chunk_header_t *chunk) {
    return (uint8_t *)chunk + sizeof(chunk_header_t);
}

/**
 * @brief Retrieves the chunk header from a user-accessible memory pointer.
 *
 * This function computes the address of the `chunk_header_t` by subtracting the
 * size of the header from the given user pointer.
 *
 * @param ptr Pointer to the user-accessible memory block.
 * @return Pointer to the corresponding chunk header.
 */
chunk_header_t *get_chunk_from_data(const void *ptr) {
    return (chunk_header_t *)((uint8_t *)ptr - sizeof(chunk_header_t));
}
