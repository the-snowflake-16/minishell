#include "minishell.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void free_env(t_env *env)
{
    t_env *tmp;
    while (env != NULL)
    {
        // printf("free env");
        tmp = env->next;
        free(env->key);
        free(env->value);
        free(env);
        env = tmp;
    }
   }
   void ft_split_equal(char *s, t_env *env)
   {
    int i = 0;
    while (s[i] != '=' && s[i] != '\0')
    	i++;
    env->key = ft_strndup(s, i);
    if (s[i] == '=')
    	env->value = ft_strdup(s + i + 1);
    else
    	env->value = ft_strdup("");
   }

t_env *add_left_right(char *s)
{
    t_env *new_env = malloc(sizeof(t_env));
    if (!new_env)
        return NULL;
    ft_split_equal(s, new_env);
    new_env->exported = 1;
    new_env->next = NULL;
    return new_env;
}

t_env *init_env(char **s)
{
    t_env *head = NULL;
    t_env *current = NULL;
    int i = 0;
    
    while (s[i])
    {
        t_env *new_node = add_left_right(s[i]);
        if (!new_node)
        {
            free_env(head);
            return NULL;
        }
        
        if (!head)
        {
            head = new_node;
            current = head;
        }
        else
        {
            current->next = new_node;
            current = current->next;
        }
        i++;
    }
    return head;
}

// void init_export_for_env(t_env *my_env)
// {

// }
void print_env(t_env *my_env)
{
    if(my_env)
        while (my_env != NULL)
        {
            printf("%s=%s\n", my_env->key, my_env->value);
            my_env = my_env->next;
        }
}



// int main()
// {
//     char *env_array[] = {
//         "SYSTEMD_EXEC_PID=111676",
//         "DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/101623/bus",
//         "LIBVIRT_DEFAULT_URI=qemu:///system",
//         NULL
//     };
    
//     t_env *env = init_env(env_array);  // No malloc here - init_env creates the list
//     print_env(env);
//     free_env(env);
    
//     return 0;
// }
// void check_input(int argc, char **argv, char **env)
// {
//     if(argc == 3 && strcmp(argv[1], "-c") == 0)
//     {
//         cmp_input(argv[2], env);
//         exit(0);
//     }
// }

// int main(int argc, char *argv[], char **env)
// {
//     // check_input(argc, argv, env);
//     t_env *my_env = init_env(env);
//     print_env(my_env);    
//     free_env(my_env);
// }