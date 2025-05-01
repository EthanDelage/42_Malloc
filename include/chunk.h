#ifndef CHUNK_H
#define CHUNK_H

#include "memory/chunk_header.h"

#define ALIGNMENT 8

void chunk_dll_insert_after(chunk_header_t *pos, chunk_header_t *value);
void chunk_dll_insert_before(chunk_header_t *pos, chunk_header_t *value);
void chunk_dll_insert_sorted(chunk_header_t **head, chunk_header_t *new_chunk);
void chunk_dll_remove(chunk_header_t *pos);
void chunk_dll_transfer_sorted(chunk_header_t **dest_head,
                               chunk_header_t *chunk);

chunk_header_t *chunk_split(chunk_header_t *chunk, size_t size);
void chunk_coalesce(chunk_header_t *chunk);
uint8_t is_adjacent_chunk(const chunk_header_t *first,
                          const chunk_header_t *second);

#endif // CHUNK_H
