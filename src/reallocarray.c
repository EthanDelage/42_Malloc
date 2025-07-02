#include "malloc.h"
#include "utils/printf.h"

#include <stddef.h>

void *reallocarray(void *ptr, size_t nmemb, size_t size) {
    // printf("reallocarray(%p, %zu, %zu);", ptr, nmemb, size);
    size_t total_size = nmemb * size;

    if (nmemb != 0 && total_size / nmemb != size) {
        return NULL;
    }
    return realloc(ptr, total_size);
}
