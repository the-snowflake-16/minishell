#include "minishell.h"

t_token *tokenize(char *s)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;

    token->token_arr = ft_splitt(s);
    token->count_tokens = count_words(s);
    return token;
}

void free_token(t_token *token)
{
    int i = 0;
    while (token->token_arr[i])
    {
        free(token->token_arr[i]);
        i++;
    }
    free(token->token_arr);
    free(token);
}

