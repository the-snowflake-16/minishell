#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int file_fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd < 0)
    {
        perror("open");
        return 1;
    }

    int saved_stdout = dup(file_fd); // Сохраняем оригинальный stdout

    // Перенаправляем stdout в файл
    dup2(file_fd, 0);

    close(file_fd);
    close(saved_stdout);
    return 0;
}
