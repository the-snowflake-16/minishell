/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:31:03 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/23 10:08:07 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_exit(t_command *command, t_state *state)
{
	int				argc;
	long long		num;
	unsigned char	exit_code;

	argc = count(command->args);
	if (argc == 1)
		clean_and_exit(command, state, state->last_exit_code);
	if (!is_numeric(command->args[1]))
		exit_with_numeric_error(command, state);
	if (argc > 2)
		return (exit_with_too_many_args(state));
	num = strtoll(command->args[1], NULL, 10);
	exit_code = (unsigned char)(num % 256);
	clean_and_exit(command, state, exit_code);
	return (0);
}

int	ft_pwd(void)
{
	char	path[4096];

	if (getcwd(path, sizeof(path)) != NULL)
		printf("%s\n", path);
	else
	{
		perror("pwd");
		return (1);
	}
	return (0);
}

int	echo(t_command *command)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (!command || !command->args)
		return (1);
	if (command->args[1] && !ft_strcmp(command->args[1], "-n"))
	{
		newline = 0;
		i = 2;
	}
	while (command->args[i])
	{
		printf("%s", command->args[i]);
		if (command->args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	is_builtin(const char *cmd)
{
	return (!ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env"));
}
