#include <stdio.h>

#include "malloc.h"

void *malloc(size_t size) {
    (void) size;
    printf("malloc\n");
    return NULL;
}
