/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 12:57:33 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/07 12:59:58 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_comand(t_command *comand)
{
	comand->args = NULL;
	comand->redirects = NULL;
	comand->next = NULL;
}

void	init_redirin(t_command *cmd, t_token_type inp, char *file)
{
	if (!cmd->redirects)
	{
		cmd->redirects = malloc(sizeof(t_redirect));
		if (!cmd->redirects)
			return ;
	}
	if (inp == TOKEN_REDIR_IN)
	{
		cmd->redirects->file = ft_strdup(file);
		cmd->redirects->is_heredoc = false;
		cmd->redirects->is_append = false;
		cmd->redirects->is_input = true;
		cmd->redirects->is_output = false;
	}
}

void	init_heredok(char *delimiter, t_command *cmd)
{
	if (!cmd->redirects)
	{
		cmd->redirects = malloc(sizeof(t_redirect));
		if (!cmd->redirects)
			return ;
	}
	cmd->redirects->file = NULL;
	cmd->redirects->is_heredoc = true;
	cmd->redirects->is_append = false;
	cmd->redirects->is_input = false;
	cmd->redirects->is_output = false;
	cmd->redirects->delimiter = ft_strdup(delimiter);
}

void	init_append(t_token_type inp, char *file, t_command *cmd)
{
	if (!cmd->redirects)
	{
		cmd->redirects = malloc(sizeof(t_redirect));
		if (!cmd->redirects)
			return ;
	}
	cmd->redirects->file = ft_strdup(file);
	if (inp == TOKEN_REDIR_APPEND)
	{
		cmd->redirects->is_heredoc = false;
		cmd->redirects->is_output = false;
		cmd->redirects->is_input = false;
		cmd->redirects->is_append = true;
	}
}

void	init_redir_out(t_token_type inp, char *file, t_command *cmd)
{
	if (!cmd->redirects)
	{
		cmd->redirects = malloc(sizeof(t_redirect));
		if (!cmd->redirects)
			return ;
	}
	cmd->redirects->file = ft_strdup(file);
	if (inp == TOKEN_REDIR_OUT)
	{
		cmd->redirects->is_heredoc = false;
		cmd->redirects->is_append = false;
		cmd->redirects->is_input = false;
		cmd->redirects->is_output = true;
	}
}
