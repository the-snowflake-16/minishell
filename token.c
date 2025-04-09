#include "minishell.h"

t_token *tokenize(char *s)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;

    token->token_arr = ft_split(s, ' ');
    token->count_tokens = count_words(s);

    for (int i = 0; i < token->count_tokens; i++)
    {
        printf("%s\n", token->token_arr[i]);
    }
    printf("Count: %d\n", token->count_tokens);
    return token;
}

void free_token(t_token *token)
{
        // Free the split array
    for (int i = 0; i < token->count_tokens; i++)
        free(token->token_arr[i]);
    free(token->token_arr);
    free(token);
}

int find_last_pipe(t_token *token)
{
    int start = 0;
    int end = token->count_tokens -1;
    while (end >= start)
    {
        if(ft_strcmp(token->token_arr[end], "|") == 0)
            return (end);
        end--;
    }
    return (end);
}

void print_left_right(t_token *token)
{
    int left = find_last_pipe(token);
    int i = 0;
    printf("Left part:\n");
    while (i < left)
    {
        printf("%s ", token->token_arr[i]);
        i++;
    }
    printf("\nRight part:\n");
    while (i< token->count_tokens)
    {
        printf("%s ", token->token_arr[i]);
        i++;
    }
    
}
int main(void)
{
    t_token *token = tokenize("echo `hello world`");
        // int i = find_last_pipe(token);
        // printf("last pipe %d\n", i);
    print_left_right(token);
    free_token(token);
    return 0;
}



