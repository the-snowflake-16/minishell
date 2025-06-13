/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 08:03:24 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/13 14:17:50 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirectionss(t_command *cmd)
{
	if (!cmd->redirects)
		return (0);
	if (cmd->redirects->is_input && redirect_input(cmd->redirects->file))
		return (-1);
	if (cmd->redirects->is_output && redirect_output(cmd->redirects->file))
		return (-1);
	if (cmd->redirects->is_append && redirect_append(cmd->redirects->file))
		return (-1);
	return (0);
}

int	execute_builtin_last(t_command *cmd, t_state *state)
{
	if (handle_redirectionss(cmd))
		return (-1);
	state->last_exit_code = handle_builtin(cmd, state);
	return (0);
}

void	setup_child_input(t_state *state)
{
	if (state->heredoc_fd != -1)
	{
		dup2(state->heredoc_fd, STDIN_FILENO);
		close(state->heredoc_fd);
	}
	else if (state->in_fd != STDIN_FILENO)
	{
		dup2(state->in_fd, STDIN_FILENO);
	}
}

void	setup_child_output(t_state *state, int has_next)
{
	if (has_next)
	{
		dup2(state->fd[1], STDOUT_FILENO);
		close(state->fd[1]);
		close(state->fd[0]);
	}
}

void	child_process(t_command *cmd, t_state *state, int has_next)
{
	setup_child_input(state);
	setup_child_output(state, has_next);
	if (handle_redirectionss(cmd))
		exit(1);
	if (state->in_fd != STDIN_FILENO)
		close(state->in_fd);
	cmp_input(cmd, state);
	free_comand(cmd);
	free_env(state->env);
	exit(state->last_exit_code);
}
