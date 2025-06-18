
/**
 * @file array.c
 * @brief Implementation of the dynamic array data structure.
 *
 * @author  heapbadger
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

/**
 * @brief Ensure the array has at least the specified capacity.
 *
 * @param p_array Pointer to the array.
 * @param new_cap The new minimum capacity required.
 *
 * @return ARRAY_SUCCESS on success, appropriate error code otherwise.
 */
static array_error_code_t array_reserve(array_t *p_array, size_t new_cap);

/**
 * @brief Reduce the array's capacity to better fit its current size.
 *
 * @param p_array Pointer to the array.
 *
 * @return ARRAY_SUCCESS on success, appropriate error code otherwise.
 */
static array_error_code_t array_shrink_to_fit(array_t *p_array);

array_t *
array_create (size_t           initial_capacity,
              const del_func   del_f,
              const cmp_func   cmp_f,
              const print_func print_f,
              const copy_func  cpy_f)
{
    array_t *p_array = NULL;

    if ((0U < initial_capacity) && (NULL != del_f) && (NULL != cmp_f)
        && (NULL != print_f) && (NULL != cpy_f))
    {
        p_array = (array_t *)calloc(1, sizeof(array_t));

        if (NULL != p_array)
        {
            p_array->pp_array
                = (void **)calloc(initial_capacity, sizeof(void *));

            if (NULL != p_array->pp_array)
            {
                p_array->cap     = initial_capacity;
                p_array->len     = 0U;
                p_array->del_f   = del_f;
                p_array->cmp_f   = cmp_f;
                p_array->print_f = print_f;
                p_array->cpy_f   = cpy_f;
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
        if (NULL != p_array->pp_array)
        {
            array_clear(p_array);
            free(p_array->pp_array);
        }

        free(p_array);
    }
}

void
array_clear (array_t *p_array)
{
    if ((NULL != p_array) && (NULL != p_array->pp_array))
    {
        for (size_t idx = 0U; idx < p_array->len; ++idx)
        {
            array_delete_element(p_array, p_array->pp_array[idx]);
            p_array->pp_array[idx] = NULL;
        }

        p_array->len = 0U;
    }
}

void
array_delete_element (array_t *p_array, void *p_value)
{
    if ((NULL != p_array) && (NULL != p_value))
    {
        p_array->del_f(p_value);
    }
}

array_error_code_t
array_fill (array_t *p_array, void *p_value)
{
    if ((NULL == p_array) || (NULL == p_value) || (NULL == p_array->cpy_f))
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    array_error_code_t ret = ARRAY_SUCCESS;

    for (size_t idx = 0U; idx < p_array->cap; ++idx)
    {
        void *p_copy = p_array->cpy_f(p_value);

        if (NULL == p_copy)
        {
            ret = ARRAY_ALLOCATION_FAILURE;
            break;
        }

        ret = array_push(p_array, p_copy);

        if (ARRAY_SUCCESS != ret)
        {
            break;
        }
    }

    if (ARRAY_SUCCESS != ret)
    {
        array_clear(p_array);
    }

    return ret;
}

array_error_code_t
array_insert (array_t *p_array, size_t index, void *p_value)
{
    array_error_code_t ret = ARRAY_SUCCESS;

    if ((NULL == p_array) || (NULL == p_value))
    {
        ret = ARRAY_INVALID_ARGUMENT;
        goto EXIT;
    }

    if (index > p_array->len)
    {
        array_delete_element(p_array, p_value);
        ret = ARRAY_OUT_OF_BOUNDS;
        goto EXIT;
    }

    if (true == array_is_full(p_array))
    {
        ret = array_reserve(p_array, p_array->cap * ARRAY_RESIZE_FACTOR);
    }

    if (ARRAY_SUCCESS == ret)
    {
        for (size_t idx = p_array->len; idx > index; --idx)
        {
            p_array->pp_array[idx] = p_array->pp_array[idx - 1U];
        }

        p_array->pp_array[index] = p_value;
        p_array->len++;
    }
    else
    {
        array_delete_element(p_array, p_value);
    }

EXIT:
    return ret;
}

array_error_code_t
array_remove (array_t *p_array, size_t index)
{
    if (NULL == p_array)
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    if (index >= p_array->len)
    {
        return ARRAY_OUT_OF_BOUNDS;
    }

    array_delete_element(p_array, p_array->pp_array[index]);

    for (size_t i = index; i < p_array->len - 1U; ++i)
    {
        p_array->pp_array[i] = p_array->pp_array[i + 1U];
    }

    p_array->len--;
    p_array->pp_array[p_array->len] = NULL;

    // Attempt to shrink capacity if underutilized
    (void)array_shrink_to_fit(p_array);

    return ARRAY_SUCCESS;
}

array_error_code_t
array_push (array_t *p_array, void *p_value)
{
    if ((NULL == p_array) || (NULL == p_value))
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    return array_insert(p_array, p_array->len, p_value);
}

array_error_code_t
array_pop (array_t *p_array, void **p_out)
{
    if ((NULL == p_array) || (NULL == p_out))
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    if (0U == p_array->len)
    {
        return ARRAY_OUT_OF_BOUNDS;
    }

    size_t last_index = p_array->len - 1U;
    *p_out            = p_array->pp_array[last_index];

    p_array->pp_array[last_index] = NULL;
    p_array->len--;

    return array_shrink_to_fit(p_array);
}

array_error_code_t
array_get (const array_t *p_array, size_t index, void **p_out)
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

array_error_code_t
array_set (array_t *p_array, size_t index, void *p_value)
{
    if ((NULL == p_array) || (NULL == p_value))
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    // If setting past current end, error.
    if (index > p_array->len)
    {
        array_delete_element(p_array, p_value);
        return ARRAY_OUT_OF_BOUNDS;
    }

    // If appending at the end, use array_insert to handle all logic safely.
    if (index == p_array->len)
    {
        return array_insert(p_array, index, p_value);
    }

    // Replace value at index.
    if (NULL != p_array->pp_array[index])
    {
        array_delete_element(p_array, p_array->pp_array[index]);
    }

    p_array->pp_array[index] = p_value;
    return ARRAY_SUCCESS;
}

array_error_code_t
array_find (const array_t *p_array, void *p_key, size_t *p_idx)
{
    if ((NULL == p_array) || (NULL == p_key) || (NULL == p_array->cmp_f)
        || (NULL == p_idx))
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    for (size_t idx = 0U; idx < p_array->len; idx++)
    {
        void *p_item = p_array->pp_array[idx];

        if (0 == p_array->cmp_f(p_key, p_item))
        {
            *p_idx = idx;
            return ARRAY_SUCCESS;
        }
    }

    return ARRAY_NOT_FOUND;
}

array_error_code_t
array_size (const array_t *p_array, size_t *p_size)
{
    if (NULL == p_array)
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    *p_size = p_array->len;
    return ARRAY_SUCCESS;
}

array_error_code_t
array_capacity (const array_t *p_array, size_t *p_cap)
{
    if (NULL == p_array)
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    *p_cap = p_array->cap;
    return ARRAY_SUCCESS;
}

bool
array_is_empty (const array_t *p_array)
{
    size_t size = 0U;

    if ((NULL == p_array) || (ARRAY_SUCCESS != array_size(p_array, &size)))
    {
        return true;
    }

    return (size == 0U);
}

bool
array_is_full (const array_t *p_array)
{
    size_t size = 0U;

    if ((NULL == p_array) || (ARRAY_SUCCESS != array_size(p_array, &size)))
    {
        return false;
    }

    return (size >= p_array->cap);
}

bool
array_is_equal (const array_t *p_array_a, const array_t *p_array_b)
{
    bool is_equal = true;

    if ((NULL == p_array_a) && (NULL == p_array_b))
    {
        goto EXIT;
    }

    if ((NULL == p_array_a) || (NULL == p_array_b))
    {
        is_equal = false;
        goto EXIT;
    }

    size_t size_a = 0U, size_b = 0U;

    if ((ARRAY_SUCCESS != array_size(p_array_a, &size_a))
        || (ARRAY_SUCCESS != array_size(p_array_b, &size_b)))
    {
        is_equal = false;
        goto EXIT;
    }

    if (size_a != size_b)
    {
        is_equal = false;
        goto EXIT;
    }

    for (size_t idx = 0U; idx < size_a; idx++)
    {
        void *p_a_out = p_array_a->pp_array[idx];
        void *p_b_out = p_array_b->pp_array[idx];

        if (0 != p_array_a->cmp_f(p_a_out, p_b_out))
        {
            is_equal = false;
            break;
        }
    }

EXIT:
    return is_equal;
}

array_error_code_t
array_foreach (array_t *p_array, foreach_func func)
{
    if ((NULL == p_array) || (NULL == func))
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    for (size_t idx = 0U; idx < p_array->len; idx++)
    {
        func(p_array->pp_array[idx], idx);
    }

    return ARRAY_SUCCESS;
}

array_t *
array_clone (const array_t *p_ori)
{
    array_t *p_new = NULL;

    if (NULL == p_ori)
    {
        goto EXIT;
    }

    p_new = array_create(
        p_ori->cap, p_ori->del_f, p_ori->cmp_f, p_ori->print_f, p_ori->cpy_f);

    if (NULL == p_new)
    {
        goto EXIT;
    }

    for (size_t idx = 0U; idx < p_ori->len; ++idx)
    {
        void *p_copy = p_ori->cpy_f(p_ori->pp_array[idx]);

        if (NULL == p_copy)
        {
            array_destroy(p_new);
            p_new = NULL;
            goto EXIT;
        }

        array_error_code_t ret = array_insert(p_new, idx, p_copy);

        if (ARRAY_SUCCESS != ret)
        {
            array_delete_element(p_new, p_copy);
            array_destroy(p_new);
            p_new = NULL;
            goto EXIT;
        }
    }

EXIT:
    return p_new;
}

void
array_print (const array_t *p_array)
{
    if ((NULL == p_array) || (NULL == p_array->print_f))
    {
        return;
    }

    printf("[");

    for (size_t idx = 0U; idx < p_array->len; ++idx)
    {
        p_array->print_f(p_array->pp_array[idx], idx);

        if (idx < p_array->len - 1)
        {
            printf(", ");
        }
    }

    printf("]\n");
}

static array_error_code_t
array_reserve (array_t *p_array, size_t new_cap)
{
    array_error_code_t ret = ARRAY_SUCCESS;

    if (NULL == p_array)
    {
        ret = ARRAY_INVALID_ARGUMENT;
        goto EXIT;
    }

    if ((p_array->cap >= new_cap) || (ARRAY_MAX_SIZE < new_cap))
    {
        ret = ARRAY_OUT_OF_BOUNDS;
        goto EXIT;
    }

    void **new_pp_array
        = (void **)realloc(p_array->pp_array, new_cap * sizeof(void *));

    if (NULL == new_pp_array)
    {
        ret = ARRAY_ALLOCATION_FAILURE;
        goto EXIT;
    }

    memset(&new_pp_array[p_array->cap],
           0,
           (new_cap - p_array->cap) * sizeof(void *));
    p_array->pp_array = new_pp_array;
    p_array->cap      = new_cap;

EXIT:
    return ret;
}

static array_error_code_t
array_shrink_to_fit (array_t *p_array)
{
    array_error_code_t ret = ARRAY_SUCCESS;

    if ((NULL == p_array) || (NULL == p_array->pp_array))
    {
        ret = ARRAY_INVALID_ARGUMENT;
        goto EXIT;
    }

    if ((p_array->len >= p_array->cap / ARRAY_SHRINK_THRESHOLD_DIVISOR)
        || (p_array->cap <= ARRAY_MIN_CAPACITY))
    {
        goto EXIT; // don't need to shrink array
    }

    size_t new_cap = p_array->cap / ARRAY_SHRINK_FACTOR;

    if (new_cap < ARRAY_MIN_CAPACITY)
    {
        new_cap = ARRAY_MIN_CAPACITY;
    }

    void **new_data
        = (void **)realloc(p_array->pp_array, new_cap * sizeof(void *));

    if (NULL == new_data)
    {
        ret = ARRAY_ALLOCATION_FAILURE;
        goto EXIT;
    }

    // Update array with new sized array
    p_array->pp_array = new_data;
    p_array->cap      = new_cap;

EXIT:
    return ret;
}

/*** end of file ***/
