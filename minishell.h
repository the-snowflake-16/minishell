#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#include <readline/readline.h>
#include <readline/history.h>


typedef enum e_token_type {
    TOKEN_WORD,         // команда или аргумент
    TOKEN_PIPE,         //  |
    TOKEN_REDIR_IN,       //  <
    TOKEN_REDIR_OUT,    //  >
    TOKEN_REDIR_APPEND, // >>
    TOKEN_REDIR_HEREDOC, // <<
} t_token_type;

typedef struct s_token
{
    char    **token_arr;
    int     count_tokens;
} t_token;

typedef struct s_parser
{
    char *word;
    t_token_type type;
    struct s_parser *next;
    int single_quotes;
    int double_quotes;
} t_parser;

typedef struct s_env
{
    char *key;
    char *value;
    int exported;
    int index;
    struct s_env *next;
} t_env;

typedef struct s_redirect {
    char *file;       // Куда/откуда перенаправляем (имя файла)
    bool is_output;   // Это выход (> или >>) или вход (< или <<)?
    bool append;      // Дописывать (>>) или перезаписать (>)?
    bool is_heredoc;  // Это специальный ввод << (heredoc)?
    // char *delimiter;  // Разделитель для heredoc (например, "EOF")
} t_redirect;

typedef struct s_command {
    char *command;
    char **args;           // Аргументы команды (например, ["ls", "-la"])
    t_redirect *input;     // Входной редирект (< или <<)
    t_redirect *output;    // Выходной редирект (> или >>)
    struct s_command *next; // Следующая команда (для пайпов)
} t_command;

// token_utils.c
int count_words(char *s);
char **ft_splitt(char *s);
// only for checking don`t delete
//  !!!!!!!!!!!!!!!!!!
void check_arr_of_token(t_token *token);



// token.c
t_token *tokenize(char *s);
void free_token(t_token *token);

// check_list.c
void check_symbol(t_parser *parser);

// sort_input.c
char	*filter(char *s);

// create_list.c
void free_list(t_parser *head);
t_parser *create_list(char **ss, t_env *my_env);
void print_list(t_parser *parser);
t_command *create_command(t_parser *parser);
void print_command(t_command *cmd);
void free_comand(t_command *command);


// env.c
t_env *init_env(char **s);
void print_env(t_env *env);
void free_env(t_env *env);
t_env *add_left_right(char *s);


// first_check_input.c
int incorect_input(char *s);

// execve.c
void start_execve(char **ss, t_env *my_env);
char **t_env_to_envp(t_env *env);

// export_print.c
void export_print(t_env *my_env);
int count_exported(t_env *my_env);

// export_add.c
void export_add(t_env *my_env, char *s);
t_env *find_key(t_env *my_env, char *s);
int incorect_input_for_key(char *s);
char *split_by_equal_key(char *s);
int check_key_in_env(t_env *my_env, char *s);

// export_utils.c
int check_str_for_export_add_after_equal(char *s);

// export_create.c
void export_create(t_env *my_env, char *s);
void unset(t_env **my_env, char *s);

#endif