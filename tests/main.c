#include <stdio.h>
#include <stdlib.h>

int main() {
    void *p = malloc(40);
    printf("%p\n", p);
    p = realloc(p, 60);
    printf("%p\n", p);
    free(p);
    return 0;
}