#ifndef CHUNK_UTILS_H
#define CHUNK_UTILS_H

#include "memory/chunk_header.h"

#define ALIGNMENT sizeof(void *)

chunk_header_t *chunk_split(chunk_header_t *chunk, size_t size);
void chunk_coalesce(chunk_header_t *chunk);
chunk_header_t *chunk_coalesce_surrounding(chunk_header_t *chunk);
uint8_t is_adjacent_chunk(const chunk_header_t *first,
                          const chunk_header_t *second);
void *get_chunk_data(const chunk_header_t *chunk);
chunk_header_t *get_chunk_from_data(const void *ptr);

#endif // CHUNK_UTILS_H
