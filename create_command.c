/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 10:39:27 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/13 14:11:43 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_redirection_without_target(t_parser *parser)
{
	return ((parser->type == TOKEN_REDIR_APPEND
			|| parser->type == TOKEN_REDIR_HEREDOC
			|| parser->type == TOKEN_REDIR_IN
			|| parser->type == TOKEN_REDIR_OUT)
		&& parser->next == NULL);
}

static bool	fill_command_args(t_parser **parser, t_command *cmd)
{
	int	j;

	j = 0;
	while (*parser && (*parser)->type != TOKEN_PIPE)
	{
		if (is_redirection_without_target(*parser))
		{
			perror("error\n");
			return (false);
		}
		if (handle_redirections(parser, cmd))
			continue ;
		if ((*parser)->word != NULL)
		{
			cmd->args[j] = ft_strdup((*parser)->word);
			cmd->single_quotes = (*parser)->single_quotes;
			j++;
		}
		*parser = (*parser)->next;
	}
	cmd->args[j] = NULL;
	return (true);
}

static t_command	*new_command(t_parser **parser)
{
	t_command	*cmd;
	int			arg_count;

	arg_count = count_args_until_pipe(*parser);
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	init_comand(cmd);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return (NULL);
	if (!fill_command_args(parser, cmd))
	{
		free(cmd->args);
		free(cmd);
		return (NULL);
	}
	if (cmd->args[0])
		cmd->command = ft_strdup(cmd->args[0]);
	return (cmd);
}

t_command	*create_parser(t_parser *parser)
{
	t_command	*head;
	t_command	*prev;
	t_command	*cmd;

	head = NULL;
	prev = NULL;
	while (parser)
	{
		cmd = new_command(&parser);
		if (!cmd)
			return (NULL);
		if (!head)
			head = cmd;
		if (prev)
			prev->next = cmd;
		prev = cmd;
		if (parser && parser->type == TOKEN_PIPE)
			parser = parser->next;
	}
	return (head);
}

t_command	*create_command(t_parser *parser)
{
	t_command	*command;

	if (cheak_token(parser))
	{
		command = create_parser(parser);
		free_list(parser);
		return (command);
	}
	return (NULL);
}
