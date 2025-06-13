/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:03:01 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/13 15:36:22 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_non_content(const char *s, int i, char *quote)
{
	while (s[i] == ' ')
		i++;
	if (s[i] == '\'' || s[i] == '"')
	{
		*quote = s[i];
		i++;
		while (s[i] && s[i] != *quote)
			i++;
		if (s[i] == *quote)
			i++;
	}
	else
		*quote = 0;
	return (i);
}

int	count_words(char *s)
{
	int		i;
	int		count;
	char	quote;

	i = 0;
	count = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		i = skip_non_content(s, i, &quote);
		if (quote)
			count++;
		else if (s[i] && s[i] != ' ')
		{
			count++;
			while (s[i] && s[i] != ' ')
				i++;
		}
	}
	return (count);
}

char	*extract_word(char *s, int start, int end)
{
	char	*word;
	int		i;

	word = malloc(end - start + 1);
	if (!word)
		return (NULL);
	i = 0;
	while (start < end)
		word[i++] = s[start++];
	word[i] = '\0';
	return (word);
}

void	free_words(char **rs, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free(rs[i]);
		i++;
	}
	free(rs);
}

char	**ft_splitt(char *s)
{
	char	**rs;
	int		i;
	int		word_index;
	int		res;

	i = 0;
	word_index = 0;
	rs = malloc(sizeof(char *) * (count_words(s) + 1));
	if (!rs)
		return (NULL);
	while (s[i])
	{
		res = split_loop(s, rs, &i, word_index);
		if (res == -1)
			return (NULL);
		word_index = res;
	}
	rs[word_index] = NULL;
	return (rs);
}

// Debug function only – not norminette compliant
// void	check_arr_of_token(t_token *token)
// {
// 	int	i;

// 	i = 0;
// 	while (token->token_arr[i])
// 	{
// 		printf("Token %d: %s\n", i, token->token_arr[i]);
// 		i++;
// 	}
// 	printf("Word count: %d\n", token->count_tokens);
// }
