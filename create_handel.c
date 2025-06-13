/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_handel.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 13:07:16 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/13 08:54:57 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	cheak_token(t_parser *parser)
{
	while (parser)
	{
		if (!ft_strcmp(parser->word, "|") && parser->next == NULL)
		{
			perror("error");
			return (false);
		}
		parser = parser->next;
	}
	return (true);
}

int	count_args_until_pipe(t_parser *parser)
{
	int	count;

	count = 0;
	while (parser && parser->type != TOKEN_PIPE)
	{
		count++;
		parser = parser->next;
	}
	return (count);
}

bool	handle_heredoc_append(t_parser **parser, t_command *cmd)
{
	if ((*parser)->type == TOKEN_REDIR_HEREDOC && (*parser)->next)
	{
		cmd->command = NULL;
		init_heredok((*parser)->next->word, cmd);
		*parser = (*parser)->next->next;
		return (true);
	}
	if ((*parser)->type == TOKEN_REDIR_APPEND)
	{
		init_append((*parser)->type, (*parser)->next->word, cmd);
		*parser = (*parser)->next->next;
		return (true);
	}
	return (false);
}

bool	handle_in_out_redirs(t_parser **parser, t_command *cmd)
{
	if ((*parser)->type == TOKEN_REDIR_OUT)
	{
		init_redir_out((*parser)->type, (*parser)->next->word, cmd);
		*parser = (*parser)->next->next;
		return (true);
	}
	if ((*parser)->type == TOKEN_REDIR_IN)
	{
		init_redirin(cmd, (*parser)->type, (*parser)->next->word);
		*parser = (*parser)->next->next;
		return (true);
	}
	return (false);
}

bool	handle_redirections(t_parser **parser, t_command *cmd)
{
	if (handle_heredoc_append(parser, cmd))
		return (true);
	if (handle_in_out_redirs(parser, cmd))
		return (true);
	return (false);
}
