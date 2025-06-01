#include "minishell.h"

void sigint_handler(int signo)
{
    (void)signo; // unused param

    // Print a newline, reset prompt with readline
    write(1, "\n", 1);
    // This tells readline to clear current input and redisplay prompt:
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void sigquit_handler(int signo)
{
    (void)signo;
    // Ignore or print message
    write(1, "Quit\n", 5);
}

// int main(void)
// {
//     signal(SIGINT, sigint_handler);
//     signal(SIGQUIT, SIG_IGN);

//     while (1)
//     {
//         char *line = readline("minishell❄️   ");
//         if (!line)
//             break;

//         if (*line)
//             add_history(line);

//         // Do your parsing and execution here

//         free(line);
//     }

//     return 0;
// }
