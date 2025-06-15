/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:36:16 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/15 16:50:27 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_with_numeric_error(t_command *cmd, t_state *state)
{
	fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
		cmd->args[1]);
	free_comand(cmd);
	free_env(state->env);
	exit(2);
}

int	exit_with_too_many_args(t_state *state)
{
	fprintf(stderr, "minishell: exit: too many arguments\n");
	state->last_exit_code = 1;
	return (1);
}

void	clean_and_exit(t_command *cmd, t_state *state, int code)
{
	free_comand(cmd);
	free_env(state->env);
	exit(code);
}

int	count(char **ss)
{
	int	i;

	i = 0;
	while (ss[i])
		i++;
	return (i);
}

int	is_numeric(const char *str)
{
	if (!str)
		return (0);
	if (*str == '+' || *str == '-')
		str++;
	if (!*str)
		return (0);
	while (*str)
	{
		if (!isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}
