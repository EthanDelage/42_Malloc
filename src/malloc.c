#include "malloc.h"

#include <stdio.h>
malloc_data_t malloc_data = {0};

void *malloc(size_t size) {
    (void) size;
    printf("malloc\n");
    return NULL;
}
