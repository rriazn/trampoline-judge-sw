/**
 * @file  sem.h
 * @brief Semaphore implementation for the synchronization of POSIX threads.
 */

#ifndef SEM_H
#define SEM_H


typedef struct SEM SEM;

/**
 * @brief Creates a semaphore.
 *
 * In case of an error, all alreadz allocated memory is cleared.
 * Semaphore can be initialized with negative value.
 *
 * @param initVal The initial value of the semaphore.
 * @return Handle for the created semaphore, or @c NULL if an error occurred.
 */
SEM *semCreate(int initVal);

/**
 * @brief Destroys a semaphore and frees all associated resources.
 * @param sem Handle of the semaphore to destroy. If a @c NULL pointer is
 *            passed, the implementation does nothing.
 */
void semDestroy(SEM *sem);

/**
 * @brief P-operation.
 *
 * Decrements the semaphore value if it is greater than 0 or blocks until it is greater
 * than 0.
 *
 * @param sem Handle of the semaphore to decrement.
 */
void P(SEM *sem);

/**
 * @brief V-operation.
 *
 * Increments the semaphore value by 1 and notifies P-operations that are
 * blocked on the semaphore of the change.
 *
 * @param sem Handle of the semaphore to increment.
 */
void V(SEM *sem); 


#endif /* SEM_H */
