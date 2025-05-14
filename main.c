#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

#include <stdio.h>
#include <unistd.h>

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int g_exit_code;

void ft_pwd(void)
{
    char path[4096];

    if (getcwd(path, sizeof(path)) != NULL)
    {
        printf("%s\n", path);
        g_exit_code = 0;
    }
    else
    {
        perror("pwd"); // Выводит: pwd: <ошибка>
        g_exit_code = 1;
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
int count_words_in_arr(char **ss)
{
    int i = 0;
    while (ss[i])
    {
        i++;
    }
    return i;
}

int change_cwd(char **swd)
{
    int count = 0;

    while (swd[count])
        count++;

    if (count == 1) // Только "cd" — переходим в $HOME
    {
        char *home = getenv("HOME");
        if (!home)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
        if (chdir(home) == -1)
        {
            perror("cd");
            return 1;
        }
    }
    else if (count > 2) // Слишком много аргументов
    {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }
    else // Обычный случай: cd path
    {
        if (chdir(swd[1]) == -1)
        {
            perror("cd");
            return 1;
        }
    }
    return 0;
}

int echo(t_command *command, t_env *my_env)
{
    int i = 1;
    // printf("Im in echo\n");

    while (command->args[i])
    {
        if (command->double_qoutes == true)
        {
            char *key = get_parametr(command->args[i]);
            // printf("key: %s\n", key);
            if (check_key_in_env(my_env, key))
            {
                // printf("IM HERE");
                t_env *my_key = find_key(my_env, key);
                // char *word = ft_strdup(my_key->value);
                printf("%s", my_key->value);
            }
            else
            {
                printf("%s", command->args[i]);
            }
            free(key);
        }
        
        else if(!ft_strcmp(command->args[i], "$?"))
        {
            printf("%d\n", command->exit_code);
            i++;
        }
        else {
            printf("%s", command->args[i]);
        }

        if (command->args[i + 1])
            printf(" ");
        i++;
    }
    printf("\n");
    return 0;
}

void cmp_inpu(t_command *command, t_env *my_env)
{

    // printf("%s\n", parser_list->word);
    if(!ft_strcmp(command->command, "exit"))
    {
        free_comand(command);
        free_env(my_env);
        // 
        exit(1);
    }
    else if(!ft_strcmp(command->command, "echo"))
        echo(command, my_env);
    // else if(command->redirects->is_output)
    //     redirect(command->redirects->file, command->args);
    // else if (command->redirects->is_append)
    //     redirect_append(command->redirects->file, command->args);
    
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
        // check_arr_of_token(token);
        free(sort_input);
        
        t_parser *parser_list = create_list(token->token_arr, my_env);
        t_command *command = create_command(parser_list);
        if (command) {
            execute_pipeline(command, my_env);
            // print_command(command);
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
        input = readline("minishell❄️   ");
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