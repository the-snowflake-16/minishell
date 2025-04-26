#include "minishell.h"
void ft_pwd()
{
    char path[1024];
    getcwd(path, sizeof(path));
    printf("%s\n", path);
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
int count_words_in_arr(char **ss)
{
    int i = 0;
    while (ss[i])
    {
        i++;
    }
    return i;
}

void change_cwd(char **swd)
{
    if(chdir(swd[1])== -1)
        printf("dirctory does`n exist\n");
}

void cmp_inpu(t_command *command, t_env *my_env, t_token *token)
{
    // printf("%s\n", parser_list->word);
    if(!ft_strcmp(command->command, "exit"))
    {
        free_comand(command);
        free_env(my_env);
        free_token(token);
        // 
        exit(1);
    }
    else if(!ft_strcmp(command->command, "pwd"))
        ft_pwd();
    else if(!ft_strcmp(command->command, "cd"))
        change_cwd(command->args);
    else if(!ft_strcmp(command->command, "export") && count_words_in_arr(command->args) > 1)
        export_create(my_env, command->args[1]);

    else if(!ft_strcmp(command->command, "export") && count_words_in_arr(command->args) == 1)
        export_print(my_env);
    else if(!ft_strcmp(command->command, "env"))
        print_env(my_env);
    else if(!ft_strcmp(command->command, "unset"))
    {
        if(!command->args[1])
            printf ("unset: not enough arguments\n");
        else
            unset(&my_env, command->args[1]);
    }
    else if(command->command)
        start_execve(command->args, my_env);
}


void start_token(char *input, t_env *my_env)
{

    if(!incorect_input(input))
    {
        char *sort_input = filter(input);
        t_token *token = tokenize(sort_input);
        check_arr_of_token(token);
        free(sort_input);
        
        t_parser *parser_list = create_list(token->token_arr, my_env);
        t_command *command = create_command(parser_list);
        if (command) {
            cmp_inpu(command, my_env, token);
            print_command(command);
            free_comand(command);
        }

        free_token(token); 
    }
   
}
void init_minishell(char **env)
{
    t_env *my_env = init_env(env);
    char *input;
    while (1)
    {
        input = readline("minishell$ ");
        if(!input)
            break;
        if(input)
            add_history(input);
        start_token(input, my_env);
        free(input);
    } 
}

int main(int argc, char **argv, char **env) {
    (void)argv;
    (void)argc;
    // check_input(argc, argv, env);
    init_minishell(env);
    return 0;
}