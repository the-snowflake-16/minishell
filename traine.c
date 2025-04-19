#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    char input[1024];
    char cwd[1024];

    while (1) {
        // Печатаем текущий каталог
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s> \n", cwd);
        } else {
            perror("getcwd");
            return 1;
        }

        // Читаем команду
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // Удаляем '\n'

        // Если команда начинается с "cd "
        if (strncmp(input, "cd ", 3) == 0) {
            char *path = input + 3;
            if (chdir(path) != 0) {
                perror("cd error");
            }
        } else if (strcmp(input, "exit") == 0) {
            break;
        } else {
            printf("Неизвестная команда: %s\n", input);
        }
    }

    return 0;
}
