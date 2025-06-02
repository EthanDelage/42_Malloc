#ifndef PAGE_HEADER_H
#define PAGE_HEADER_H

#include "memory/chunk_header.h"

typedef struct page_header_s page_header_t;

struct page_header_s {
    chunk_header_t *alloc_list;
    chunk_header_t *free_list;
    size_t max_free_chunk_size;
    size_t size;
    page_header_t *next;
    page_header_t *previous;
};

void page_list_insert(page_header_t **head, page_header_t *new_page);
void page_list_remove(page_header_t *pos);
page_header_t *find_available_page(page_header_t *head, size_t min_size);
page_header_t *find_chunk_page(chunk_header_t *chunk, page_header_t *page_head);
void update_max_free_chunk_size(page_header_t *page);

#endif // PAGE_HEADER_H
