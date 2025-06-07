/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 08:23:39 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/07 09:35:13 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_list(t_parser *head)
{
	t_parser	*current;
	t_parser	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		free(current->word);
		free(current);
		current = next;
	}
}

int	chech_symbol(char *s)
{
	if (!ft_strcmp(s, "<<"))
		return (TOKEN_REDIR_HEREDOC);
	if (!ft_strcmp(s, ">>"))
		return (TOKEN_REDIR_APPEND);
	if (!ft_strcmp(s, "<"))
		return (TOKEN_REDIR_IN);
	if (!ft_strcmp(s, ">"))
		return (TOKEN_REDIR_OUT);
	if (!ft_strcmp(s, "|"))
		return (TOKEN_PIPE);
	return (TOKEN_WORD);
}

static int	count_quotes(const char *s, char q)
{
	int	i;
	int	count;

	i = 1;
	count = 0;
	while (s[i])
	{
		if (s[i] == q)
			count++;
		i++;
	}
	return (count);
}

static void	copy_without_quotes(const char *s, char q, char *res)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == q && s[i + 1] != q)
			i++;
		else if (s[i] == q && s[i + 1] == q)
			i += 2;
		else
		{
			res[j] = s[i];
			i++;
			j++;
		}
	}
	res[j] = '\0';
}

char	*delete_quotes(const char *s, char q)
{
	int		len;
	int		count;
	char	*res;

	len = ft_strlen(s);
	count = count_quotes(s, q);
	res = malloc(len - count + 1);
	if (!res)
		return (NULL);
	copy_without_quotes(s, q, res);
	res[len - count] = '\0';
	return (res);
}
