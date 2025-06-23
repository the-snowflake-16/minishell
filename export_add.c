/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_add.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 08:35:01 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/23 10:17:10 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*delete_quotes_export(char *s)
{
	int		len;
	int		i;
	int		j;
	char	*res;

	len = ft_strlen(s);
	res = malloc(sizeof(char) * (len - 1));
	if (!res)
		return (NULL);
	i = 0;
	j = 1;
	while (j < len - 1)
		res[i++] = s[j++];
	res[i] = '\0';
	return (res);
}

int	parse_export_key(t_command *cmd, char **key)
{
	if (!incorect_input_for_key(cmd->args[1]))
	{
		*key = split_by_equal_key(cmd->args[1]);
		if (!(*key))
		{
			printf("not a valid identifier\n");
			cmd->exit_code = 1;
			return (0);
		}
		return (1);
	}
	printf("not a valid identifier\n");
	cmd->exit_code = 1;
	return (0);
}

char	*parse_export_value(char *arg)
{
	char	*val;
	char	*tmp;

	val = get_value_after_equal(arg);
	if (!val)
		return (NULL);
	if ((val[0] == '\'' || val[0] == '"')
		&& val[0] == val[ft_strlen(val) - 1])
	{
		tmp = delete_quotes_export(val);
		free(val);
		val = tmp;
	}
	return (val);
}

void	update_env_value(t_env *env, char *key, char *val)
{
	t_env	*tmp;

	tmp = find_key(env, key);
	while (env)
	{
		if (!ft_strcmp(env->key, tmp->key))
		{
			free(env->value);
			if (val)
				env->value = ft_strdup(val);
			else
				env->value = ft_strdup("");
		}
		env = env->next;
	}
}

void	export_add(t_env *env, t_command *cmd)
{
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	if (!parse_export_key(cmd, &key))
		return ;
	if (check_str_for_export_add_after_equal(cmd->args[1]))
		value = parse_export_value(cmd->args[1]);
	update_env_value(env, key, value);
	free(key);
	if (value)
		free(value);
}
