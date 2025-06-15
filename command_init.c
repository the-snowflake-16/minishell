/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:44:02 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/15 16:50:48 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_builtin(t_command *command, t_state *state)
{
	int	exit_code;

	exit_code = 0;
	if (!ft_strcmp(command->command, "exit"))
		exit_code = handle_exit(command, state);
	else if (!ft_strcmp(command->command, "pwd"))
		exit_code = ft_pwd();
	else if (!ft_strcmp(command->command, "cd"))
		exit_code = change_cwd(command->args);
	else if (!ft_strcmp(command->command, "export") && count(command->args) > 1)
		exit_code = export_create(state->env, command);
	else if (!ft_strcmp(command->command, "export"))
		export_print(state->env);
	else if (!ft_strcmp(command->command, "env"))
		print_env(state->env);
	else if (!ft_strcmp(command->command, "unset"))
	{
		if (!command->args[1])
			exit_code = 0;
		else
			exit_code = unset(&state->env, command->args[1]);
	}
	else if (!ft_strcmp(command->command, "echo"))
		exit_code = echo(command);
	return (exit_code);
}

void	cmp_input(t_command *command, t_state *state)
{
	if (command->command && is_builtin(command->command))
		state->last_exit_code = handle_builtin(command, state);
	else if (command->command)
		start_execve(command->args, state);
}

int	validate_redirections(t_parser *parser)
{
	t_parser	*tmp;

	tmp = parser;
	while (tmp)
	{
		if (tmp->type == TOKEN_REDIR_IN || tmp->type == TOKEN_REDIR_OUT
			|| tmp->type == TOKEN_REDIR_HEREDOC
			|| tmp->type == TOKEN_REDIR_APPEND)
		{
			if (!tmp->next || tmp->next->type != TOKEN_WORD)
			{
				printf("syntax error: redirection missing filename\n");
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
