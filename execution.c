 #include "minishell.h"

 #include <unistd.h>
 #include <fcntl.h>
 #include <stdio.h>
 #include <stdlib.h>
//  void redirect_output(const char *filename) {
//     int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd == -1) {
//         perror("open");
//         exit(1);
//     }

//     if (dup2(fd, STDOUT_FILENO) == -1) {  // redirect stdout
//         perror("dup2");
//         close(fd);
//         exit(1);
//     }

//     close(fd);
// }

// int redirect(const char *file, char **argv) {
//     redirect_output(file);

//     // simulate: echo hello > filee.txt
//     int i = 1;
//     while (argv[i])
//     {
//         write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
//         i++;
//     }
//     return 0;
// }
int redirect_output(const char *file)
{
    int file_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (file_fd < 0)
    {
        perror("open output");
        return 1;
    }
    if (dup2(file_fd, STDOUT_FILENO) < 0)
    {
        perror("dup2 output");
        close(file_fd);
        return 1;
    }
    close(file_fd);
    return 0;
}
int redirect_input(const char *file)
{
    int fd = open(file, O_RDONLY);
    if (fd < 0)
    {
        perror("open input");
        return 1;
    }
    if (dup2(fd, STDIN_FILENO) < 0)
    {
        perror("dup2 input");
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}
int is_parent_builtin(const char *cmd)
{
    return (
        !ft_strcmp(cmd, "cd") ||
        !ft_strcmp(cmd, "export") ||
        !ft_strcmp(cmd, "unset") ||
        !ft_strcmp(cmd, "exit")
        // !ft_strcmp(cmd, "echo")
    );
}

void execute_pipeline(t_command *cmd, t_state *state)
{
    int fd[2];
    int in_fd = STDIN_FILENO;

    while (cmd)
    {
        int has_next = (cmd->next != NULL);

        if (has_next && pipe(fd) < 0)
        {
            perror("pipe");
            return;
        }

        if (is_parent_builtin(cmd->command) && !has_next)
        {
            // Redirection in parent for single built-in (e.g., "cd > out.txt" is weird but possible)
            if (cmd->redirects)
            {
                if (cmd->redirects->is_input && redirect_input(cmd->redirects->file))
                {
                    state->last_exit_code = 1;
                    return;
                }
                if (cmd->redirects->is_output && redirect_output(cmd->redirects->file))
                {
                    state->last_exit_code = 1;
                    return;
                }
            }
            state->last_exit_code = handle_builtin(cmd, state);
        }
        else
        {
            pid_t pid = fork();
            if (pid < 0)
            {
                perror("fork");
                return;
            }
            else if (pid == 0)
            {
                // Child

                if (in_fd != STDIN_FILENO)
                    dup2(in_fd, STDIN_FILENO);

                if (has_next)
                    dup2(fd[1], STDOUT_FILENO);

                if (cmd->redirects)
                {
                    if (cmd->redirects->is_input && redirect_input(cmd->redirects->file))
                        exit(1);
                    if (cmd->redirects->is_output && redirect_output(cmd->redirects->file))
                        exit(1);
                }

                if (has_next)
                    close(fd[0]);
                if (in_fd != STDIN_FILENO)
                    close(in_fd);

                cmp_input(cmd, state);
                exit(state->last_exit_code);
            }
            else
            {
                // Parent
                waitpid(pid, &state->last_exit_code, 0);
                if (has_next)
                    close(fd[1]);
                if (in_fd != STDIN_FILENO)
                    close(in_fd);
                in_fd = has_next ? fd[0] : STDIN_FILENO;
            }
        }

        cmd = cmd->next;
    }
}