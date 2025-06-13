#include "minishell.h"

char	*strjoin_and_free(char *s1, const char *s2)
{
	char	*new;

	new = ft_strjoin(s1, s2);
	free(s1);
	return (new);
}

char	*strjoin_and_free_char(char *s1, char c)
{
	char	temp[2];

	temp[0] = c;
	temp[1] = '\0';
	return (strjoin_and_free(s1, temp));
}

t_parser	*creat_node(char *s)
{
	t_parser	*new;

	new = malloc(sizeof(t_parser));
	if (!new)
		return (NULL);
	new->word = ft_strdup(s);
	new->type = chech_symbol(s);
	new->next = NULL;
	return (new);
}

t_parser	*append_node(t_parser **head, t_parser **current, t_parser *new_node)
{
	if (!new_node)
		return (NULL);
	if (!*head)
	{
		*head = new_node;
		*current = new_node;
	}
	else
	{
		(*current)->next = new_node;
		*current = new_node;
	}
	return (new_node);
}

int	first_quoter(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'')
			return (1);
		else if (s[i] == '"')
			return (2);
		i++;
	}
	return (0);
}

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

static char	*expand_question_mark(char *expanded, t_state *state)
{
	char	*exit_str;

	exit_str = ft_itoa(state->last_exit_code);
	expanded = strjoin_and_free(expanded, exit_str);
	free(exit_str);
	return (expanded);
}

static char	*expand_variable(char *word, int *j, t_state *state, char *expanded)
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
	int			i;
	t_parser	*head;
	t_parser	*current;
	t_parser	*new_node;
	char		*clean;
	char		*expanded;

	i = 0;
	head = NULL;
	current = NULL;
	while (ss[i])
	{
		clean = check_quoter(ss[i]);
		if (first_quoter(ss[i]) == 1)
			new_node = creat_node(clean);
		else
        {
            expanded= clean_word(clean, state);
			new_node = creat_node(expanded);
			free(expanded);
        }
		free(clean);
		if (!append_node(&head, &current, new_node))
			return (NULL);
		i++;
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


