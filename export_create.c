#include "minishell.h"

int check_key_in_env(t_env *my_env, char *s)
{
    t_env *tmp = my_env;
    while(tmp)
    {
        if(!ft_strcmp(tmp->key, s))
            return (1);
        tmp = tmp->next;
    }
    return 0;
}
void export_create(t_env *my_env, char *s)
{

    if(!incorect_input_for_key(s))
    {
        char *key = split_by_equal_key(s);
        if(check_key_in_env(my_env, key))
        {
            export_add(my_env, s);
        }
        else if(!check_key_in_env(my_env, s))
        {
            t_env *tmp = malloc(sizeof(t_env));
            if(!tmp)
                return;
            tmp->key = ft_strdup(key);
            tmp->value = ft_strdup("");
            tmp->next = NULL;
            t_env *current = my_env;
            while (current->next)
            {
                current = current->next;
            }
            current->next =tmp;
            export_add(my_env, s);
        }
        free(key);
        }
}

void unset(t_env **my_env, char *s)
{
    t_env *current = *my_env;
    t_env *prev = NULL;

    while (current)
    {
        if (!ft_strcmp(current->key, s))
        {
            if (prev)
                prev->next = current->next;
            else
                *my_env = current->next;

            free(current->key);
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}
