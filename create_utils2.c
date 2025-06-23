/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 13:12:52 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/23 10:02:34 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirect(t_redirect *redirect)
{
	if (!redirect)
		return ;
	if (redirect->file)
		free(redirect->file);
	free(redirect);
}

void	free_comand(t_command *command)
{
	t_command	*tmp;
	int			i;

	while (command)
	{
		tmp = command->next;
		if (command->command)
			free(command->command);
		if (command->args)
		{
			i = 0;
			while (command->args[i])
			{
				free(command->args[i]);
				i++;
			}
			free(command->args);
		}
		if (command->redirects)
			free_redirect(command->redirects);
		free(command);
		command = tmp;
	}
}
