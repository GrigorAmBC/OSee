#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define THREAD_CREATION_SUCCESS 0
#define THREAD_JOIN_SUCCESS 0
#define EXIT_ERROR 1
#define DECIMAL_CONSTANT 0
#define num_steps 200000000
#define THREAD_NUMBER_TOP_LIMIT 300
#define NO_ERROR 0

struct param{
    long long threadNumber;
    double result;
    int threadsAmount;
} typedef params;

void printError(int code, char* error) {
    char* buf = strerror(code);
    printf("%s: %s\n", error, buf);
}

void* calculate(void* arg) {
    double localpi = 0.0;
    long long i;
    params* param = (params *) arg;

    for (i = param->threadNumber; i < num_steps ; i += param->threadsAmount) {
        localpi += 1.0 / (i * 4.0 + 1.0);
        localpi -= 1.0 / (i * 4.0 + 3.0);
    }

    param->result = localpi;
    return param;
}

int parseArguments(int argc, char **argv) {
    char* ret;
    int numOfThreads;

    if (argc != 2) {
        printf("Usage %s [number of threads] format\n", argv[0]);
        exit(EXIT_ERROR);
    }

    numOfThreads = strtol(argv[1], &ret, DECIMAL_CONSTANT);
    if (errno != NO_ERROR) {
        printError(errno, "Failed to convert argument to number");
        exit(EXIT_ERROR);
    }

    if (ret[0] != '\0') {
        printf("Only number available as argument\n");
        exit(EXIT_ERROR);
    }

    if (numOfThreads < 1) {
        printf("Invalid number of threads: %d\n", numOfThreads);
        exit(EXIT_ERROR);
    }

    return numOfThreads;
}

int main(int argc, char** argv) {
    double pi = 0;
    int i, code;
    long int threadsAmount;
    pthread_t* threads;
    params* param;

    threadsAmount = parseArguments(argc, argv);

    threads = (pthread_t*) malloc(threadsAmount * sizeof(pthread_t));
	if (threads == NULL) {
        printError(errno, "Failed to alloc memory for threads");
        exit(EXIT_ERROR);
    }

    param = (params*) malloc(threadsAmount * sizeof(params));
    if (param == NULL) {
        printError(errno, "Failed to alloc memory for args");
        exit(EXIT_ERROR);
    }

    for(i = 0; i < threadsAmount; i++) {
        param[i].threadNumber = i;
		param[i].threadsAmount = threadsAmount;
        code = pthread_create(threads + i, NULL, calculate, &param[i]);
        if (code != THREAD_CREATION_SUCCESS){
            printError(code, "Failed to create thread");
            exit(EXIT_ERROR);
        }
    }

    for(i = 0; i < threadsAmount; i++) {
        code = pthread_join(threads[i], NULL);
        if (code != THREAD_JOIN_SUCCESS) {
            printError(code, argv[0]);
            exit(EXIT_ERROR);
        }
        pi += param[i].result;
    }
    pi *= 4.0;
    printf("pi = %.16f\n", pi);

    free(param);
    free(threads);

    pthread_exit(NULL);
}
