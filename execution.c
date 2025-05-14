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
int redirect_output(const char *file, char **argv)
{
    int file_fd = open(file, O_WRONLY | O_CREAT| O_TRUNC, 0666);
    if(file_fd < 0)
    {
        perror("open");
        return 1;
    }
    // int saved_stdout = dup(file_fd);
    dup2(file_fd, 1);
    int i = 1;
    while (argv[i])
    {
        write(1, argv[i], ft_strlen(argv[i]));
        i++;
    }
    close(file_fd);
    // close(saved_stdout);
    return 0;
}
int redirect_input(const char *file)
{
    int fd = open(file, O_RDONLY);
    if (fd < 0)
    {
        perror("open input");
        exit(1);
    }
    dup2(fd, 0);
    close(fd);
    return 0;
}

void execute_pipeline(t_command *cmd, t_env *my_env)
{
    int fd[2];
    int in = STDIN_FILENO;
    pid_t pid;

    while (cmd)
    {
        pipe(fd); // создаём пайп

        pid = fork();
        if (pid == 0)
        {
            // --- Дочерний процесс ---

            // 1. Редирект входа
            if (cmd->redirects && cmd->redirects->is_input)
            {
                // printf("%s\n", cmd->redirects->file);
                redirect_input(cmd->redirects->file);

            }

            // else if (in != STDIN_FILENO)
            //     dup2(in, STDIN_FILENO);

            // 2. Редирект вывода
            if (cmd->redirects && cmd->redirects->is_output)
                redirect_output(cmd->redirects->file, cmd->args);
            else if (cmd->next)
                dup2(fd[1], STDOUT_FILENO);

            // 3. Закрываем лишние дескрипторы
            close(fd[0]);
            close(fd[1]);

            // 4. Выполняем команду
            cmp_inpu(cmd, my_env);
            exit(1); // exec не должен возвращать
        }

        // --- Родительский процесс ---

        waitpid(pid, NULL, 0); // можно вынести за пределы цикла, если нужен параллелизм
        close(fd[1]);
        if (in != STDIN_FILENO)
            close(in);
        in = fd[0]; // передаём выход текущего процесса как вход для следующего
        cmd = cmd->next;
    }
}



