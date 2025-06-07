/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:53:25 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/07 07:59:44 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_parent_redirs(t_redirect *redir)
{
	if (redir->is_input && redirect_input(redir->file))
		return (1);
	if (redir->is_output && redirect_output(redir->file))
		return (1);
	if (redir->is_append && redirect_append(redir->file))
		return (1);
	return (0);
}

int	handle_heredoc(t_redirect *redir)
{
	int	fd;

	if (!redir || !redir->is_heredoc)
		return (-1);
	fd = heredok(redir->delimiter);
	free(redir->delimiter);
	return (fd);
}

void	redirect_fds(t_state *s, int *fd, int in_fd)
{
	if (s->has_next)
		dup2(fd[1], STDOUT_FILENO);
	if (s->has_next)
		close(fd[0]);
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
}

int	apply_redirects(t_redirect *r)
{
	if (!r)
		return (0);
	if (r->is_input && redirect_input(r->file))
		return (1);
	if (r->is_output && redirect_output(r->file))
		return (1);
	if (r->is_append && redirect_append(r->file))
		return (1);
	return (0);
}

void	execute_pipeline(t_command *cmd, t_state *state)
{
	int		fd[2];
	int		in_fd;

	in_fd = STDIN_FILENO;
	while (cmd)
	{
		state->has_next = (cmd->next != NULL);
		if (state->has_next && pipe(fd) < 0)
		{
			perror("pipe");
			return ;
		}
		if (cmd->command && is_parent_builtin(cmd->command)
			&& !state->has_next)
		{
			if (cmd->redirects && handle_parent_redirs(cmd->redirects))
				return ;
			state->last_exit_code = handle_builtin(cmd, state);
		}
		else if (launch_child(cmd, state, fd, &in_fd))
			return ;
		cmd = cmd->next;
	}
}
