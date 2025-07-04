#include "utils/libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
    size_t	index = 0;

    while (index < n)
    {
        ((char *) dest)[index] = ((char *) src)[index];
        index++;
    }
    return dest;
}