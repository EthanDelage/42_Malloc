#ifndef CHUNK_SEARCH_H
#define CHUNK_SEARCH_H

#include "memory/page_header.h"

int is_chunk_in_pages(page_header_t *page_head, chunk_header_t *target_chunk);
int is_chunk_in_list(chunk_header_t *chunk_head, chunk_header_t *target_chunk);
chunk_header_t *find_free_chunk(page_header_t *page, size_t size);

#endif // CHUNK_SEARCH_H
