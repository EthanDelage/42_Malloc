#include <stdio.h>

#include "malloc.h"

void *realloc(void *ptr, size_t size) {
    (void) size;
    printf("realloc\n");
    return ptr;
}
