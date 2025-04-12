#include "minishell.h"

void print_list(t_parser *parser)
{
    while (parser != NULL)
    {
        printf ("%s\n", parser->word);
        parser = parser->next;
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
t_parser *split_token_left(int e_toke)
{
    t_parser *new = malloc(sizeof(t_parser));
    if(e_toke == TOKEN_PIPE)
        new->word = ft_strdup("|");  // копируем "|" в динамическую память
    else if(e_toke == TOKEN_REDIR_IN)
        new->word = ft_strdup("<");
    else if(e_toke == TOKEN_REDIR_OUT)
        new->word = ft_strdup(">");
    else if (e_toke == TOKEN_REDIR_APPEND)
        new->word = ft_strdup(">>");
    else if (e_toke == TOKEN_REDIR_HEREDOC)
        new->word = ft_strdup("<<");
    else if (e_toke == CAT)
        new->word = ft_strdup("cat");
    new->next = NULL;
    return(new);
}

t_parser  *split_token_right(char *ss, int len)
{
    t_parser *new = malloc(sizeof(t_parser));
    char *res = malloc(sizeof(char) * (len + 1));
    int i = 1;
    int j = 0;
    while (j < len)
    {
        res[j] = ss[i];
        j++;
        i++;
    }
    res[j] = '\0';
    new->word = res;
    new->next = NULL;
    // free(res);
    return (new);
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
// int check_comand(char *s)
// {
//     char cat[] = "cat";
//     char append[] = ">>";
//     char heredoc[] = "<<";
//     int i = 0;
//     int j = 0;
//     while (s[i])
//     {
//         j = 0;
//         if(s[i] == append[0])
//         {

//         }
//         while (heredoc[j] == s[i])
//             i++;
//         if (i == 2)
//             return(TOKEN_REDIR_HEREDOC);
//         i = 0;
//         while (append[i] == s[i])
//             i++;
//         if (i == 2)
//             return(TOKEN_REDIR_APPEND);
//         i = 0;
//         while (cat[i] == s[i])
//             i++;
//         if(i == 2)
//             return(CAT);
//         else
//             return(chech_symbol(s));
//         return (0);
//     }
    

// }

void split_with_symbol(char *s, char *symbol, t_parser **head, t_parser **current)
{
    char *found = ft_strnstr(s, symbol, ft_strlen(s));
    
    if (!found) {
        // Symbol not found - add the whole string as a node
        t_parser *new = creat_node(s);
        if (!*head) *head = new;
        if (*current) (*current)->next = new;
        *current = new;
        return;
    }

    // Calculate positions
    int split_pos = found - s;
    int symbol_len = ft_strlen(symbol);

    // Add part before symbol
    if (split_pos > 0) {
        char *before = ft_substr(s, 0, split_pos);
        t_parser *new = creat_node(before);
        free(before);
        
        if (!*head) *head = new;
        if (*current) (*current)->next = new;
        *current = new;
    }

    // Add the symbol itself
    t_parser *sym_node = creat_node(symbol);
    if (*current) (*current)->next = sym_node;
    *current = sym_node;

    // Add part after symbol
    if (s[split_pos + symbol_len] != '\0') {
        char *after = ft_substr(s, split_pos + symbol_len, ft_strlen(s) - split_pos - symbol_len);
        t_parser *new = creat_node(after);
        free(after);
        
        if (*current) (*current)->next = new;
        *current = new;
    }
}

// t_parser *creat_list(char **ss)
// {
//     t_parser *head = NULL;
//     t_parser *current = NULL;
//     int i = 0;
    
//     while (ss[i])
//     {
//         t_parser *new_node;
//         if(!check_comand(ss[i]))
//         {
//             new_node = creat_node(ss[i]);
//         }
//         else if (check_comand(ss[i]))
//         {
//             new_node = split_token_left(check_comand(ss[i]));
//             if (!head) {
//                 head = new_node;
//                 current = head;
//             } else {
//                 current->next = new_node;
//                 current = current->next;
//             }
//             new_node = split_token_right(ss[i], ft_strlen(ss[i]));
//         }
//         if (!head) {
//             head = new_node;
//             current = head;
//         } else {
//             current->next = new_node;
//             current = current->next;
//         }
//         i++;

//     }
    
//     return head;
// }

int main(void)
{
    char *input = "cat'hello| ||| world' >>'hello world' |echo";

    char *sort_input = filter(input);
    t_token *token = tokenize(sort_input);
    check_arr_of_token(token);
    
    // t_parser *parser_list = creat_list(token->token_arr);
    // if (parser_list) {
    //     print_list(parser_list);
    //     // check_symbol(parser_list);
    //     free_list(parser_list);
    // }
    free(sort_input);
    free_token(token);
    return 0;
}