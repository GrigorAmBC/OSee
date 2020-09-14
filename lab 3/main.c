#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define THREAD_COUNT 4
#define THREAD_CREATION_SUCCESS 0
#define BUFFER_SIZE 256
#define EXIT_ERROR 1

void *thread_body(void *param) {
    for (char ** t = (char **)param; *t != NULL; t++)
        printf("%s\n", *t);

    return NULL;
}

void printError(int code, char **argv) {
    char buf[BUFFER_SIZE];
    strerror_r(code, buf, sizeof buf);
    printf("%s: creating thread: %s\n", argv[0], buf);
}

int main(int argc, char *argv[]) {
    pthread_t threads[THREAD_COUNT];
    int code;
    static char* params[THREAD_COUNT][BUFFER_SIZE] = {
        {"s11", "s12", "s13", NULL},
        {"s21", "s22", "s23", "s24", NULL},
        {"s31", "s32", "s33", NULL},
        {"s41", "s42", NULL}
    };

    for (int i = 0; i < THREAD_COUNT; i++) {
        code = pthread_create(&threads[i], NULL, thread_body, params[i]);
        if (code != THREAD_CREATION_SUCCESS) {
            printError(code, argv);
            exit(EXIT_ERROR);
        }
    }

    pthread_exit(NULL);
}

