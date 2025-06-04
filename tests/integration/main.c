#include <stdio.h>
#include <stdlib.h>

// malloc: 5
// 00007A4F68A1F050
// free: 00007A4F68A1F050
// malloc: 120
// 00007A4F68A11050
// malloc: 12
// 00007A4F68A1F050
// malloc: 792
// 00007A4F68A1F030
// malloc: 104
// 00007A4F68A110E8
// malloc: 1328
// 00007A4F68A110E8
// malloc: 208

int main() {
    free(malloc(5));
    (void) malloc(120);
    (void) malloc(12);
    (void) malloc(792);
    (void) malloc(104);
    (void) malloc(1328);
    (void) malloc(208);
    return 0;
}