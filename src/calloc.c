#include "malloc.h"
#include "utils/printf.h"

#include <string.h>

void *calloc(size_t nmemb, size_t size) {
    void *ptr;
    size_t total_size = nmemb * size;

    printf("calloc: %zu, %zu\n", nmemb, size);
    if (nmemb != 0 && total_size / nmemb != size) {
        return NULL;
    }
    ptr = malloc(total_size);
    if (ptr == NULL) {
        return NULL;
    }
    // TODO: remove libc function
    memset(ptr, 0, total_size);
    return ptr;
}
