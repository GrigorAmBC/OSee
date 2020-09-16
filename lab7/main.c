#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define THREAD_CREATION_SUCCESS 0
#define THREAD_JOIN_SUCCESS 0
#define BUFFER_SIZE 256
#define EXIT_ERROR 1
#define DECIMAL_CONSTANT 0
#define num_steps 200000000

struct param{
    long long threadNumber;
    double result;
} typedef params;

void printError(int code, char **argv) {
    char buf[BUFFER_SIZE];
    strerror_r(code, buf, sizeof buf);
    printf("%s: %s\n", argv[0], buf);
}

long int threadsAmount;

void* calculate(void* param) {
    double localpi = 0.0;
    long long i = ((params *)param)->threadNumber;
    for (i; i < num_steps ; i += threadsAmount) {
        localpi += 1.0 / (i * 4.0 + 1.0);
        localpi -= 1.0 / (i * 4.0 + 3.0);
    }
    printf("Thread %lld finished, partial sum %.16f\n", ((params *)param)->threadNumber, localpi);
    ((params *)param)->result = localpi;
    return param;
} 

int main(int argc, char** argv) {
    double pi = 0;
    int i, code;
    pthread_t* threads;
    params* param;

    if (argc > 1) {
        char* non_convertible;
        threadsAmount = strtol(argv[1], &non_convertible, DECIMAL_CONSTANT);
        if (*non_convertible != NULL) {
            fprintf(stderr, "thread number argument must be a number\n");
            exit(EXIT_ERROR);
        }
    } else {
        fprintf(stderr, "requires an argument\n");
        exit(EXIT_ERROR);
    }

    if (threadsAmount < 1) { 
        fprintf(stderr, "usage: %s threadnum\n", argv[0]);
        exit(EXIT_ERROR);
    }

    param = malloc(threadsAmount * sizeof(params));
    threads = malloc(threadsAmount * sizeof(pthread_t));				
    for(i = 0; i < threadsAmount; i++) {
        param[i].threadNumber = i;
        code = pthread_create(threads + i, NULL, calculate, (void*)(param + i));
        if (code != THREAD_CREATION_SUCCESS){
            printError(code, argv);
            exit(EXIT_ERROR);
        }
    }

    for(i = 0; i < threadsAmount; i++) {
        code = pthread_join(threads[i], NULL);
        if (code != THREAD_JOIN_SUCCESS) {
            printError(code, argv);
            exit(EXIT_ERROR);
        }
        pi += param[i].result;
    }
    pi *= 4.0;
    printf ("pi = %.16f\n", pi);
	
    free(param);
    free(threads);
	
    pthread_exit(NULL);
}
