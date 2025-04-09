#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_token
{
    char    **token_arr;
    int     count_tokens;
} t_token;

// token_utils.c
int count_words(char *s);

#endif