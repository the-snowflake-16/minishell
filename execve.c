/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 10:32:53 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/03 10:59:43 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_with_slash(const char *dir, const char *command)
{
	size_t	len_dir;
	size_t	len_command;
	char	*full_path;

	if (!dir || !command)
		return (NULL);
	len_dir = ft_strlen(dir);
	len_command = ft_strlen(command);
	full_path = (char *)malloc(len_dir + len_command + 2);
	if (!full_path)
		return (NULL);
	strcpy(full_path, dir);
	full_path[len_dir] = '/';
	strcpy(full_path + len_dir + 1, command);
	return (full_path);
}

static void	handle_absolute_or_relative(char **args, char **envp)
{
	if (!access(args[0], X_OK))
	{
		execve(args[0], args, envp);
		perror("execve");
		exit(126);
	}
	else
	{
		perror(args[0]);
		exit(127);
	}
}

static void	handle_path_not_found(char *cmd)
{
	fprintf(stderr, "minishell: %s: No such file or directory\n", cmd);
	exit(127);
}

static void	search_and_exec_in_path(char **args, char **paths, char **envp)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin_with_slash(paths[i], args[0]);
		if (!access(full_path, X_OK))
		{
			execve(full_path, args, envp);
			perror("execve");
			exit(126);
		}
		free(full_path);
		i++;
	}
	fprintf(stderr, "minishell: command not found: %s\n", args[0]);
	exit(127);
}

void	start_execve(char **args, t_state *state)
{
	char	**envp;
	char	*path;
	char	**paths;

	envp = t_env_to_envp(state->env);
	if (args[0][0] == '/' || ft_strncmp(args[0], "./", 2) == 0
		|| ft_strncmp(args[0], "../", 3) == 0)
		handle_absolute_or_relative(args, envp);
	path = get_path_from_env(state->env);
	if (!path)
		handle_path_not_found(args[0]);
	paths = ft_split(path, ':');
	search_and_exec_in_path(args, paths, envp);
}
