#include "utils/libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
    char	*ptr = (char *) s;
    size_t	index = 0;

    while (index < n)
    {
        ptr[index] = c;
        index++;
    }
    return s;
}