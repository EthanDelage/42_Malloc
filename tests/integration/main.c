#include <stdio.h>
#include <stdlib.h>

// malloc: 472 -> 00007F989654C050
// malloc: 120 -> 00007F989654C248
// malloc: 1024 -> 00007F989659E020
// free: 00007F989654C248
// free: 00007F989659E020
// free: 00007F989654C050

int main() {
    void *p1 = malloc(472);
    void *p2 = malloc(120);
    void *p3 = malloc(1024);
    free(realloc(NULL, 10));
    free(p2);
    free(p3);
    free(p1);
    return 0;
}