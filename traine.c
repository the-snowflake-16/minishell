#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// char *find_command(const char *cmd)
// {
//     char *path = getenv("PATH");
//     char *dirs = strdup(path);
//     char *dir = strtok(dirs, ":");

//     while (dir)
//     {
//         char full_path[1024];
//         snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
//         if (access(full_path, X_OK) == 0)
//         {
//             free(dirs);
//             return strdup(full_path);
//         }
//         dir = strtok(NULL, ":");
//     }

//     free(dirs);
//     return NULL;
// }

// int main(void)
// {
//     char *command = "ls";
//     char *full_path = find_command(command);
//     if (!full_path)
//     {
//         fprintf(stderr, "Command not found\n");
//         return 1;
//     }

//     char *args[] = {full_path, "-l", NULL};
//     execve(full_path, args, NULL);
//     perror("execve");
//     free(full_path);
//     return 1;
// }

int main()
{
    char *path = "/bin/ls";
    char *argv[] = {"ls", "-la", NULL};
    printf("Before\n");
    if (execve(path, argv, NULL)== -1)
    {
        perror("execve");
    }
    printf("After\n");
}
