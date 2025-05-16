#ifndef MEMORY_ZONE_H
#define MEMORY_ZONE_H

#include "memory/chunk_header.h"

#include <stddef.h>

#define TINY_MAX_SIZE 64
#define SMALL_MAX_SIZE 512
#define ZONE_CHUNK_MIN_COUNT 100

typedef enum zone_type_e zone_type_t;

enum zone_type_e {
    INVALID = -1,
    TINY,
    SMALL,
    LARGE,
};

size_t get_zone_size(zone_type_t type);
zone_type_t get_zone_type(size_t size);
zone_type_t get_chunk_zone_type(chunk_header_t *chunk);

#endif // MEMORY_ZONE_H
