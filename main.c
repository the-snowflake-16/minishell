#include "minishell.h"
void ft_pwd(t_env *my_env)
{
    char path[1024];
    getcwd(path, sizeof(path));
    printf("Текущий каталог: %s\n", path);
    // while (my_env)
    // {
    //     if(! ft_strcmp(my_env->key, "PWD"))
    //         printf("%s\n", my_env->value);
    //     my_env = my_env->next;
    // }
}
// void check_input(int argc, char **argv, char **env)
// {
//     if(argc == 3 && strcmp(argv[1], "-c") == 0)
//     {
//         // cmp_input(argv[2], env);
//         // exit(0);
//     }
// }

void change_cwd(char *cwd, char *swd)
{
    if(chdir(swd)== -1)
        printf("%s dirctory does`n exist\n", swd);
}

void cmp_inpu(t_parser *parser_list, t_env *my_env, t_token *token)
{
    // printf("%s\n", parser_list->word);
    if(!ft_strcmp(parser_list->word, "exit") && parser_list->next == NULL)
    {
        free_list(parser_list);
        free_env(my_env);
        free_token(token);
        exit(1);
    }
    else if(!ft_strcmp(parser_list->word, "pwd") && parser_list->next == NULL)
    {
        ft_pwd(my_env);
    }
       
    else if(!ft_strcmp(parser_list->word, "env") && parser_list->next == NULL)
        print_env(my_env);
    else if(!ft_strcmp(parser_list->word, "cd") && parser_list->next->type == TOKEN_WORD)
        change_cwd(parser_list->word, parser_list->next->word);
    else if(parser_list->word)
        start_execve(parser_list, my_env);
}


void start_token(char *input, t_env *env)
{
    if(!incorect_input(input))
    {
        char *sort_input = filter(input);
        t_token *token = tokenize(sort_input);
        // check_arr_of_token(token);
        free(sort_input);
        
        t_parser *parser_list = create_list(token->token_arr);
        if (parser_list) {
            cmp_inpu(parser_list, env, token);
            print_list(parser_list);
            free_list(parser_list);
        }

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

        
        t_env *my_env = init_env(env);
        start_token(input, my_env);

        
        // cmp_input(input, my_env);
        free(input);
        free_env(my_env);
    } 
}

int main(int argc, char **argv, char **env) {

    // check_input(argc, argv, env);
    init_minishell(env);
    return 0;
}