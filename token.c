/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:07:57 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/13 15:35:10 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*tokenize(char *s)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->token_arr = ft_splitt(s);
	if (!token->token_arr)
	{
		free(token);
		return (NULL);
	}
	return (token);
}

void	free_token(t_token *token)
{
	int	i;

	i = 0;
	while (token->token_arr[i])
	{
		free(token->token_arr[i]);
		i++;
	}
	free(token->token_arr);
	token->token_arr = NULL;
	free(token);
}

int	skip_spaces_and_quotes(char *s, int *i, int *in_single, int *in_double)
{
	while (s[*i] == ' ' && !(*in_single) && !(*in_double))
		(*i)++;
	if (!s[*i])
		return (0);
	return (1);
}

int	find_word_end(char *s, int i, int *in_single, int *in_double)
{
	while (s[i])
	{
		if (s[i] == '\'' && !(*in_double))
			*in_single = !(*in_single);
		else if (s[i] == '"' && !(*in_single))
			*in_double = !(*in_double);
		else if (s[i] == ' ' && !(*in_single) && !(*in_double))
			break ;
		i++;
	}
	return (i);
}

int	split_loop(char *s, char **rs, int *i, int word_index)
{
	int	start;
	int	end;
	int	in_single;
	int	in_double;

	in_single = 0;
	in_double = 0;
	if (!skip_spaces_and_quotes(s, i, &in_single, &in_double))
		return (word_index);
	start = *i;
	end = find_word_end(s, *i, &in_single, &in_double);
	rs[word_index] = extract_word(s, start, end);
	if (!rs[word_index])
	{
		free_words(rs, word_index);
		return (-1);
	}
	*i = end;
	return (word_index + 1);
}
