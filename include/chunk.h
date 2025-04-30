#ifndef CHUNK_H
#define CHUNK_H

#include "malloc.h"

void chunk_dll_insert(chunk_header_t **pos, chunk_header_t *value);
void chunk_dll_remove(chunk_header_t **pos);

#endif // CHUNK_H
