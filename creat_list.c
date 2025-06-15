/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:49:54 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/15 16:21:48 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_quoter(char *word)
{
	char	*res;
	bool	is_single_quote;
	bool	is_double_quote;

	is_single_quote = ft_strchr(word, '\'') && first_quoter(word) == 1;
	is_double_quote = ft_strchr(word, '"') && first_quoter(word) == 2;
	if (is_single_quote)
		res = delete_quotes(word, '\'');
	else if (is_double_quote)
		res = delete_quotes(word, '"');
	else
		res = ft_strdup(word);
	return (res);
}

char	*expand_question_mark(char *expanded, t_state *state)
{
	char	*exit_str;

	exit_str = ft_itoa(state->last_exit_code);
	expanded = strjoin_and_free(expanded, exit_str);
	free(exit_str);
	return (expanded);
}

char	*expand_variable(char *word, int *j, t_state *state, char *expanded)
{
	int		start;
	char	*key;
	t_env	*found;

	start = *j;
	while (ft_isalnum(word[*j]) || word[*j] == '_')
		(*j)++;
	if (*j > start)
	{
		key = ft_substr(word, start, *j - start);
		found = find_key(state->env, key);
		if (found)
			expanded = strjoin_and_free(expanded, found->value);
		free(key);
	}
	else
		expanded = strjoin_and_free_char(expanded, '$');
	return (expanded);
}

char	*clean_word(char *word, t_state *state)
{
	char	*expanded;
	int		j;

	expanded = ft_calloc(1, 1);
	j = 0;
	while (word[j])
	{
		if (word[j] == '$')
		{
			j++;
			if (word[j] == '\0')
				return (strjoin_and_free(expanded, "$"));
			if (word[j] == '?')
			{
				j++;
				expanded = expand_question_mark(expanded, state);
			}
			else
				expanded = expand_variable(word, &j, state, expanded);
		}
		else
			expanded = strjoin_and_free_char(expanded, word[j++]);
	}
	return (expanded);
}

t_parser	*create_list(char **ss, t_state *state)
{
	t_parser	*head;
	t_parser	*current;
	t_parser	*new_node;
	char		*clean;
	char		*expanded;

	head = NULL;
	current = NULL;
	while (*ss)
	{
		clean = check_quoter(*ss);
		if (first_quoter(*ss) == 1)
			new_node = creat_node(clean);
		else
		{
			expanded = clean_word(clean, state);
			new_node = creat_node(expanded);
			free(expanded);
		}
		free(clean);
		if (!append_node(&head, &current, new_node))
			return (NULL);
		ss++;
	}
	return (head);
}

// void print_command(t_command *command)
// {

//     while (command)
//     {
//         int i = 0;
//         while (command->args && command->args[i])
//         {
//             printf("%s\n", command->args[i]);
//             i++;
//         }
//         command = command->next;
//     }
// }

// void print_redirect(t_redirect *redir)
// {
//     if (!redir)
//     {
//         printf("  No redirect\n");
//         return;
//     }
//     printf("  Redirect:\n");
//     if (redir->file != NULL) {
//         printf("Redirect file: %s\n", redir->file);
//     } else {
//         fprintf(stderr, "Error: Redirect file is NULL!\n");
//     }
//     printf("    is_input: %s\n", redir->is_input ? "true" : "false");
//     printf("    append: %s\n", redir->is_append ? "true" : "false");
//     printf("    is_heredoc: %s\n", redir->is_heredoc ? "true" : "false");
//     if(redir->is_heredoc)
//         printf("delimiter %s\n", redir->delimiter);
// }

// void print_command(t_command *cmd)
// {
//     int i;

//     while (cmd)
//     {
//         printf("Command:\n");
//         if (cmd->args)
//         {
//             i = 0;
//             while (cmd->args[i])
//             {
//                 printf("  arg[%d]: %s\n", i, cmd->args[i]);
//                 i++;
//             }
//         }
//         else
//             printf("  No args\n");

//         printf("Input redirect:\n");
//         print_redirect(cmd->redirects);

//         printf("\n---\n");
//         cmd = cmd->next;
//     }
// }
