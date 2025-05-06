#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			index;
	unsigned char	*array;
	unsigned char	*string;

	array = (unsigned char *)s1;
	string = (unsigned char *)s2;
	index = 0;
	while (((array[index]) || (string[index])))
	{
		if (array[index] != string[index])
			return ((array[index] + '0') - (string[index] + '0'));
		index++;
	}
	return (0);
}
