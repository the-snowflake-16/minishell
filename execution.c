/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:53:25 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/13 11:32:28 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parent_process_cleanup(t_state *state, int has_next)
{
	if (state->heredoc_fd != -1)
		close(state->heredoc_fd);
	if (state->in_fd != STDIN_FILENO)
		close(state->in_fd);
	if (has_next)
	{
		close(state->fd[1]);
		state->in_fd = state->fd[0];
	}
}

int	fork_and_execute(t_command *cmd, t_state *state,
					int has_next, pid_t *pids)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	else if (pid == 0)
	{
		child_process(cmd, state, has_next);
	}
	else
	{
		pids[(state->pid_count)++] = pid;
		parent_process_cleanup(state, has_next);
	}
	return (0);
}

int	process_command(t_command *cmd, t_state *state, pid_t *pids)
{
	int	has_next;

	has_next = (cmd->next != NULL);
	state->heredoc_fd = -1;
	if (setup_pipe(state, has_next))
		return (-1);
	if (handle_heredoc(cmd, state))
		return (-1);
	if (cmd->command && is_parent_builtin(cmd->command) && !has_next)
	{
		if (execute_builtin_last(cmd, state))
			return (-1);
	}
	else
	{
		if (fork_and_execute(cmd, state, has_next, pids))
			return (-1);
	}
	return (0);
}

static void	wait_for_children(pid_t *pids, t_state *state)
{
	int	status;
	int	i;

	i = 0;
	while (i < state->pid_count)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			state->last_exit_code = WEXITSTATUS(status);
		i++;
	}
}

void	execute_pipeline(t_command *cmd, t_state *state)
{
	pid_t	pids[256];

	state->in_fd = STDIN_FILENO;
	state->pid_count = 0;
	while (cmd)
	{
		if (process_command(cmd, state, pids))
			return ;
		cmd = cmd->next;
	}
	wait_for_children(pids, state);
}
