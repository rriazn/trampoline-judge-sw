#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


static int argc;
static char **argv;

int cmdlineInit(int argcs, char *argvs[]){
    argc = argcs;
    argv = argvs;
    for(int i = 1; i < argc; i++) {
        if(argv[i][0] != '-' || strchr(argv[i], '=') == NULL) {
            errno = EINVAL;
            return -1;
        }
    }
    return 0;
}

const char *cmdlineGetProgramName(void) {
    return argv[0];
}

const char *cmdlineGetValueForKey(const char key[]) {
    for(int i = 1; i < argc; i++) {
        char *next_key = strdup(argv[i] + 1);
        int j = 2;
        if(next_key[0] == '-') {
            next_key++;
            j++;
        }
        strtok(next_key, "=");
        if(!strcmp(key, next_key)) {
            return argv[i] + strlen(key) + j;
        }
        free(next_key);
    }
    return NULL;
}