#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

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
} t_parser;

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;

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
t_parser	*create_list(char **ss);
void print_list(t_parser *parser);


// env.c
t_env *init_env(char **s);
void print_env(t_env *env);
void free_env(t_env *env);


// first_check_input.c
int incorect_input(char *s);

// execve.c
void start_execve(t_parser *parser_list, t_env *my_env);

#endif