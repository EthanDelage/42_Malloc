#ifndef CHUNK_UTILS_H
#define CHUNK_UTILS_H

#include "memory/chunk_header.h"

#define ALIGNMENT 8

chunk_header_t *chunk_split(chunk_header_t *chunk, size_t size);
void chunk_coalesce(chunk_header_t *chunk);
uint8_t is_adjacent_chunk(const chunk_header_t *first,
                          const chunk_header_t *second);

#endif // CHUNK_UTILS_H
