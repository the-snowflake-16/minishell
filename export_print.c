#include "minishell.h"

int count_exported(t_env *my_env)
{
    int count = 0;
    while (my_env != NULL)
    {
        count++;
        my_env = my_env->next;
    }
    return count;
}



// void print_export_sorted(t_env *env)
// {
//     int count = count_exported(env);
//     if (count == 0)
//         return;

//     t_env **env_array = malloc(sizeof(t_env *) * count);
//     if (!env_array)
//         return;

//     int i = 0;
//     t_env *tmp = env;
//     while (tmp)
//     {

//         env_array[i++] = tmp;
//         tmp = tmp->next;
//     }

//     qsort(env_array, count, sizeof(t_env *), compare_env);

//     for (int j = 0; j < count; j++)
//     {
//         if (env_array[j]->value)
//             printf("declare -x %s=\"%s\"\n", env_array[j]->key, env_array[j]->value);
//         else
//             printf("declare -x %s\n", env_array[j]->key);
//     }

//     free(env_array);
// }

int compare_env(const void *a, const void *b)
{
    t_env *env1 = *(t_env **)a;
    t_env *env2 = *(t_env **)b;
    return ft_strcmp(env1->key, env2->key);
}

// void give_index(t_env *my_env)
// {
//     int index = 0;
//     t_env *tmp = my_env;
//     t_env *lower;
//     int lower_int;
//     while(tmp->next != NULL)
//     {
//         lower = tmp;
//         lower_int = ft_strcmp(tmp->key, tmp->next->key);
//         // printf("%d\n", ft_strcmp(tmp->key, tmp->next->key));
//         if(ft_strcmp(tmp->key, tmp->next->key) < lower_int)
//         {
//             lower_int = ft_strcmp(tmp->key, tmp->next->key);
//             lower = tmp;
//             tmp->index = lower_int;
//         }

//         tmp = tmp->next;
//     }

// }
// void sort_env(t_env *my_env)
// {
//     int count = count_exported(my_env);
//     t_env **sorted_env = malloc(sizeof(t_env) * (count) + 1);
//     give_index(my_env);

// }
void assign_sorted_indexes(t_env *env)
{
    int index = 0;
    t_env *current;
    t_env *min_node;
    t_env *tmp;

    current = env;
    while (current)
    {
        current->index = -1;
        current = current->next;
    }

    while (1)
    {
        min_node = NULL;
        tmp = env;
        while (tmp)
        {
            if (tmp->index == -1)
            {
                if (!min_node || ft_strcmp(tmp->key, min_node->key) < 0)
                    min_node = tmp;
            }
            tmp = tmp->next;
        }
        if (!min_node)
            break;
        min_node->index = index;
        // printf("%s %d\n", min_node->key, min_node->index);
        index++;
    }
}

void export_print(t_env *my_env)
{
    assign_sorted_indexes(my_env);
    int i = 0;
    int count = count_exported(my_env);
    t_env *tmp;
    while (i < count)
    {
        tmp = my_env;
        while (tmp)
        {
            if(tmp->index == i)
                printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
            tmp = tmp->next;
        }
        i++;
    }
}
