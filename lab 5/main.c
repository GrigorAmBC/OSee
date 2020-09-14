#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define THREAD_CANCELING_SUCCESS 0
#define THREAD_CREATION_SUCCESS 0
#define THREAD_JOIN_SUCCESS 0
#define BUFFER_SIZE 256
#define EXIT_ERROR 1
#define NOT_CANCELLED 1
#define DONT_EXECUTE_AFTER_POP 0
#define SUSPEND_TIME 2


void printError(int code, char **argv) {
    char buf[BUFFER_SIZE];
    strerror_r(code, buf, sizeof buf);
    printf("%s: %s\n", argv[0], buf);
}

void cancel_handler(void * arg) {
    printf("I'm child's cancellation cleanup handler.\n");
}

void* thread_body(void * param) {
    pthread_cleanup_push(cancel_handler, NULL);

    while (NOT_CANCELLED){
        write(STDOUT_FILENO, "Child\n", 6);
    }

    pthread_cleanup_pop(DONT_EXECUTE_AFTER_POP);
}

int main(int argc, char *argv[]) {
    pthread_t thread;
    int code;

    code = pthread_create(&thread, NULL, thread_body, NULL);
    if (code != THREAD_CREATION_SUCCESS) {
        printError(code, argv);
        exit(EXIT_ERROR);
    }

    sleep(SUSPEND_TIME);

    code = pthread_cancel(thread);
    if (code != THREAD_CANCELING_SUCCESS) {
        printError(code, argv);
        exit(EXIT_ERROR);
    }

	code = pthread_join(thread, NULL);
    if (code != THREAD_JOIN_SUCCESS) {
        printError(code, argv);
        exit(EXIT_ERROR);
    }
	
    printf("Canceled.\n");

    pthread_exit(NULL);
}