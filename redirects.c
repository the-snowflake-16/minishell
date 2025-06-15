/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 07:12:37 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/15 16:18:06 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	is_parent_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "unset"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

int	setup_pipe(t_state *state, int has_next)
{
	if (has_next && pipe(state->fd) < 0)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}
