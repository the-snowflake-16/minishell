/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredok.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 08:13:32 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/15 16:19:42 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_in_heredoc = 0;

void	heredoc_child_process(const char *delimiter, int pipe_fd[2])
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	close(pipe_fd[0]);
	while (1)
	{
		line = readline("❄️  ");
		if (!line)
		{
			close(pipe_fd[1]);
			exit(0);
		}
		if (!ft_strcmp(line, delimiter))
		{
			free(line);
			close(pipe_fd[1]);
			exit(0);
		}
		write_line_to_pipe(pipe_fd[1], line);
		free(line);
	}
}

int	heredoc_parent_process(pid_t pid, int *pipe_fd)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	return (handle_child_status(status, pipe_fd[0]));
}

static void	restore_signal_handler(void (*old_handler)(int))
{
	signal(SIGINT, old_handler);
	g_in_heredoc = 0;
}

static int	setup_heredoc_pipe_and_fork(int pipe_fd[2],
	void (*old_handler)(int))
{
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (-1);
	g_in_heredoc = 1;
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		restore_signal_handler(old_handler);
		return (-1);
	}
	return (pid);
}

int	heredok(const char *delimiter)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		result;
	void	(*old_handler)(int);

	old_handler = signal(SIGINT, SIG_IGN);
	pid = setup_heredoc_pipe_and_fork(pipe_fd, old_handler);
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		heredoc_child_process(delimiter, pipe_fd);
		exit(1);
	}
	else
	{
		result = heredoc_parent_process(pid, pipe_fd);
		restore_signal_handler(old_handler);
		return (result);
	}
}
