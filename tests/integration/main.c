#include "malloc.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    void *p = malloc(80);
    // printf("%p\n", p);
    p = realloc(p, 180);
    show_alloc_mem();
    // printf("%p\n", p);
    // free(p);
    // free(p);
    return 0;
}