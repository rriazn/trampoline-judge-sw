#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

#include "cmdline.h"
#include "intbuffer.c"

#define EXECUTION_JUDGES 6
#define DIFFICULTY_JUDGES 1
#define TOF_HD_JUDGES 1
#define PORT 6471


static int parseCharToInt(const char *number, const char *paramName);
static void startThreads(int executionj, int difficultyj, int tof_hdj);
static void acceptClients(int port);
static void die(char *msg);


int main(int argc, char **argv) {
    // evaluate and parse arguments
    cmdlineInit(argc, argv);
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

    startThreads(executionj, difficultyj, tof_hdj);

    int port = PORT;
    char *port_c = cmdlineGetValueForKey("port");
    if(NULL != port) {
        port = parseCharToInt(port_c, "port");
    }
    

    acceptClients(port);
}

static void die(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

static int parseCharToInt(const char *number, const char *paramName) {
    errno = 0;
    char *endptr;
    long ret = strtol(number, &endptr, 10);
    if(errno) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }
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

static void startThreads(int executionj, int difficultyj, int tof_hdj) {

}

static void acceptClients(int port) {
    int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if(-1 == sockfd) die("socket");
     if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1) die("setsockopt");
    struct sockaddr_in6 name = {
        .sin6_family = AF_INET6,
        .sin6_port = htons(port),
        .sin6_addr = in6addr_any,
    };
    if(bind(sockfd, (struct sockaddr *) &name, sizeof(name)) == -1) die("bind");
    if(listen(sockfd, SOMAXCONN) == -1) die("listen");

    // wait for connections:
    while(1) {
        int client = accept(sockfd, NULL, NULL);
        if(-1 == client) {
            perror("accept");
            continue;
        }
        bbPut(client);
    }
}