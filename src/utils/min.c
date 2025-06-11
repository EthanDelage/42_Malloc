#include <stddef.h>

size_t ft_min(size_t a, size_t b) {
    if (a < b) {
        return a;
    }
    return b;
}