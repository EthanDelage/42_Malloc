#ifndef CHUNK_HEADER_H
#define CHUNK_HEADER_H

#include <stddef.h>
#include <stdint.h>

typedef struct chunk_header_s chunk_header_t;

struct chunk_header_s {
    size_t size;
    uint8_t in_use;
    chunk_header_t *next;
    chunk_header_t *previous;
};

#endif // CHUNK_HEADER_H
