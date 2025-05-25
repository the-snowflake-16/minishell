#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

#include <stdio.h>
#include <unistd.h>

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <limits.h> // for LLONG_MAX, LLONG_MIN
#include <stdlib.h> // for strtol
#include <ctype.h>  // for isdigit

int count_words_in_arr(char **ss) {
    int i = 0;
    while (ss[i]) i++;
    return i;
}

int is_numeric(const char *str) {
    if (!str) return 0;
    if (*str == '+' || *str == '-') str++;
    if (!*str) return 0;
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

int handle_exit(t_command *command, t_state *state) {
    int argc = count_words_in_arr(command->args);
    printf("exit\n");

    if (argc == 1) {
        free_comand(command);
        free_env(state->env);
        exit(state->last_exit_code);
    }

    if (!is_numeric(command->args[1])) {
        fprintf(stderr, "minishell: exit: %s: numeric argument required\n", command->args[1]);
        free_comand(command);
        free_env(state->env);
        exit(2);
    }

    if (argc > 2) {
        fprintf(stderr, "minishell: exit: too many arguments\n");
        state->last_exit_code = 1;
        return 1; // Do NOT exit
    }

    long long num = strtoll(command->args[1], NULL, 10);
    unsigned char exit_code = (unsigned char)(num % 256);
    free_comand(command);
    free_env(state->env);
    exit(exit_code);
}

void ft_pwd(void) {
    char path[4096];
    if (getcwd(path, sizeof(path)) != NULL) {
        printf("%s\n", path);
    } else {
        perror("pwd");
    }
}


int change_cwd(char **swd) {
    int count = 0;
    while (swd[count]) count++;

    if (count == 1) {
        char *home = getenv("HOME");
        if (!home) {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
        if (chdir(home) == -1) {
            perror("cd");
            return 1;
        }
    } else if (count > 2) {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    } else {
        if (chdir(swd[1]) == -1) {
            perror("cd");
            return 1;
        }
    }
    return 0;
}

int echo(t_command *command) {
    int i = 1;
    while (command->args[i]) {
        if (command->single_qoutes) {
            // При одинарных кавычках ничего не интерпретируем
            printf("%s", command->args[i]);
        }
        if (command->args[i + 1])
            printf(" ");
        i++;
    }
    printf("\n");
    return 0;
}


int is_builtin(const char *cmd) {
    return (!ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "echo") ||
            !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "cd") ||
            !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset") ||
            !ft_strcmp(cmd, "env"));
}

int handle_builtin(t_command *command, t_state *state) {
    if (!ft_strcmp(command->command, "exit")) {
        handle_exit(command, state);
    } 
    else if (!ft_strcmp(command->command, "pwd"))
        ft_pwd();
    else if (!ft_strcmp(command->command, "cd"))
    {
        state->last_exit_code = change_cwd(command->args);

    }
        
    else if (!ft_strcmp(command->command, "export") && count_words_in_arr(command->args) > 1)
    {
        state->last_exit_code = export_create(state->env, command);
    }
        
    else if (!ft_strcmp(command->command, "export"))
        export_print(state->env);
    else if (!ft_strcmp(command->command, "env"))
        print_env(state->env);
    else if (!ft_strcmp(command->command, "unset")) {
        if (!command->args[1])
        {
            
            // state->last_exit_code = 1;
        }
        else
            state->last_exit_code = unset(&state->env, command->args[1]);
    }
    else if (!ft_strcmp(command->command, "echo"))
    {
        // printf("%d\n", state->last_exit_code);
        state->last_exit_code = echo(command);
    }
    return 1;
}

void cmp_input(t_command *command, t_state *state) {
    if (command->command && is_builtin(command->command))
        handle_builtin(command, state);
    else if (command->command)
        state->last_exit_code = start_execve(command->args, state->env);
}

void start_token(char *input, t_state *state) {
    if (!incorect_input(input)) {
        char *sort_input = filter(input);
        t_token *token = tokenize(sort_input);
        free(sort_input);
        // check_arr_of_token(token);
        t_parser *parser_list = create_list(token->token_arr, state);
        t_command *command = create_command(parser_list);
        if (command) {
            // print_command(command);
            execute_pipeline(command, state);
            free_comand(command);
        }
        free_token(token);
    }
}
// void free_state(t_state *state)
// {
//     if (!state)
//         return;

//     if (state->env)
//         free_env(state->env);

//     free(state);
// }

void init_minishell(char **env) {
    t_state state;
    state.env = init_env(env);
    state.last_exit_code = 0;

    char *input;
    while (1) {
        input = readline("minishell❄️   ");
        if (!input) break;
        if (input) add_history(input);
        start_token(input, &state);
        free(input);
    }
    free_env(state.env);
}

int main(int argc, char **argv, char **env) {
    (void)argv;
    (void)argc;
    init_minishell(env);
    return 0;
}