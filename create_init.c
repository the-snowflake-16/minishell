/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 12:57:33 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/13 14:12:31 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init_comand(t_command *cmd)
{
	cmd->command = NULL;
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	cmd->single_quotes = false;
	cmd->exit_code = 0;
}


void	init_redirin(t_command *cmd, t_token_type inp, char *file)
{
	if (cmd->redirects)
		free_redirect(cmd->redirects);
	cmd->redirects = malloc(sizeof(t_redirect));
	if (!cmd->redirects)
		return ;
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
	if (cmd->redirects)
		free_redirect(cmd->redirects);
	cmd->redirects = malloc(sizeof(t_redirect));
	if (!cmd->redirects)
		return ;
	cmd->redirects->file = NULL;
	cmd->redirects->is_heredoc = true;
	cmd->redirects->is_append = false;
	cmd->redirects->is_input = false;
	cmd->redirects->is_output = false;
	cmd->redirects->delimiter = ft_strdup(delimiter);
}

void	init_append(t_token_type inp, char *file, t_command *cmd)
{
	if (cmd->redirects)
		free_redirect(cmd->redirects);
	cmd->redirects = malloc(sizeof(t_redirect));
	if (!cmd->redirects)
		return ;
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
	if (cmd->redirects)
		free_redirect(cmd->redirects);
	cmd->redirects = malloc(sizeof(t_redirect));
	if (!cmd->redirects)
		return ;
	cmd->redirects->file = ft_strdup(file);
	if (inp == TOKEN_REDIR_OUT)
	{
		cmd->redirects->is_heredoc = false;
		cmd->redirects->is_append = false;
		cmd->redirects->is_input = false;
		cmd->redirects->is_output = true;
	}
}
