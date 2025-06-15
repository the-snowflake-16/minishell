/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 13:17:26 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/15 16:52:25 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_and_tokenize(char *input, t_token **token)
{
	char	*sort_input;

	if (incorect_input(input))
		return (0);
	sort_input = filter(input);
	*token = tokenize(sort_input);
	free(sort_input);
	if (!(*token) || !(*token)->token_arr)
		return (0);
	return (1);
}

static void	parse_and_execute(t_token *token, t_state *state)
{
	t_parser	*parser_list;
	t_command	*command;

	parser_list = create_list(token->token_arr, state);
	free_token(token);
	if (validate_redirections(parser_list))
	{
		free_list(parser_list);
		return ;
	}
	command = create_command(parser_list);
	if (command)
	{
		execute_pipeline(command, state);
		free_comand(command);
	}
	else
		free_list(parser_list);
}

void	start_token(char *input, t_state *state)
{
	t_token	*token;

	token = NULL;
	if (init_and_tokenize(input, &token))
		parse_and_execute(token, state);
}

void	init_minishell(char **env)
{
	t_state	state;
	char	*input;

	state.env = init_env(env);
	state.last_exit_code = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = readline("minishell❄️   ");
		if (!input)
			break ;
		if (input)
			add_history(input);
		start_token(input, &state);
		free(input);
	}
	free_env(state.env);
}

int	main(int argc, char **argv, char **env)
{
	(void)argv;
	(void)argc;
	rl_bind_key('\t', rl_insert);
	init_minishell(env);
	return (0);
}
