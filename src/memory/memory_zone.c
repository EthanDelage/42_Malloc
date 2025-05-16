#include "memory/memory_zone.h"

#include "chunk/chunk_search.h"
#include "memory/malloc_data.h"
#include "memory/memory_utils.h"
#include "memory/page_header.h"

/**
 * @brief Calculates the total size required for a memory zone based on its
 * type.
 *
 * This function computes the size of a memory zone of the given type (TINY or
 * SMALL), including the page header and a minimum number of chunk headers and
 * their associated data. The result is aligned to the system's page size.
 *
 * @param type The zone type (TINY or SMALL) for which to calculate the memory
 * size.
 * @return The total aligned size in bytes required for the memory zone.
 */
size_t get_zone_size(zone_type_t type) {
    size_t alloc_max_size = 0;

    if (type == TINY) {
        alloc_max_size = TINY_MAX_SIZE;
    } else if (type == SMALL) {
        alloc_max_size = SMALL_MAX_SIZE;
    }
    size_t zone_min_size =
        sizeof(page_header_t) +
        (sizeof(chunk_header_t) + alloc_max_size) * ZONE_CHUNK_MIN_COUNT;
    return align_page(zone_min_size);
}

zone_type_t get_zone_type(size_t size) {
    if (size <= TINY_MAX_SIZE) {
        return TINY;
    }
    if (size <= SMALL_MAX_SIZE) {
        return SMALL;
    }
    return LARGE;
}

/**
 * @brief Determines the zone type (TINY, SMALL, or LARGE) for a given chunk.
 *
 * If the chunk is NULL or not found in any known zone, returns `ZONE_INVALID`.
 *
 * @param chunk Pointer to the chunk to inspect.
 * @return The zone type (TINY, SMALL, LARGE) or ZONE_INVALID if unknown.
 */
zone_type_t get_chunk_zone_type(chunk_header_t *chunk) {
    zone_type_t type = get_zone_type(chunk->size);

    if ((type == TINY && !is_chunk_in_pages(malloc_data.tiny, chunk)) ||
        (type == SMALL && !is_chunk_in_pages(malloc_data.small, chunk)) ||
        (type == LARGE && !is_chunk_in_list(malloc_data.large, chunk))) {
        return INVALID;
    }
    return type;
}
