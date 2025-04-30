#ifndef MALLOC_H
#define MALLOC_H

#include <stdlib.h>

#define TINY_MAX_SIZE 64
#define SMALL_MAX_SIZE 512
#define ZONE_CHUNK_MIN_COUNT 100

typedef struct malloc_data_s malloc_data_t;
typedef struct page_header_s page_header_t;
typedef struct chunk_header_s chunk_header_t;

struct malloc_data_s {
    page_header_t *tiny;
    page_header_t *small;
    chunk_header_t *large;
};

struct page_header_s {
    chunk_header_t *alloc_list;
    chunk_header_t *free_list;
    page_header_t *next;
};

struct chunk_header_s {
    size_t size;
    u_int8_t in_use;
    chunk_header_t *next;
    chunk_header_t *previous;
};

extern malloc_data_t malloc_data;

void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);

#endif //MALLOC_H
