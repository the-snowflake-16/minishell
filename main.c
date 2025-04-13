#include "minishell.h"

int cmp_input(char *input, char **env)
{
    if(strcmp(input, "env") == 0)
    {
        while (*env)
        {
            printf("%s\n", *env);
            env++;
        }
        return 1;
    }
    if(strcmp(input, "exit") == 0)
        exit(1);
    else
        printf("%s command not exist\n", input);
    return 0;
}
void check_input(int argc, char **argv, char **env)
{
    if(argc == 3 && strcmp(argv[1], "-c") == 0)
    {
        cmp_input(argv[2], env);
        exit(0);
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
        // cmp_input(input, env);
        free(input);
    }
    
}
int main(int argc, char **argv, char **env) {

    check_input(argc, argv, env);
    init_minishell(env);

    return 0;
}