#include "malloc.h"

#include <errno.h>
#include <stddef.h>

void *reallocarray(void *ptr, size_t nmemb, size_t size) {
    size_t total_size = nmemb * size;

    if (nmemb != 0 && total_size / nmemb != size) {
        free(ptr);
        errno = ENOMEM;
        return NULL;
    }
    return realloc(ptr, total_size);
}
