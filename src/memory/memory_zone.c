#include "memory/memory_zone.h"
#include "memory/memory_utils.h"
#include "memory/page_header.h"

/**
 * @brief Calculates the total size required for a memory zone, including
 * headers and enough space for a minimum number of chunks, then aligns it to
 * the system page size.
 * @param alloc_max_size The maximum size in bytes for each allocation within
 * the zone.
 * @return The total size in bytes for the memory zone, aligned to the system
 * page size.
 */
size_t get_zone_size(size_t alloc_max_size) {
    size_t zone_min_size =
        sizeof(page_header_t) +
        (sizeof(chunk_header_t) + alloc_max_size) * ZONE_CHUNK_MIN_COUNT;
    return page_align(zone_min_size);
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
