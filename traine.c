#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void segfault_handler(int sig) {
    printf("⚠️ Пойман SIGSEGV (%d). Ошибка сегментации!\n", sig);
    exit(1); // лучше завершить работу безопасно
}

int main() {
    signal(SIGSEGV, segfault_handler);

    int *ptr = NULL;
    *ptr = 5; // это вызовет SIGSEGV

    return 0;
}
