/**
 * @file  intbuffer.h
 * @brief Bounded-buffer implementation to manage integer values, supporting
 *  	  multiple readers but only a single writer.
 *
 * uses semaphore module (sem.h)
 */

#ifndef INTBUFFER_H
#define INTBUFFER_H

#include <stdlib.h>


typedef struct BNDBUF BNDBUF;

/**
 * @brief Creates a new bounded buffer.
 *
 * In case of an error, all already allocated resources are freed.
 *
 * @param size The number of integers that can be stored in the bounded buffer.
 * @return Handle for the created bounded buffer, or @c NULL if an error
 *         occurred.
 */
BNDBUF *bbCreate(size_t size);

/**
 * @brief Destroys a bounded buffer.
 *
 * All resources associated with the bounded buffer are released.
 *
 * @param bb Handle of the bounded buffer that shall be freed. If a @c NULL
 *           pointer is passed, the implementation does nothing.
 */
void bbDestroy(BNDBUF *bb);

/**
 * @brief Adds an element to a bounded buffer.
 *
 * This function adds an element to a bounded buffer. If the buffer is full, the
 * function blocks until an element has been removed from it.
 *
 * @param bb    Handle of the bounded buffer.
 * @param value Value that shall be added to the buffer.
 */
void bbPut(BNDBUF *bb, int value);

/**
 * @brief Retrieves an element from a bounded buffer.
 *
 * This function removes an element from a bounded buffer. If the buffer is
 * empty, the function blocks until an element has been added.
 *
 * @param bb Handle of the bounded buffer.
 * @return The integer element.
 */
int bbGet(BNDBUF *bb);


#endif /* INTBUFFER_H */
