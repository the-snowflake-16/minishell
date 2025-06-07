/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:10:23 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/03 11:52:37 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_before(char **result, int *i, int *len)
{
	char	*new_str;

	if (*i > 0 && (*result)[*i - 1] != ' ')
	{
		new_str = add_space_before(*result, *i);
		free(*result);
		*result = new_str;
		(*len)++;
		(*i)++;
		return (1);
	}
	return (0);
}

static int	process_after(char **result, int *i, int *len)
{
	char	*new_str;

	if (*i < *len - 1 && (*result)[*i + 1] != ' ')
	{
		new_str = add_space_after(*result, *i);
		free(*result);
		*result = new_str;
		(*len)++;
		return (1);
	}
	return (0);
}

static int	process_char(char **result, int *i, int *len, int checker)
{
	if (is_special((*result)[*i]) && !checker)
	{
		if (process_before(result, i, len))
			return (1);
		if (is_double_operator(*result, *i, *len))
			(*i)++;
		if (process_after(result, i, len))
			return (1);
	}
	return (0);
}

static int	process_filter_loop(char **result, int *len)
{
	int		i;
	int		checker;

	i = -1;
	checker = 0;
	while (++i < *len)
	{
		if ((*result)[i] == '\'' || (*result)[i] == '"')
			checker = 1;
		if (process_char(result, &i, len, checker))
			return (1);
	}
	return (0);
}

char	*filter(char *s)
{
	char	*result;
	int		modified;
	int		len;

	result = ft_strdup(s);
	if (!s || !result)
		return (NULL);
	modified = 1;
	while (modified)
	{
		modified = 0;
		len = strlen(result);
		if (process_filter_loop(&result, &len))
			modified = 1;
	}
	return (result);
}
