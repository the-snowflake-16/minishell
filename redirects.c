/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 07:12:37 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/07 07:39:40 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_heredoc_interrupted = 0;

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 1;
	rl_done = 1;
}

int	redirect_output(const char *file)
{
	int	file_fd;

	file_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (file_fd < 0)
	{
		perror("open output");
		return (1);
	}
	if (dup2(file_fd, STDOUT_FILENO) < 0)
	{
		perror("dup2 output");
		close(file_fd);
		return (1);
	}
	close(file_fd);
	return (0);
}

int	redirect_input(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror("open input");
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2 input");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	redirect_append(const char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd < 0)
	{
		perror("open append");
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2 append");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

void	handle_child_process(int *pipe_fd, const char *delimiter)
{
	char	*line;

	signal(SIGINT, heredoc_sigint_handler);
	close(pipe_fd[0]);
	while (1)
	{
		line = readline("❄️  ");
		if (!line)
			exit(0);
		if (g_heredoc_interrupted)
			exit(1);
		if (!strcmp(line, delimiter))
		{
			free(line);
			exit(0);
		}
		write(pipe_fd[1], line, strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
}
