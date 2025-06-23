/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 10:07:01 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/23 10:15:04 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_env *my_env)
{
	while (my_env != NULL)
	{
		printf("%s=%s\n", my_env->key, my_env->value);
		my_env = my_env->next;
	}
}

int	cd_to_home(void)
{
	char	*home;

	home = getenv("HOME");
	if (!home)
	{
		fprintf(stderr, "cd: HOME not set\n");
		return (1);
	}
	if (chdir(home) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	cd_to_path(char *path)
{
	if (chdir(path) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	change_cwd(char **swd)
{
	int	count;

	count = 0;
	while (swd[count])
		count++;
	if (count == 1)
		return (cd_to_home());
	else if (count > 2)
	{
		fprintf(stderr, "cd: too many arguments\n");
		return (1);
	}
	else
		return (cd_to_path(swd[1]));
}
