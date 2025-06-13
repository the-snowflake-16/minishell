/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_list2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:48:11 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/13 15:52:46 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*strjoin_and_free(char *s1, const char *s2)
{
	char	*new;

	new = ft_strjoin(s1, s2);
	free(s1);
	return (new);
}

char	*strjoin_and_free_char(char *s1, char c)
{
	char	temp[2];

	temp[0] = c;
	temp[1] = '\0';
	return (strjoin_and_free(s1, temp));
}

t_parser	*creat_node(char *s)
{
	t_parser	*new;

	new = malloc(sizeof(t_parser));
	if (!new)
		return (NULL);
	new->word = ft_strdup(s);
	new->type = chech_symbol(s);
	new->next = NULL;
	return (new);
}

t_parser	*append_node(t_parser **head, t_parser **current, t_parser *new)
{
	if (!new)
		return (NULL);
	if (!*head)
	{
		*head = new;
		*current = new;
	}
	else
	{
		(*current)->next = new;
		*current = new;
	}
	return (new);
}

int	first_quoter(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'')
			return (1);
		else if (s[i] == '"')
			return (2);
		i++;
	}
	return (0);
}
