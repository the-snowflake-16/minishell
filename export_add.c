#include "minishell.h"

t_env *find_key(t_env *my_env, char *s)
{
    t_env *tmp = my_env;
    while(tmp)
    {
        if(!ft_strcmp(tmp->key, s))
            return (tmp);
        tmp = tmp->next;
    }
    return NULL;

}
int incorect_input_for_key(char *s)
{
    int i = 0;
    while (s[i])
    {
        if(s[i] == '=')
            return 0;
        i++;
    }
    return 1;
}
char *split_by_equal_key(char *s)
{
    int i = 0;
    while(s[i] != '=')
    {
        i++;
    }
    char *res = malloc(sizeof(char) * (i + 1));
    i = 0;
    while (s[i] != '=')
    {
        res[i] = s[i];
        i++;
    }
    res[i] = '\0';
    return (res);
}
char *split_by_equal(char *s)
{
    int count = 0;
    int i = 0;
    int checker = 0;
    while (s[i])
    {
        if(s[i] == '=')
            checker = 1;
        if(checker)
            count++;
        i++;
    }
    char *res = malloc(sizeof(char) * (count));
    i = 0;
    int j = 0;
    checker = 0;
    while (s[i])
    {
        if(s[i] == '=')
        {
            checker = 1;
            i++;
        }

        if(checker)
            res[j++] = s[i];
        i++;
    }
    res[j] = '\0';
    return(res);
}
char *delete_quotes_export(char *s)
{
    int len = ft_strlen(s);
    char *res = malloc(sizeof(char) * (len -1));
    int i = 0;
    int j = 1;
    while (j < len - 1)
    {
        res[i] = s[j];
        i++;
        j++;
    }
    res[i] = '\0';
    return res;
}
void export_add(t_env *my_env, char *s)
{
    if(!incorect_input_for_key(s))
    {
        char *key = split_by_equal_key(s);
        char *word;
        if(check_str_for_export_add_after_equal(s))
        {
            word = split_by_equal(s);
            printf("%s\n", word);
            printf("%zu\n", ft_strlen(word));
            if((word[0] == '\'' || word[0] == '"') && word[0] == word[ft_strlen(word) -1])
            {
                char *tmp = delete_quotes_export(word);
                free(word);
                word = tmp;
            }

        }

        t_env *tmp = NULL;
    
        tmp = find_key(my_env, key);
        // free(tmp->value);
        // tmp->value = ft_strdup(word);
        // free(word);
        while (my_env)
        {
            if(!ft_strcmp(my_env->key, tmp->key))
            {
                printf("%s\n", my_env->key);
                printf("%s\n", my_env->value);
                free(my_env->value);
                if(check_str_for_export_add_after_equal(s))
                {

                    // char *res = split_by_equal()
                    
                    // my_env->key = ft_strdup("GHHG");
                    // free(my_env->value);
                    // fix_word(word);
                    my_env->value = ft_strdup(word);
                    // new_env->value = ft_strdup("some");
                    printf("%s\n", my_env->value);
                }
                else
                    my_env->value = ft_strdup("");
            }
            // new_env = new_env->next;
            // free(tmp);
            my_env = my_env->next;
        }
        free(key);
        if(check_str_for_export_add_after_equal(s))
            free(word);
    }

    // printf("%s\n", tmp->key);
    // printf("%s\n", word);

    // return(new_env);
}