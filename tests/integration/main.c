#include "malloc.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    void *p = malloc(40);
    printf("%p\n", p);
    p = realloc(p, 60);
    printf("%p\n", p);
    show_alloc_mem_hex(p);
    show_alloc_mem();
    free(p);
    return 0;
}