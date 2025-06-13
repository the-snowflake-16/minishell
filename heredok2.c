/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredok2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 07:45:25 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/13 07:49:31 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	write_line_to_pipe(int pipe_fd, char *line)
{
	write(pipe_fd, line, strlen(line));
	write(pipe_fd, "\n", 1);
	return (0);
}

int	handle_child_status(int status, int pipe_fd)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipe_fd);
		return (-1);
	}
	return (pipe_fd);
}

int	handle_heredoc(t_command *cmd, t_state *state)
{
	if (cmd->redirects && cmd->redirects->is_heredoc)
	{
		state->heredoc_fd = heredok(cmd->redirects->delimiter);
		free(cmd->redirects->delimiter);
		if (state->heredoc_fd == -1)
			return (-1);
	}
	return (0);
}
