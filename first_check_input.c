#include "minishell.h"

// int contain_symbols(char *s)
// {
//     int i = 0;
//     while (s[i])
//     {
//         if(s[i] == '<')
//             return 1;
//         else if(s[i] == '>')
//             return 1;
//         else if(s[i] == '|')
//             return 1;
//         i++;
//     }
//     return (0);
// }

// int incorect_symbols(char *s)
// {
//     int i = 0;
//     if(contain_symbols(s))
//     while (s[i])
//     {
//         if (s[i] == '<' && s[i+1] == ' ' && s[i+2] == '<')
//         {
//             printf("Incorect input\n");
//             return(1);
//         }
//         else if (s[i] == '>' && s[i+1] == ' ' && s[i+2] == '>')
//         {
//             printf("Incorect input\n");
//             return(1);
//         }
//         else if(s[i] == '|' && s[i+1] == '|')
//         {
//             printf("Incorect input\n");
//             return(1);
//         }
//         i++;
//     }
//     return (0);
// }

int contain_queter(char *s)
{
    int i = 0;
    while (s[i])
    {
        if(s[i] == '\'')
            return 1;
        else if(s[i] == '"')
            return 1;
        i++;
    }
    return 0;
}
int incorect_quote(char *s)
{
    int quote_count = 0;
    int double_quote = 0;
    for (int i = 0; s[i]; i++)
    {
        if (s[i] == '\'')
            quote_count++;
        else if (s[i] == '"')
        {
            double_quote++;
        }
        
    }
    if (quote_count % 2 != 0 || double_quote % 2 != 0)
    {
        printf("Incorrect input\n");
        return 1;
    }
    return 0;
}

int incorect_input(char *s)
{
    if(contain_queter(s))
        return (incorect_quote(s));
    return (0);
}
