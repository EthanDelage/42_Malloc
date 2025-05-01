#ifndef MEMORY_ZONE_H
#define MEMORY_ZONE_H

#include <stddef.h>

#define TINY_MAX_SIZE 64
#define SMALL_MAX_SIZE 512
#define ZONE_CHUNK_MIN_COUNT 100

typedef enum zone_type_e zone_type_t;

enum zone_type_e {
    TINY,
    SMALL,
    LARGE,
};

size_t get_zone_size(size_t alloc_max_size);
zone_type_t get_zone_type(size_t size);

#endif // MEMORY_ZONE_H
