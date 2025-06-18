/**
 * @file    stack.h
 * @brief   Header file for `stack.c`.
 *
 * @author  heapbadger
 */

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "array.h"

typedef enum
{
    STACK_SUCCESS            = 0,  /**< Operation succeeded. */
    STACK_NOT_FOUND          = -1, /**< Element not found. */
    STACK_OUT_OF_BOUNDS      = -2, /**< Index out of range. */
    STACK_INVALID_ARGUMENT   = -3, /**< Invalid argument provided. */
    STACK_ALLOCATION_FAILURE = -4, /**< Memory allocation failed. */
    STACK_EMPTY              = -5, /**< Empty stack. */
    STACK_FAILURE            = -6, /**< Generic failure. */

} stack_error_code_t;

typedef struct
{
    array_t *p_array;
} stack_t;

/**
 * @brief Creates a new stack with an initial capacity.
 *
 * @param cap      Initial size of the underlying array.
 * @param del_f    Delete function for element cleanup.
 * @param cmp_f    Comparison function for element matching.
 * @param print_f  Print function for element output.
 * @param cpy_f    Copy function for deep copying.
 *
 * @return Pointer to new stack or NULL on allocation failure.
 */
stack_t *stack_create(size_t           cap,
                      const del_func   del_f,
                      const cmp_func   cmp_f,
                      const print_func print_f,
                      const copy_func  cpy_f);

/**
 * @brief Frees all memory used by the stack and its elements.
 *
 * @param p_stack Pointer to the stack.
 */
void stack_destroy(stack_t *p_stack);

/**
 * @brief Removes all elements from the stack.
 *
 * @param p_stack Stack to clear.
 *
 * @return STACK_SUCCESS on success, error code otherwise.
 */
stack_error_code_t stack_clear(stack_t *p_stack);

/**
 * @brief Fill the entire stack with copies of the given value.
 *
 * @param p_array Pointer to the stack to fill.
 * @param p_value Pointer to the value to copy into each element.
 *
 * @note Caller is responsible for freeing p_value.
 *
 * @return STACK_SUCCESS on success, error code otherwise.
 */
stack_error_code_t stack_fill(stack_t *p_stack, void *p_value);

/**
 * @brief Pushes an element onto the stack.
 *
 * @param p_stack Stack to push into.
 * @param p_data  Pointer to the data.
 *
 * @return STACK_SUCCESS on success, error code otherwise.
 */
stack_error_code_t stack_push(stack_t *p_stack, void *p_data);

/**
 * @brief Removes the top element from the stack.
 *
 * @param p_stack Stack to pop from.
 * @param p_out   Output pointer to receive the top element.

 * @note Caller is responsible for freeing data.
 * @return STACK_SUCCESS on success, error code otherwise.
 */
stack_error_code_t stack_pop(stack_t *p_stack, void **p_out);

/**
 * @brief Retrieves the top element of the stack without removing it.
 *
 * @param p_stack Stack to peek into.
 * @param p_top   Output pointer to receive the data.
 *
 * @return STACK_SUCCESS on success, error code otherwise.
 */
stack_error_code_t stack_peek(const stack_t *p_stack, void **p_top);

/**
 * @brief Checks if the stack is empty.
 *
 * @param p_stack Stack to check.
 *
 * @return true if stack is empty or NULL, false otherwise.
 */
bool stack_is_empty(const stack_t *p_stack);

/**
 * @brief Gets the number of elements currently in the stack.
 *
 * @param p_stack Stack to inspect.
 * @param p_size  Output pointer to receive the size.
 *
 * @return STACK_SUCCESS on success, error code otherwise.
 */
stack_error_code_t stack_size(const stack_t *p_stack, size_t *p_size);

/**
 * @brief Create a deep copy of the stack structure.
 *
 * @param p_ori Pointer to the source stack.
 *
 * @return Pointer to a new stack on success, or NULL on failure.
 */
stack_t *stack_clone(const stack_t *p_ori);

/**
 * @brief Prints all elements in the stack using the registered print function.
 *
 * @param p_stack Stack to print.
 */
void stack_print(const stack_t *p_stack);

#endif // STACK_H

/*** end of file ***/
