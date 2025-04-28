#include <stdio.h>

#include "malloc.h"

void free(void *ptr) {
    (void) ptr;
    printf("free\n");
}