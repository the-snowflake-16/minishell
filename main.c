#include "minishell.h"

void cmp_input(char *input, t_env *my_env)
{
    if(ft_strcmp(input, "exit") == 0)
    {
        free(input);
        free_env(my_env);
        
        exit(1);
    }
}
// void check_input(int argc, char **argv, char **env)
// {
//     if(argc == 3 && strcmp(argv[1], "-c") == 0)
//     {
//         // cmp_input(argv[2], env);
//         // exit(0);
//     }
// }
// void cmp_inpu(t_parser *parser_list)
// {
//     // printf("%s\n", parser_list->word);
//     if(!ft_strcmp(parser_list->word, "echo") && parser_list->next == NULL)
//         printf("\n");
//     else if(!ft_strcmp(parser_list->word, "echo") && parser_list->next->type == TOKEN_WORD)
//         printf("%s\n", parser_list->next->word);
// }
void start_token(char *input)
{
    if(!incorect_input(input))
    {
        char *sort_input = filter(input);
        t_token *token = tokenize(sort_input);
        // check_arr_of_token(token);
        
        t_parser *parser_list = create_list(token->token_arr);
        if (parser_list) {
            cmp_inpu(parser_list);
            // print_list(parser_list);
            free_list(parser_list);
        }
        free(sort_input);
        free_token(token); 
    }
   
}
void init_minishell(char **env)
{
    char *input;
    while (1)
    {
        input = readline("minishell$ ");
        if(!input)
            break;
        if(input)
            add_history(input);
        start_token(input);
        
        t_env *my_env = init_env(env);
        // print_env(my_env);
        cmp_input(input, my_env);
        free(input);
        free_env(my_env);
    } 
}

int main(int argc, char **argv, char **env) {

    // check_input(argc, argv, env);
    init_minishell(env);
    return 0;
}