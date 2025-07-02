#include "malloc.h"
#include "utils/libft.h"
#include "utils/printf.h"

void *calloc(size_t nmemb, size_t size) {
    // printf("calloc(%zu, %zu);", nmemb, size);
    void *ptr;
    size_t total_size = nmemb * size;

    if (nmemb != 0 && total_size / nmemb != size) {
        return NULL;
    }
    ptr = malloc(total_size);
    if (ptr == NULL) {
        return NULL;
    }
    ft_memset(ptr, 0, total_size);
    return ptr;
}
