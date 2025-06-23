/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 08:33:19 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/23 10:06:55 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env != NULL)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	ft_split_equal(char *s, t_env *env)
{
	int	i;

	i = 0;
	while (s[i] != '=' && s[i] != '\0')
		i++;
	env->key = ft_strndup(s, i);
	if (s[i] == '=')
		env->value = ft_strdup(s + i + 1);
	else
		env->value = ft_strdup("");
}

t_env	*add_left_right(char *s)
{
	t_env	*new_env;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (NULL);
	ft_split_equal(s, new_env);
	new_env->exported = 1;
	new_env->next = NULL;
	return (new_env);
}

int	append_env_node(t_env **head, t_env **current, t_env *new_node)
{
	if (!*head)
	{
		*head = new_node;
		*current = *head;
	}
	else
	{
		(*current)->next = new_node;
		*current = (*current)->next;
	}
	return (0);
}

t_env	*init_env(char **s)
{
	t_env	*head;
	t_env	*current;
	t_env	*new_node;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while (s[i])
	{
		new_node = add_left_right(s[i]);
		if (!new_node)
		{
			free_env(head);
			return (NULL);
		}
		append_env_node(&head, &current, new_node);
		i++;
	}
	return (head);
}
