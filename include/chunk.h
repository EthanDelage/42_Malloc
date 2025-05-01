#ifndef CHUNK_H
#define CHUNK_H

#include "malloc.h"

void chunk_dll_insert_after(chunk_header_t *pos, chunk_header_t *value);
void chunk_dll_insert_before(chunk_header_t *pos, chunk_header_t *value);
void chunk_dll_remove(chunk_header_t *pos);

chunk_header_t *chunk_split(chunk_header_t *chunk, size_t size);
void chunk_coalesce(chunk_header_t *chunk);
uint8_t is_adjacent_chunk(const chunk_header_t *first,
                          const chunk_header_t *second);

#endif // CHUNK_H
