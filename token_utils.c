#include "minishell.h"

int count_words(char *s)
{
    int count = 0;
    int i = 0;

    if (!s)
        return 0;

    while (s[i])
    {
        // Skip any spaces
        while (s[i] == ' ')
            i++;

        // If we find a word
        if (s[i] && s[i] != ' ')
        {
            count++;
            // Skip the word
            while (s[i] && s[i] != ' ')
                i++;
        }
    }
    return count;
}

void **ft_split(char *s)
{
    char **rs = malloc(sizeof(char*) * count_words(s) + 1);
    int i = 0;
    while (s[i] == ' ')
        i++;
    while (s[i])
    {
        if (s[i] == ' ')
        i++;
    }
    
}
// int main(void)
// {
//     char *s = "  he llo   world how are you  ";
//     printf("%d\n", count_words(s)); // Output: 5
//     return 0;
// }
