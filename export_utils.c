#include "minishell.h"

int check_str_for_export_add_after_equal(char *s)
{
    int i = 0;
    while (s[i])
    {
        if(s[i] == '=' && s[i+1])
        {
            return 1;
        }
        i++;
    }
    return 0;
}