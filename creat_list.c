#include "minishell.h"

void print_list(t_parser *parser)
{
    int i = 0;
    while (parser != NULL)
    {
        printf("token %d ",i);
        printf ("%s\n", parser->word);
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

t_parser *creat_node(char *s)
{
    t_parser *new = malloc(sizeof(t_parser));
    if(!new)
        return NULL;
    new->word = ft_strdup(s);
    new->next = NULL;
    return (new);
}
int chech_symbol(char *s)
{
    if (s[0] == '|' && ft_strlen(s) > 1)
        return (TOKEN_PIPE);
    else if(s[0] == '<' && ft_strlen(s) > 1)
        return (TOKEN_REDIR_IN);
    else if (s[0] == '>' && ft_strlen(s) > 1)
        return(TOKEN_REDIR_OUT);
    return (0);
}

t_parser *create_list(char **ss)
{
    int i = 0;
    t_parser *head = NULL;
    t_parser *current = NULL;
    
    while (ss[i])
    {
        if(!ft_strcmp(ss[i], "<") && !ft_strcmp(ss[i+1],"<"))
        {
            current->next = creat_node("<<");
            current = current->next;
            i += 2;
        }
        if(!ft_strcmp(ss[i], ">") && !ft_strcmp(ss[i+1],">"))
        {
            current->next = creat_node(">>");
            current = current->next;
            i += 2;           
        }
        if (!head)
        {
            head = creat_node(ss[i]);
            current = head;
        }
        else
        {
            current->next = creat_node(ss[i]);
            current = current->next;
        }
        i++;     
    }
    return head;
}

void start_token(char *input)
{
    char *sort_input = filter(input);
    t_token *token = tokenize(sort_input);
    // check_arr_of_token(token);
    
    t_parser *parser_list = create_list(token->token_arr);
    if (parser_list) {
        print_list(parser_list);
        // check_symbol(parser_list);
        free_list(parser_list);
    }
    free(sort_input);
    free_token(token); 
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