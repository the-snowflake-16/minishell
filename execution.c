/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fortytwo <fortytwo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 12:53:25 by fortytwo          #+#    #+#             */
/*   Updated: 2025/06/08 07:04:38 by fortytwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
int redirect_append(const char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd < 0)
    {
        perror("open append");
        return 1;
    }
    if (dup2(fd, STDOUT_FILENO) < 0)
    {
        perror("dup2 append");
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}
// volatile sig_atomic_t g_heredoc_interrupted = 0;

// #include <signal.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <readline/readline.h>

volatile sig_atomic_t g_heredoc_interrupted = 0;

void heredoc_sigint_handler(int sig)
{
    (void)sig;
    g_heredoc_interrupted = 1;
    // write(1, "\n", 1);
    // rl_replace_line("", 0);
    // rl_redisplay();
    rl_done = 1; // Прерывает readline
}

int heredok(const char *delimiter)
{
    int pipe_fd[2];
    pid_t pid;

    if (pipe(pipe_fd) == -1)
        return -1;

    pid = fork();
    if (pid == -1)
        return -1;

    if (pid == 0)
    {
        // Child process
        char *line;

        // Устанавливаем обработчик до начала чтения
        signal(SIGINT, heredoc_sigint_handler);
        close(pipe_fd[0]); // не читаем

        while (1)
        {
            line = readline("❄️  ");
            if (!line)
                exit(0); // Ctrl+D

            if (g_heredoc_interrupted)
                exit(1); // Ctrl-C

            if (!strcmp(line, delimiter))
            {
                free(line);
                exit(0);
            }

            write(pipe_fd[1], line, strlen(line));
            write(pipe_fd[1], "\n", 1);
            free(line);
        }
    }
    else
    {
        // Parent process
        int status;
        close(pipe_fd[1]); 
        waitpid(pid, &status, 0);

        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
        {
            close(pipe_fd[0]);
            return -1;
        }

        return pipe_fd[0];
    }
}


int is_parent_builtin(const char *cmd)
{
    if (!cmd)
        return 0;
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
    int exit_status;
    int fd[2];
    int in_fd = STDIN_FILENO;

    while (cmd)
    {
        int has_next = (cmd->next != NULL);

        // Create pipe for next command if needed
        if (has_next && pipe(fd) < 0)
        {
            perror("pipe");
            return;
        }

        // Handle heredoc before forking
        int heredoc_fd = -1;
        if (cmd->redirects && cmd->redirects->is_heredoc)
        {
            heredoc_fd = heredok(cmd->redirects->delimiter);
            free(cmd->redirects->delimiter);
            if (heredoc_fd == -1)
            {
                free(cmd->redirects->delimiter);
                return; // heredok error
            }

        }

        // If parent builtin and last command
        if (cmd->command && is_parent_builtin(cmd->command) && !has_next)
        {
            if (cmd->redirects)
            {
                if (cmd->redirects->is_input && redirect_input(cmd->redirects->file))
                    return;
                if (cmd->redirects->is_output && redirect_output(cmd->redirects->file))
                    return;
                if (cmd->redirects->is_append && redirect_append(cmd->redirects->file))
                    return;
            }
            state->last_exit_code = handle_builtin(cmd, state);
        }
        else
        {
            pid_t pid = fork();
            if (pid < 0)
            {
                perror("fork");
                if (heredoc_fd != -1)
                    close(heredoc_fd);
                return;
            }
            else if (pid == 0)
            {
                // Child

                // If heredoc, redirect stdin from heredoc pipe
                if (heredoc_fd != -1)
                {
                    dup2(heredoc_fd, STDIN_FILENO);
                    close(heredoc_fd);
                }
                else if (in_fd != STDIN_FILENO)
                {
                    dup2(in_fd, STDIN_FILENO);
                }

                if (has_next)
                    dup2(fd[1], STDOUT_FILENO);

                if (cmd->redirects)
                {
                    if (cmd->redirects->is_input && redirect_input(cmd->redirects->file))
                        exit(1);
                    if (cmd->redirects->is_output && redirect_output(cmd->redirects->file))
                        exit(1);
                    if (cmd->redirects->is_append && redirect_append(cmd->redirects->file))
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
                waitpid(pid, &exit_status, 0);
                if (WIFEXITED(exit_status))
                    state->last_exit_code = WEXITSTATUS(exit_status);

                if (heredoc_fd != -1)
                    close(heredoc_fd);

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
