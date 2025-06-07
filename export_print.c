/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:57:22 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/03 12:00:09 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	reset_indexes(t_env *env)
{
	while (env)
	{
		env->index = -1;
		env = env->next;
	}
}

static t_env	*find_min_node(t_env *env)
{
	t_env	*min;

	min = NULL;
	while (env)
	{
		if (env->index == -1)
		{
			if (!min || ft_strcmp(env->key, min->key) < 0)
				min = env;
		}
		env = env->next;
	}
	return (min);
}

void	assign_sorted_indexes(t_env *env)
{
	int		index;
	t_env	*min_node;

	index = 0;
	reset_indexes(env);
	while (1)
	{
		min_node = find_min_node(env);
		if (!min_node)
			break ;
		min_node->index = index++;
	}
}

void	export_print(t_env *my_env)
{
	int		i;
	int		count;
	t_env	*tmp;

	assign_sorted_indexes(my_env);
	count = count_exported(my_env);
	i = 0;
	while (i < count)
	{
		tmp = my_env;
		while (tmp)
		{
			if (tmp->index == i)
			{
				if (tmp->value)
					printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
				else
					printf("declare -x %s\n", tmp->key);
			}
			tmp = tmp->next;
		}
		i++;
	}
}
