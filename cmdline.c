#include <stdio.h>
#include <string.h>
#include <errno.h>


static const int argc;
static const char **argv;

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
        char *next_key = argv[i] + 1;
        if(next_key[0] == '-') next_key++;
        strtok(next_key, "=");
        if(strcmp(key, next_key)) return next_key + strlen(key) + 1;
    }
    return NULL;
}