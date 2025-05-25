#include "minishell.h"

char **t_env_to_envp(t_env *env)
{
    int count = 0;
    t_env *tmp = env;

    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }

    char **envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return NULL;

    tmp = env;
    int i = 0;

    while (tmp)
    {
        char *key_equal = ft_strjoin(tmp->key, "=");
        char *full_str = ft_strjoin(key_equal, tmp->value);
        free(key_equal);
        envp[i] = full_str;
        i++;
        tmp = tmp->next;
    }
    envp[i] = NULL;

    return envp;
}

void free_envp(char **envp)
{
    if (!envp)
        return;

    int i = 0;
    while (envp[i])
    {
        free(envp[i]);
        i++;
    }
    free(envp);
}

char *get_path_from_env(t_env *env)
{
    while (env)
    {
        if (ft_strcmp(env->key, "PATH") == 0)
            return env->value;
        env = env->next;
    }
    return NULL;
}
char *ft_strjoin_with_slash(const char *dir, const char *command)
{

    if (!dir || !command)
        return NULL;
    
    size_t len_dir = strlen(dir);
    size_t len_command = strlen(command);
    
 
    char *full_path = (char *)malloc(len_dir + len_command + 2); 
    
    if (!full_path)
        return NULL;
    

    strcpy(full_path, dir);
    

    full_path[len_dir] = '/';
    strcpy(full_path + len_dir + 1, command);
    
    return full_path;
}

void ft_free_array(char **arr)
{
    if (!arr)
        return;

    int i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}
int start_execve(char **args, t_state *state)
{
    char **envp = t_env_to_envp(state->env);
    int status = 127; // Default to "command not found"

    // If command is a direct path (contains '/')
    if (ft_strchr(args[0], '/'))
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            execve(args[0], args, envp);
            perror("execve");
            exit(EXIT_FAILURE); // Exit from child on failure
        }
        else if (pid > 0)
        {
            waitpid(pid, &status, 0);
        }
        else
        {
            perror("fork");
        }
        free_envp(envp);
        return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    }

    // Otherwise, search in PATH
    char *path_var = get_path_from_env(state->env);
    if (!path_var)
    {
        fprintf(stderr, "PATH variable not found\n");
        free_envp(envp);
        return status;
    }

    char **paths = ft_split(path_var, ':');
    bool executed = false;

    for (int i = 0; paths[i]; i++)
    {
        char *full_path = ft_strjoin_with_slash(paths[i], args[0]);
        if (access(full_path, X_OK) == 0)
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                execve(full_path, args, envp);
                perror("execve");
                exit(EXIT_FAILURE);
            }
            else if (pid > 0)
            {
                waitpid(pid, &status, 0);
            }
            else
            {
                perror("fork");
            }
            executed = true;
            free(full_path);
            break;
        }
        free(full_path);
    }

    if (!executed)
        fprintf(stderr, "minishell: command not found: %s\n", args[0]);
        

    ft_free_array(paths);
    free_envp(envp);

    return (state->last_exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : 1);
}
