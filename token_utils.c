#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

int count_words(char *s)
{
    int count = 0;
    int i = 0;

    if (!s)
        return 0;
    while (s[i])
    {
        while (s[i] == ' ')
            i++;
        if (s[i] == '\'')
        {
            i++;
            while (s[i] != '\'' && s[i] != '\0')
                i++;
            count++;
            i++;
        }
        if (s[i] == '"')
        {
            i++;
            while (s[i] != '"' && s[i] != '\0')
                i++;
            count++;
            i++;
        }
        if (s[i] && s[i] != ' ')
        {
            count++;
            while (s[i] && s[i] != ' ')
                i++;
        }
    }
    return count;
}

char **ft_splitt(char *s)
{
    char **rs;
    int i = 0;
    int word_index = 0;
    int start;
    int len = 0;
    int in_single_quote = 0;
    int in_double_quote = 0;

    rs = malloc(sizeof(char *) * (count_words(s) + 1));
    if (!rs)
        return NULL;

    while (s[i])
    {
        while (s[i] == ' ' && !in_single_quote && !in_double_quote)
            i++;

        if (!s[i])
            break;

        start = i;
        while (s[i])
        {
            if (s[i] == '\'' && !in_double_quote)
            {
                in_single_quote = !in_single_quote;
                i++;
            }
            else if (s[i] == '"' && !in_single_quote)
            {
                in_double_quote = !in_double_quote;
                i++;
            }
            else if (s[i] == ' ' && !in_single_quote && !in_double_quote)
            {
                break;
            }
            else
            {
                i++;
            }
        }

        len = i - start;
        rs[word_index] = malloc(sizeof(char) * (len + 1));
        if (!rs[word_index])
        {
            for (int k = 0; k < word_index; k++)
                free(rs[k]);
            free(rs);
            return NULL;
        }
        for (int j = 0; j < len; j++)
        {
            rs[word_index][j] = s[start + j];
        }
        rs[word_index][len] = '\0';
        word_index++;
    }

    rs[word_index] = NULL;
    return rs;
}
// only for checking don`t delete
//  !!!!!!!!!!!!!!!!!!
// void check_arr_of_token(t_token *token)
// {   

//         for (int i = 0; token->token_arr[i]; i++)
//         {
//             printf("Token %d: %s\n", i, token->token_arr[i]);
//         }
//         printf("Word count: %d\n", token->count_tokens);

// }

