/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:42:44 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/03 11:47:59 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*add_space_before(char *s, int pos)
{
	char	*res;

	res = malloc(strlen(s) + 2);
	if (!res)
		return (NULL);
	strncpy(res, s, pos);
	res[pos] = ' ';
	strcpy(res + pos + 1, s + pos);
	return (res);
}

char	*add_space_after(char *s, int pos)
{
	char	*res;

	res = malloc(strlen(s) + 2);
	if (!res)
		return (NULL);
	strncpy(res, s, pos + 1);
	res[pos + 1] = ' ';
	strcpy(res + pos + 2, s + pos + 1);
	return (res);
}

int	is_special(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

int	is_double_operator(char *str, int i, int len)
{
	if ((str[i] == '>' && i < len - 1 && str[i + 1] == '>')
		|| (str[i] == '<' && i < len - 1 && str[i + 1] == '<'))
		return (1);
	return (0);
}