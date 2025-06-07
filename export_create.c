/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:05:24 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/03 11:05:25 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_key_in_env(t_env *my_env, char *s)
{
	t_env	*tmp;

	tmp = my_env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, s))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

bool	is_valid_export_key(const char *s)
{
	int	i;

	i = 0;
	if (!s || !s[0])
		return (false);
	if (!ft_isalpha(s[0]) && s[0] != '_')
		return (false);
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static void	add_new_env_var(t_env *my_env, char *key, t_command *command)
{
	t_env	*tmp;
	t_env	*current;

	tmp = malloc(sizeof(t_env));
	if (!tmp)
		return ;
	tmp->key = ft_strdup(key);
	tmp->value = ft_strdup("");
	tmp->next = NULL;
	current = my_env;
	while (current->next)
		current = current->next;
	current->next = tmp;
	export_add(my_env, command);
}

int	export_create(t_env *my_env, t_command *command)
{
	char	*key;

	if (!is_valid_export_key(command->args[1]))
	{
		fprintf(stderr, "%s: not a valid identifier\n", command->args[1]);
		return (1);
	}
	if (!incorect_input_for_key(command->args[1]))
	{
		key = split_by_equal_key(command->args[1]);
		if (!key)
		{
			fprintf(stderr, "%s: not a valid identifier\n", command->args[1]);
			return (1);
		}
		if (check_key_in_env(my_env, key))
			export_add(my_env, command);
		else if (!check_key_in_env(my_env, command->args[1]))
			add_new_env_var(my_env, key, command);
		free(key);
	}
	return (0);
}

int	unset(t_env **my_env, char *s)
{
	t_env	*current;
	t_env	*prev;

	current = *my_env;
	prev = NULL;
	while (current)
	{
		if (!ft_strcmp(current->key, s))
		{
			if (prev)
				prev->next = current->next;
			else
				*my_env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}
