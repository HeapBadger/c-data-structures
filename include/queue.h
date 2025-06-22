/**
 * @file    queue.h
 * @brief   Header for `queue.c`.
 *
 * @author  heapbadger
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include "auxiliary.h"
#include "linked_list.h"

typedef enum
{
    QUEUE_SUCCESS            = 0,  /**< Operation succeeded. */
    QUEUE_NOT_FOUND          = -1, /**< Element not found. */
    QUEUE_OUT_OF_BOUNDS      = -2, /**< Index out of range. */
    QUEUE_INVALID_ARGUMENT   = -3, /**< Invalid argument provided. */
    QUEUE_ALLOCATION_FAILURE = -4, /**< Memory allocation failed. */
    QUEUE_EMPTY              = -5, /**< Empty queue. */
    QUEUE_FAILURE            = -6, /**< Generic failure. */
} queue_error_code_t;

typedef struct
{
    ll_t *p_queue;
} queue_t;

/**
 * @brief Creates a new queue with custom handlers.
 *
 * @param del_f   Custom delete function.
 * @param cmp_f   Custom comparison function.
 * @param print_f Custom print function.
 * @param cpy_f   Custom deep copy function.
 *
 * @return Pointer to new queue, or NULL on failure.
 */
queue_t *queue_create(const del_func   del_f,
                      const cmp_func   cmp_f,
                      const print_func print_f,
                      const copy_func  cpy_f);

/**
 * @brief Free all memory and destroy the queue.
 *
 * @param p_queue Pointer to the queue to destroy.
 */
void queue_destroy(queue_t *p_queue);

/**
 * @brief Deletes a single element using the registered delete function.
 *
 * @param p_queue Pointer to the queue.
 * @param p_value Pointer to the element to delete.
 */
void queue_del_ele(queue_t *p_queue, void *p_value);

/**
 * @brief Insert an element at the end of the queue.
 *
 * @param p_queue Pointer to the queue.
 * @param p_value Pointer to the value to insert.
 *
 * @return QUEUE_SUCCESS on success, appropriate error code otherwise.
 */
queue_error_code_t queue_enqueue(queue_t *p_queue, void *p_data);

/**
 * @brief Remove and return the front element.
 *
 * @param p_queue Pointer to the queue.
 * @param p_data  Output parameter to store the front element's data.
 *
 * @return QUEUE_SUCCESS on success, appropriate error code otherwise.
 */
queue_error_code_t queue_dequeue(queue_t *p_queue, void **p_data);

/**
 * @brief Return the top element (first in queue) without removing it.
 *
 * @param p_queue Pointer to the queue.
 * @param p_top   Output parameter to store the front element's data.
 *
 * @return QUEUE_SUCCESS on success, appropriate error code otherwise.
 */
queue_error_code_t queue_peek(const queue_t *p_queue, void **p_top);

/**
 * @brief Check whether the queue is empty.
 *
 * @param p_queue Pointer to the queue.
 *
 * @return true if empty, false otherwise.
 */
bool queue_is_empty(const queue_t *p_queue);

/**
 * @brief Prints all elements in the queue using the registered print function.
 *
 * @param p_queue Point to the queue to print.
 */
void queue_print(const queue_t *p_queue);

/**
 * @brief Get the number of elements in the queue.
 *
 * @param p_queue Pointer to the queue.
 * @param p_size Output parameter to store the queue's current size.
 *
 * @return QUEUE_SUCCESS on success, appropriate error code otherwise.
 */
queue_error_code_t queue_size(const queue_t *p_queue, size_t *p_size);

/**
 * @brief Create a deep copy of the queue structure.
 *
 * @param p_ori Pointer to the source queue.
 *
 * @return Pointer to a new queue on success, or NULL on failure.
 */
queue_t *queue_clone(const queue_t *p_ori);

#endif // QUEUE_H

/*** end of file ***/
