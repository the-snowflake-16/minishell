#include "libft.h"
#include <string.h>
#include <stdio.h>

char	*ft_strndup(const char *s, size_t n)
{
	char *dup;
	size_t len = ft_strlen(s);

	if (n < len)
		len = n;

	dup = malloc(len + 1);
	if (!dup)
		return (NULL);

	ft_memcpy(dup, s, len);
	dup[len] = '\0';
	return (dup);
}

// int main()
// {
// 	char *s = "hello world";
// 	char *ss = ft_strndup(s, 5);
// 	printf("%s", ss);
// 	free(ss);
// }