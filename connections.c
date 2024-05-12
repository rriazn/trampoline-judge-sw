#include <stdlib.h>
#include <errno.h>
#include "intbuffer.h"
#include "cmdline.h"

static BNDBUF *requestBuf;

static int parseCharToInt(const char *number, const char *paramName);
static int die(char *msg);

int initConnections(int threadCount) {
    int executionj, difficultyj, tof_hdj;
    char *executionj_c = cmdlineGetValueForKey("execution_judges");
    if(NULL == executionj_c) {
        executionj = EXECUTION_JUDGES;
    }
    else {
        executionj = parseCharToInt(executionj_c, "execution_judges");
    }
        
    char *difficultyj_c = cmdlineGetValueForKey("difficulty_judges");
    if(NULL == difficultyj_c) {
        difficultyj = DIFFICULTY_JUDGES;
    }
    else {
        difficultyj = parseCharToInt(difficultyj_c, "difficulty_judges");
    }

    char *tof_hdj_c = cmdlineGetValueForKey("tof_hd_judges");
    if(NULL == tof_hdj_c) {
        tof_hdj = DIFFICULTY_JUDGES;
    }
    else {
        tof_hdj = parseCharToInt(tof_hdj_c, "tof_hd_judges");
    }
    requestBuf = bbCreate(executionj + difficultyj + tof_hdj);
    if(NULL == requestBuf) 
        return -1;
    sigset_t set;
    if(sigemptyset(&set) == -1) 
        return -1;
    if(sigaddset(&set, SIGPIPE) == -1) 
        return -1;
    if(errno = pthread_sigmask(SIG_BLOCK, &set, NULL)) 
        return -1;

    // TODO: Create Threads
    return 0;
}

void handleConnection(int clientSocket) {
    if(NULL != requestBuf) {
        bbPut(requestBuf, clientSock);
    }

}

static int parseCharToInt(const char *number, const char *paramName) {
    errno = 0;
    char *endptr;
    long ret = strtol(number, &endptr, 10);
    if(errno) 
        die("strtol")
    if(ret >= INT_MAX) {
        fprintf(stderr, "Number given in %s too big\n", paramName);
        exit(EXIT_FAILURE);
    }
    if (endptr == number) {
        fprintf(stderr, "No digits were found in %s\n", paramName);
        exit(EXIT_FAILURE);
    }
    if(ret < 1) {
        fprintf(stderr, "%s cannot be less than 1\n", paramName);
        exit(EXIT_FAILURE);
    }
    return (int) ret;
}

static int die(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}