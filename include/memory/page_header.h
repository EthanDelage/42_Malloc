#ifndef PAGE_HEADER_H
#define PAGE_HEADER_H

#include "memory/chunk_header.h"

typedef struct page_header_s page_header_t;

struct page_header_s {
    chunk_header_t *alloc_list;
    chunk_header_t *free_list;
    size_t size;
    page_header_t *next;
};

#endif // PAGE_HEADER_H
