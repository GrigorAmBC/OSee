#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define THREAD_CREATION_SUCCESS 0
#define PRINT_COUNT 10
#define BUFFER_SIZE 256
#define EXIT_ERROR 1

void printLines(const char *str) {
    for(int i = 0; i < PRINT_COUNT; i++)
        printf(str);
}

void* thread_body(void * param) {
    printLines("Child\n");
    return NULL;
}


int main(int argc, char *argv[]) {
    pthread_t thread;
    int code;

    code = pthread_create(&thread, NULL, thread_body, NULL);
    if (code != THREAD_CREATION_SUCCESS) {
        char buf[BUFFER_SIZE];
        strerror_r(code, buf, sizeof buf);
        printf("%s: creating thread: %s\n", argv[0], buf);
        exit(EXIT_ERROR);
    }

    printLines("Parent\n");

    pthread_exit(NULL);
}
