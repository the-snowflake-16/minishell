#include "minishell.h"

void print_list(t_parser *parser)
{
    int i = 0;
    while (parser != NULL)
    {
        printf("token %d ",i);
        printf ("%s ", parser->word);
        if(parser->type == TOKEN_WORD)
            printf("type word\n");
       if(parser->type == TOKEN_PIPE)
            printf("type pipe\n");
        if(parser->type == TOKEN_REDIR_IN)
            printf("type redir in <\n");
        if(parser->type == TOKEN_REDIR_OUT)
            printf("type redir out >\n");
        if(parser->type == TOKEN_REDIR_APPEND)
            printf("type redie append >>\n");
        if(parser->type == TOKEN_REDIR_HEREDOC)
            printf("type redir heredoc <<\n");  
        parser = parser->next;

        i++;
    }
}

void free_list(t_parser *head)
{
    t_parser *current = head;
    t_parser *next;
    
    while (current != NULL)
    {
        next = current->next;
        free(current->word);
        free(current);
        current = next;
    }
}
int	chech_symbol(char *s)
{
	if (!ft_strcmp(s, "<<"))
		return (TOKEN_REDIR_HEREDOC);
	if (!ft_strcmp(s, ">>"))
		return (TOKEN_REDIR_APPEND);
	if (!ft_strcmp(s, "<"))
		return (TOKEN_REDIR_IN);
	if (!ft_strcmp(s, ">"))
		return (TOKEN_REDIR_OUT);
	if (!ft_strcmp(s, "|"))
		return (TOKEN_PIPE);
	return (TOKEN_WORD);
}

t_parser *creat_node(char *s)
{
    t_parser *new = malloc(sizeof(t_parser));
    if(!new)
        return NULL;
    new->word = ft_strdup(s);
    new->type = chech_symbol(s);
    new->next = NULL;
    return (new);
}

t_parser	*create_list(char **ss)
{
	int			i = 0;
	t_parser	*head = NULL;
	t_parser	*current = NULL;
	t_parser	*new_node;

	while (ss[i])
	{
		new_node = creat_node(ss[i]);
		if (!new_node)
			return (NULL);
		if (!head)
		{
			head = new_node;
			current = head;
		}
		else
		{
			current->next = new_node;
			current = current->next;
		}
		i++;
	}
	return (head);
}



// int main(void)
// {
//     char *input = "cat'hello| ||| world'<<'hello world'>>|echo";

//     char *sort_input = filter(input);
//     t_token *token = tokenize(sort_input);
//     check_arr_of_token(token);
    
//     t_parser *parser_list = create_list(token->token_arr);
//     if (parser_list) {
//         print_list(parser_list);
//         // check_symbol(parser_list);
//         free_list(parser_list);
//     }
//     free(sort_input);
//     free_token(token);
//     return 0;
// }