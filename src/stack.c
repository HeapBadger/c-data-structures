/**
 * @file stack.c
 * @brief Implementation of a stack (LIFO) data structure using a dynamic array.
 *
 * This stack is backed by a dynamic array, allowing amortized O(1) push and pop
 * operations. The array grows as needed, avoiding manual memory management for
 * clients.
 *
 * Array-based stacks are cache-friendly, fast, and efficient for known or
 * bounded sizes, making them ideal for high-performance scenarios compared to
 * linked list alternatives.
 *
 * @note The stack only takes ownership of an element upon successful insertion.
 *       If insertion fails, the caller must manage (and eventually free) the
 *       memory.
 * 
 * @author  heapbadger
 */

#include <stdlib.h>
#include "stack.h"

/**
 * @brief Convert an array module return code to a stack module error code.
 *
 * @param ret Array error code.
 *
 * @return Corresponding stack_error_code_t.
 */
static stack_error_code_t stack_error_from_array(array_error_code_t ret);

stack_t *
stack_create (size_t           cap,
              const del_func   del_f,
              const cmp_func   cmp_f,
              const print_func print_f,
              const copy_func  cpy_f)
{
    stack_t *p_stack = NULL;
    p_stack          = (stack_t *)calloc(1, sizeof(stack_t));

    if (NULL != p_stack)
    {
        p_stack->p_array = array_create(cap, del_f, cmp_f, print_f, cpy_f);

        if (NULL == p_stack->p_array)
        {
            stack_destroy(p_stack);
            p_stack = NULL;
        }
    }

    return p_stack;
}

void
stack_destroy (stack_t *p_stack)
{
    if (NULL != p_stack)
    {
        array_destroy(p_stack->p_array);
        free(p_stack);
    }
}

void 
stack_del_ele(stack_t *p_stack, void *p_value)
{
    if ((NULL != p_stack) && (NULL != p_value))
    {
        array_del_ele(p_stack->p_array, p_value);
    }
}

stack_error_code_t
stack_push (stack_t *p_stack, void *p_data)
{
    if ((NULL != p_stack) && (NULL != p_data))
    {
        return stack_error_from_array(array_push(p_stack->p_array, p_data));
    }

    return STACK_INVALID_ARGUMENT;
}

stack_error_code_t
stack_pop (stack_t *p_stack, void **p_out)
{
    if ((NULL != p_stack) && (NULL != p_out))
    {
        return stack_error_from_array(array_pop(p_stack->p_array, p_out));
    }

    return STACK_INVALID_ARGUMENT;
}

stack_error_code_t
stack_peek (const stack_t *p_stack, void **p_out)
{
    if ((NULL == p_stack) || (NULL == p_stack->p_array) || (NULL == p_out))
    {
        return STACK_INVALID_ARGUMENT;
    }

    size_t size;
    stack_error_code_t err = stack_size(p_stack, &size);

    if (STACK_SUCCESS != err)
    {
        return err;
    }

    if (size == 0)
    {
        return STACK_EMPTY;
    }

    return stack_error_from_array(array_get(p_stack->p_array, size - 1, p_out));
}

bool
stack_is_empty (const stack_t *p_stack)
{
    if (NULL != p_stack)
    {
        return array_is_empty(p_stack->p_array);
    }

    return true;
}

void
stack_print (const stack_t *p_stack)
{
    if (NULL != p_stack)
    {
        array_print(p_stack->p_array);
    }
}

stack_error_code_t
stack_size (const stack_t *p_stack, size_t *p_size)
{
    if ((NULL != p_stack) && (NULL != p_size))
    {
        return stack_error_from_array(array_size(p_stack->p_array, p_size));
    }

    return STACK_INVALID_ARGUMENT;
}

stack_t *
stack_clone (const stack_t *p_ori)
{
    stack_t *p_new = NULL;

    if (NULL != p_ori)
    {
        p_new = (stack_t *)calloc(1U, sizeof(stack_t));

        if (NULL == p_new)
        {
            return p_new;
        }

        p_new->p_array = array_clone(p_ori->p_array);
    }

    return p_new;
}

static stack_error_code_t
stack_error_from_array (array_error_code_t ret)
{
    switch (ret)
    {
        case ARRAY_SUCCESS:
            return STACK_SUCCESS;
        case ARRAY_NOT_FOUND:
            return STACK_NOT_FOUND;
        case ARRAY_OUT_OF_BOUNDS:
            return STACK_OUT_OF_BOUNDS;
        case ARRAY_INVALID_ARGUMENT:
            return STACK_INVALID_ARGUMENT;
        case ARRAY_ALLOCATION_FAILURE:
            return STACK_ALLOCATION_FAILURE;
        default:
            return STACK_FAILURE;
    }
}

/*** end of file ***/
