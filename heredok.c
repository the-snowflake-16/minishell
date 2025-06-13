/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredok.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 08:13:32 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/13 08:13:33 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile	sig_atomic_t g_heredoc_interrupted = 0;

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 1;
	rl_done = 1;
}

int	process_heredoc_line(char *line, const char *delimiter, int pipe_fd)
{
	if (!line)
		return (1);
	if (g_heredoc_interrupted)
		return (1);
	if (!strcmp(line, delimiter))
		return (1);
	write_line_to_pipe(pipe_fd, line);
	return (0);
}

void	heredoc_child_process(const char *delimiter, int pipe_fd)
{
	char	*line;

	signal(SIGINT, heredoc_sigint_handler);
	close(pipe_fd);
	while (1)
	{
		line = readline("❄️  ");
		if (process_heredoc_line(line, delimiter, pipe_fd))
		{
			if (line)
				free(line);
			exit(0);
		}
		free(line);
	}
}

int	heredoc_parent_process(pid_t pid, int *pipe_fd)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	return (handle_child_status(status, pipe_fd[0]));
}

int	heredok(const char *delimiter)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		heredoc_child_process(delimiter, pipe_fd[0]);
		exit(1);
	}
	else
	{
		return (heredoc_parent_process(pid, pipe_fd));
	}
}
