#include <pthread.h>
#include <stdlib.h>
#include <errno.h>


typedef struct SEM {
    volatile int value;
    pthread_cond_t condition;
    pthread_mutex_t mutex;
} SEM;

SEM *semCreate(int initVal) {
    SEM *ret = (SEM*) malloc(sizeof(SEM));
    if(NULL == ret) return NULL;
    ret->value = initVal;
    errno = 0;
    errno = pthread_mutex_init(&ret->mutex, NULL);
    if(errno) {
        free(ret);
        return NULL;
    }
    errno = pthread_cond_init(&ret->condition, NULL);
    if(errno) {
        pthread_mutex_destroy(&ret->mutex);
        free(ret);
        return NULL;
    }
    return ret;
}

void semDestroy(SEM *sem) {
    if(NULL == sem) return;
    errno = 0;
    errno = pthread_mutex_destroy(&sem->mutex);
    int e = errno;
    errno = pthread_cond_destroy(&sem->condition);
    if(e != 0 && errno == 0) errno = e; 
    free(sem);
}

void P(SEM *sem) {
    pthread_mutex_lock(&sem->mutex);
    while(sem->value <= 0) {
        pthread_cond_wait(&sem->condition, &sem->mutex);
    }
    sem->value--;
    pthread_mutex_unlock(&sem->mutex);
}

void V(SEM *sem) {
    pthread_mutex_lock(&sem->mutex);
    sem->value++;
    pthread_cond_signal(&sem->condition);
    pthread_mutex_unlock(&sem->mutex);
}