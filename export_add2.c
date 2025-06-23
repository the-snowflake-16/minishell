/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_add2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 10:16:36 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/23 10:18:54 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*find_key(t_env *my_env, char *s)
{
	t_env	*tmp;

	tmp = my_env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, s))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int	incorect_input_for_key(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

char	*split_by_equal_key(char *s)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	if (s[i] != '=' || i == 0)
		return (NULL);
	res = malloc(sizeof(char) * (i + 1));
	if (!res)
		return (NULL);
	j = -1;
	while (++j < i)
	{
		if (!ft_isalnum(s[j]))
		{
			free(res);
			return (NULL);
		}
		res[j] = s[j];
	}
	res[i] = '\0';
	return (res);
}

int	count_value_length(const char *s)
{
	int	i;
	int	len;
	int	start;

	i = 0;
	len = 0;
	start = 0;
	while (s[i])
	{
		if (s[i] == '=')
		{
			start = i + 1;
			break ;
		}
		i++;
	}
	while (s[start++])
		len++;
	return (len);
}

char	*get_value_after_equal(const char *s)
{
	int		i;
	int		j;
	char	*res;
	int		len;

	i = 0;
	j = 0;
	len = count_value_length(s);
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	while (s[i] && s[i] != '=')
		i++;
	if (s[i] == '=')
		i++;
	while (s[i])
		res[j++] = s[i++];
	res[j] = '\0';
	return (res);
}
