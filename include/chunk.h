#ifndef CHUNK_H
#define CHUNK_H

#include "malloc.h"

void chunk_dll_insert(chunk_header_t **pos, chunk_header_t *value);
void chunk_dll_remove(chunk_header_t **pos);

chunk_header_t *chunk_split(chunk_header_t *chunk, size_t size);
void chunk_coalesce(chunk_header_t *chunk);

#endif // CHUNK_H
