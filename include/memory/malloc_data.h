#ifndef MALLOC_DATA_H
#define MALLOC_DATA_H

#include "memory/chunk_header.h"
#include "memory/page_header.h"

typedef struct malloc_data_s malloc_data_t;

struct malloc_data_s {
    page_header_t *tiny;
    page_header_t *small;
    chunk_header_t *large;
};

extern malloc_data_t malloc_data;

#endif // MALLOC_DATA_H
