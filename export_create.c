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
bool is_valid_export_key(const char *s)
{
    int i = 0;

    if (!s || !s[0])
        return false;
    if (!ft_isalpha(s[0]) && s[0] != '_')
        return false;

    while (s[i] && s[i] != '=')
    {
        if (!ft_isalnum(s[i]) && s[i] != '_')
            return false;
        i++;
    }

    return true;
}

int export_create(t_env *my_env, t_command *command)
{
    if(!is_valid_export_key(command->args[1]))
    {
        fprintf(stderr,"%s: not a valid identifier\n", command->args[1]);
            // command->exit_code = 1;
        return 1;       
    }
    if(!incorect_input_for_key(command->args[1]))
    {
        char *key = split_by_equal_key(command->args[1]);
        if (key == NULL)
        {
            fprintf(stderr,"%s: not a valid identifier\n", command->args[1]);
            // command->exit_code = 1;
            return 1;
        }
        if(check_key_in_env(my_env, key))
        {
            export_add(my_env, command);
        }
        else if(!check_key_in_env(my_env, command->args[1]))
        {
            t_env *tmp = malloc(sizeof(t_env));
            if(!tmp)
                return 1;
            tmp->key = ft_strdup(key);
            tmp->value = ft_strdup("");
            tmp->next = NULL;
            t_env *current = my_env;
            while (current->next)
            {
                current = current->next;
            }
            current->next =tmp;
            export_add(my_env, command);
        }
        free(key);
        }
    return 0;
}

int unset(t_env **my_env, char *s)
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
            return 0;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}
