/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 08:08:55 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/07 08:08:56 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_str_for_export_add_after_equal(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '=' && s[i + 1])
			return (1);
		i++;
	}
	return (0);
}

int	count_exported(t_env *my_env)
{
	int	count;

	count = 0;
	while (my_env != NULL)
	{
		count++;
		my_env = my_env->next;
	}
	return (count);
}

int	compare_env(const void *a, const void *b)
{
	t_env	*env1;
	t_env	*env2;

	env1 = *(t_env **)a;
	env2 = *(t_env **)b;
	return (ft_strcmp(env1->key, env2->key));
}
