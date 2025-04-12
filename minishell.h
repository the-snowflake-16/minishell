#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>


typedef enum e_token_type {
    TOKEN_WORD,         // Обычное слово (команда или аргумент)
    TOKEN_PIPE,         // Символ pipe |
    TOKEN_REDIR_IN,       // Перенаправление ввода <
    TOKEN_REDIR_OUT,    // Перенаправление вывода >
    TOKEN_REDIR_APPEND, // Дописывание в файл >>
    TOKEN_REDIR_HEREDOC, // Здесь-документ <<
    TOKEN_QUOTE,        // Кавычки '
    TOKEN_DQUOTE,       // Двойные кавычки "
    TOKEN_EOF,          // Конец ввода
    TOKEN_ERROR,         // Ошибка токенизации
    CAT,
    ECHO
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

#endif