/**
 * @file array.c
 * @brief Implementation of the dynamic array data structure.
 *
 * @author  heapbadger
 * @date    June 09, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include "array.h"

array_t *
array_create (size_t           initial_capacity,
              const del_func   del_f,
              const cmp_func   cmp_f,
              const print_func print_f)
{
    array_t *p_array = NULL;

    if ((0 < initial_capacity) && (NULL != del_f) && (NULL != cmp_f) && (NULL != print_f))
    {
        p_array = calloc(1, sizeof(array_t));

        if (NULL != p_array)
        {
            p_array->pp_array = calloc(initial_capacity, sizeof(void *));

            if (NULL != p_array->pp_array)
            {
                p_array->cap     = initial_capacity;
                p_array->len     = 0;
                p_array->del_f   = del_f;
                p_array->cmp_f   = cmp_f;
                p_array->print_f = print_f;
            }
            else
            {
                free(p_array);
                p_array = NULL;
            }
        }
    }

    return p_array;
}

void
array_destroy (array_t *p_array)
{
    if (NULL != p_array)
    {
        array_clear(p_array);
        free(p_array->pp_array);
        free(p_array);
    }

    return;
}

void array_clear(array_t *p_array)
{
    if ((NULL != p_array) && (NULL != p_array->pp_array))
    {
        for (size_t idx = p_array->len; idx-- > 0; )
        {
            array_remove(p_array, idx);
        }
    }

    return;
}

ssize_t array_insert(array_t *p_array, size_t index, void *p_value)
{
    ssize_t ret = ARRAY_SUCCESS;

    if ((NULL == p_array) || (NULL == p_value)) 
    {
        ret = ARRAY_INVALID_ARGUMENT;
    }
    else if (index > p_array->len) // valid index is in [0, len]
    {
        ret = ARRAY_OUT_OF_BOUNDS;
    }
    else 
    {
        // Resize if full
        if (array_is_full(p_array)) 
        {
            ret = array_reserve(p_array, p_array->cap * ARRAY_RESIZE_FACTOR);
        }

        if (ret == ARRAY_SUCCESS) 
        {
            // Shift elements right
            for (size_t idx = p_array->len; idx > index; --idx) 
            {
                p_array->pp_array[idx] = p_array->pp_array[idx - 1];
            }

            // Insert new value
            p_array->pp_array[index] = p_value;
            p_array->len++;
        }
    }

    // Cleanup on failure
    if ((ARRAY_SUCCESS != ret) && (NULL != p_value)) 
    {
        p_array->del_f(p_value);
    }

    return ret;
}

ssize_t array_remove(array_t *p_array, size_t index)
{
    ssize_t ret = ARRAY_SUCCESS;

    if (NULL == p_array)
    {
        ret = ARRAY_INVALID_ARGUMENT;
    }
    else if (index >= p_array->len) // valid index: [0, len - 1]
    {
        ret = ARRAY_OUT_OF_BOUNDS;
    }
    else
    {
        // Delete the item
        p_array->del_f(p_array->pp_array[index]);

        // Shift items left to fill the gap
        for (size_t i = index; i < p_array->len - 1; ++i)
        {
            p_array->pp_array[i] = p_array->pp_array[i + 1];
        }

        p_array->len--;
        p_array->pp_array[p_array->len] = NULL; // null out the last pointer
    }

    return ret;
}

ssize_t array_push(array_t *p_array, void *p_value)
{
    if ((NULL == p_array) || (NULL == p_value))
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    return array_insert(p_array, p_array->len, p_value);
}

ssize_t array_pop(array_t *p_array)
{
    if (NULL == p_array)
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    return array_remove(p_array, p_array->len - 1);
}

ssize_t array_get(const array_t *p_array, size_t index, void **p_out)
{
    if ((NULL == p_array) || (NULL == p_out))
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    if (index >= p_array->len)
    {
        return ARRAY_OUT_OF_BOUNDS;
    }

    *p_out = p_array->pp_array[index];
    return ARRAY_SUCCESS;
}

ssize_t array_set(array_t *p_array, size_t index, void *p_value)
{
    if ((NULL == p_array) || (NULL == p_value))
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    if (index >= p_array->len)
    {
        return ARRAY_OUT_OF_BOUNDS;
    }

    p_array->del_f(p_array->pp_array[index]);
    p_array->pp_array[index] = p_value;
    return ARRAY_SUCCESS;
}


















ssize_t array_find(const array_t *p_array, void *key)
{

    return 0;
}

ssize_t array_size(const array_t *p_array)
{
    if (NULL == p_array)
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    return p_array->len;
}

ssize_t array_capacity(const array_t *p_array)
{
    if (NULL == p_array)
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    return p_array->cap;
}

bool array_is_empty(const array_t *p_array)
{
    return (array_size(p_array) <= 0);
}

bool array_is_full(const array_t *p_array)
{
    return (array_size(p_array) >= array_capacity(p_array));
}

ssize_t array_reserve(array_t *p_array, size_t new_capacity)
{
    printf("\nNeed to resize!\n");
    return ARRAY_NOT_FOUND;
}

ssize_t array_shrink_to_fit(array_t *p_array)
{
    return 0;
}

void array_sort(array_t *p_array)
{
    return;
}

ssize_t array_binary_search(const array_t *p_array, void *key)
{
    return 0;
}

/*** end of file ***/