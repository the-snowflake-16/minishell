#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*add_space_before(char *s, int pos)
{
	char	*res;

	res = malloc(strlen(s) + 2);
	strncpy(res, s, pos);
	res[pos] = ' ';
	strcpy(res + pos + 1, s + pos);
	return (res);
}

char	*add_space_after(char *s, int pos)
{
	char	*res;

	res = malloc(strlen(s) + 2);
	strncpy(res, s, pos + 1);
	res[pos + 1] = ' ';
	strcpy(res + pos + 2, s + pos + 1);
	return (res);
}

char	*filter(char *s)
{
	char	*result;
	int		modified;
	int		len;
	char	*new_str;
	int checker = 0;
	if (!s)
		return (NULL);
	result = strdup(s);
	modified = 1;
	while (modified)
	{
		modified = 0;
		len = strlen(result);
		for (int i = 0; i < len; i++)
		{
			if (result[i] == '\'' || result[i] == '"')
				checker = 1;
			if ((result[i] == '|' || result[i] == '>' || result[i] == '<') && !checker)
			{
				if (i > 0 && result[i - 1] != ' ')
				{
					new_str = add_space_before(result, i);
					free(result);
					result = new_str;
					modified = 1;
					len++;
					i++;
					break ;
				}
				if(i < len -1 && result[i] == '>' && result[i+1] == '>')
					i++;
				if(i < len -1 && result[i] == '<' && result[i+1] == '<')
					i++;
				if (i < len - 1 && result[i + 1] != ' ')
				{
					new_str = add_space_after(result, i);
					free(result);
					result = new_str;
					modified = 1;
					len++;
					break ;
				}
			}
		}
	}
	return (result);
}

// int	main(void)
// {
// 	char *s = "cat|hfg|fhf >> |fjdj |dhf";
// 	char *ss = filter(s);
// 	printf("Original: %s\n", s);
// 	printf("Filtered: %s\n", ss);
// 	free(ss);
// 	return (0);
// }