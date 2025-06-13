/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 07:19:08 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/13 15:54:40 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	cleanup_fds(int heredoc_fd, int *fd, int *in_fd, t_state *s)
// {
// 	if (heredoc_fd != -1)
// 		close(heredoc_fd);
// 	if (s->has_next)
// 		close(fd[1]);
// 	if (*in_fd != STDIN_FILENO)
// 		close(*in_fd);
// 	if (s->has_next)
// 		*in_fd = fd[0];
// 	else
// 		*in_fd = STDIN_FILENO;
// }

// int	launch_child(t_command *cmd, t_state *s, int *fd, int *in_fd)
// {
// 	pid_t	pid;
// 	int		status;
// 	int		heredoc_fd;

// 	heredoc_fd = handle_heredoc(cmd->redirects);
// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		perror("fork");
// 		if (heredoc_fd != -1)
// 			close(heredoc_fd);
// 		return (1);
// 	}
// 	if (pid == 0)
// 		child_process(cmd, s, fd, *in_fd);
// 	waitpid(pid, &status, 0);
// 	update_exit_code(s, status);
// 	cleanup_fds(heredoc_fd, fd, in_fd, s);
// 	return (0);
// }
// void	child_process(t_command *cmd, t_state *s, int *fd, int in_fd)
// {
// 	int	heredoc_fd;

// 	heredoc_fd = handle_heredoc(cmd->redirects);
// 	if (heredoc_fd != -1)
// 	{
// 		dup2(heredoc_fd, STDIN_FILENO);
// 		close(heredoc_fd);
// 	}
// 	redirect_fds(s, fd, in_fd);
// 	if (apply_redirects(cmd->redirects))
// 		exit(1);
// 	cmp_input(cmd, s);
// 	exit(s->last_exit_code);
// }

// int	heredok(const char *delimiter)
// {
// 	int		pipe_fd[2];
// 	pid_t	pid;
// 	int		status;

// 	if (pipe(pipe_fd) == -1)
// 		return (-1);
// 	pid = fork();
// 	if (pid == -1)
// 		return (-1);
// 	if (pid == 0)
// 		handle_child_process(pipe_fd, delimiter);
// 	close(pipe_fd[1]);
// 	waitpid(pid, &status, 0);
// 	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
// 	{
// 		close(pipe_fd[0]);
// 		return (-1);
// 	}
// 	return (pipe_fd[0]);
// }

// int	is_parent_builtin(const char *cmd)
// {
// 	if (!cmd)
// 		return (0);
// 	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export")
// 		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"));
// }
