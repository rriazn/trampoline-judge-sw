#include <stdlib.h>
#include <errno.h>
#include <stdatomic.h>
#include <limits.h>
#include "sem.h"


typedef struct BNDBUF {
    size_t size;
    int *buffer;
    int putIndex;
    _Atomic int getIndex;
    // blocks if buffer is full
    SEM *full;
    // blocks if buffer is empty
    SEM *empty;
} BNDBUF;


BNDBUF *bbCreate(size_t size) {
    if(size > INT_MAX) {
	 errno = EINVAL;
	 return NULL;
    }
    // return value
    BNDBUF *ret = (BNDBUF*) malloc(sizeof(BNDBUF));
    if(NULL == ret) return NULL;
    // set struct values
    ret->size = size,
    ret->buffer = (int*) malloc(size * sizeof(int));
    if(NULL == ret->buffer) {
        free(ret);
        return NULL;
    }
    ret->putIndex = 0,
    ret->full = semCreate(size);
    if(NULL == ret->full) {
        free(ret->buffer);
        free(ret);
        return NULL;
    }
    ret->empty = semCreate(0);
    if(NULL == ret->empty) {
        semDestroy(ret->full);
        free(ret->buffer);
        free(ret);
        return NULL;
    }
    atomic_init(&(ret->getIndex), 0);
    return ret;
}

void bbDestroy(BNDBUF *bb) {
    if(NULL == bb) return;
    free(bb->buffer);
    semDestroy(bb->full);
    semDestroy(bb->empty);
    free(bb);
}

void bbPut(BNDBUF *bb, int value) {
    P(bb->full);
    bb->buffer[bb->putIndex] = value;
    bb->putIndex = (bb->putIndex + 1) % bb->size;
    V(bb->empty);
}

int bbGet(BNDBUF *bb) {
    P(bb->empty);
    int ret, expected;
    do {
        expected = atomic_load(&bb->getIndex);
        ret = bb->buffer[expected];
    } while(!atomic_compare_exchange_strong(&bb->getIndex, &expected, (expected + 1) % bb->size));
    V(bb->full);
    return ret;
}
