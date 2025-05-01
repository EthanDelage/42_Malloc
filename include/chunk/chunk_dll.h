#ifndef CHUNK_DLL_H
#define CHUNK_DLL_H

#include "memory/chunk_header.h"

void chunk_dll_insert_after(chunk_header_t *pos, chunk_header_t *value);
void chunk_dll_insert_before(chunk_header_t *pos, chunk_header_t *value);
void chunk_dll_insert_sorted(chunk_header_t **head, chunk_header_t *new_chunk);
void chunk_dll_remove(chunk_header_t *pos);
void chunk_dll_transfer_sorted(chunk_header_t **dest_head,
                               chunk_header_t *chunk);

#endif // CHUNK_DLL_H
