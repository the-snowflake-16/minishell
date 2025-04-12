#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

// Структура для хранения команд
typedef struct s_command {
    char **args;          // Массив аргументов команды
    struct s_command *next; // Указатель на следующую команду (для пайпов)
} t_command;

/* ================== Управление памятью ================== */

// Освобождение памяти команды
void free_command(t_command *cmd) {
    t_command *tmp;
    while (cmd) {
        tmp = cmd->next;
        // Освобождаем аргументы команды
        if (cmd->args) {
            for (int i = 0; cmd->args[i]; i++)
                free(cmd->args[i]); // Освобождаем каждый аргумент
            free(cmd->args);       // Освобождаем массив аргументов
        }
        free(cmd); // Освобождаем саму команду
        cmd = tmp;
    }
}

/* ================== Разбор ввода ================== */

// Разбиение введенной строки на команды и аргументы
t_command *parse_input(char *input) {
    if (!input || !*input) return NULL;

    t_command *head = NULL;  // Голова списка команд
    t_command *current = NULL; // Текущая команда
    char *token = strtok(input, " "); // Разбиваем строку по пробелам
    
    while (token) {
        // Выделяем память для новой команды
        t_command *new_cmd = malloc(sizeof(t_command));
        if (!new_cmd) {
            free_command(head);
            return NULL;
        }
        
        // Выделяем память для аргументов (2 элемента - команда и NULL)
        new_cmd->args = malloc(2 * sizeof(char *));
        if (!new_cmd->args) {
            free(new_cmd);
            free_command(head);
            return NULL;
        }
        
        // Копируем токен в аргументы
        new_cmd->args[0] = strdup(token);
        new_cmd->args[1] = NULL; // Конец массива аргументов
        new_cmd->next = NULL;
        
        // Добавляем команду в список
        if (!head) {
            head = new_cmd;
            current = head;
        } else {
            current->next = new_cmd;
            current = current->next;
        }
        
        token = strtok(NULL, " "); // Следующий токен
    }
    
    return head;
}

/* ================== Встроенные команды ================== */

// Проверка, является ли команда встроенной
int is_builtin(char *cmd) {
    return strcmp(cmd, "exit") == 0 ||  // Выход из shell
           strcmp(cmd, "cd") == 0 ||    // Смена директории
           strcmp(cmd, "echo") == 0;    // Вывод текста
}

// Выполнение встроенной команды
void execute_builtin(t_command *cmd) {
    if (strcmp(cmd->args[0], "exit") == 0) {
        exit(0); // Завершаем программу
    } else if (strcmp(cmd->args[0], "echo") == 0) {
        // Выводим все аргументы echo
        for (int i = 1; cmd->args[i]; i++) {
            printf("%s", cmd->args[i]);
            if (cmd->args[i+1]) printf(" ");
        }
        printf("\n");
    }
    // Можно добавить другие встроенные команды
}

/* ================== Выполнение команд ================== */

void execute_command(t_command *cmd) {
    if (!cmd || !cmd->args || !cmd->args[0]) return;
    
    // Проверяем встроенные команды
    if (is_builtin(cmd->args[0])) {
        execute_builtin(cmd);
        return;
    }
    
    // Для внешних команд создаем дочерний процесс
    pid_t pid = fork();
    if (pid == 0) {
        // Дочерний процесс выполняет команду
        execvp(cmd->args[0], cmd->args);
        perror("minishell"); // Выводим ошибку если execvp не сработал
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Родительский процесс ждет завершения дочернего
        waitpid(pid, NULL, 0);
    } else {
        perror("fork"); // Ошибка создания процесса
    }
}

/* ================== Основной цикл ================== */

void minishell_loop() {
    char *input;
    
    while (1) {
        input = readline("minishell> "); // Чтение ввода
        if (!input) break; // Выход при EOF (Ctrl+D)
        
        if (*input) {
            add_history(input); // Добавляем в историю
            t_command *cmd = parse_input(input);
            if (cmd) {
                execute_command(cmd);
                free_command(cmd); // Не забываем освободить память
            }
        }
        
        free(input); // Освобождаем ввод
    }
}

int main() {
    printf("Minishell - для выхода введите 'exit'\n");
    minishell_loop();
    return 0;
}