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

void start_execve(char **ss, t_env *my_env)
{
    char **envp = t_env_to_envp(my_env);
    char *path = get_path_from_env(my_env);
    char **paths = ft_split(path, ':');
    
    for (int i = 0; paths[i]; i++) {
        char *full_path = ft_strjoin_with_slash(paths[i], *ss);
        if (access(full_path, X_OK) == 0) {
            printf("%s\n", full_path);
            pid_t pid = fork();
            if (pid == 0) {
                execve(full_path, ss, envp);
                perror("execve"); 
                exit(1);
            } else if (pid > 0) {

                int status;
                waitpid(pid, &status, 0);
            } else {
                perror("fork");
            }
            free(full_path);
            break;
        }
        free(full_path);
    }
    free_envp(envp);
    ft_free_array(paths);
}
