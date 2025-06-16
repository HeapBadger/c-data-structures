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

static ssize_t array_binary_search(const array_t *p_array,
                                   size_t         low,
                                   size_t         high,
                                   void          *p_key);

array_t *
array_create (size_t           initial_capacity,
              const del_func   del_f,
              const cmp_func   cmp_f,
              const print_func print_f,
              const copy_func  cpy_f)
{
    array_t *p_array = NULL;

    if ((0 < initial_capacity) && (NULL != del_f) && (NULL != cmp_f)
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
        array_clear(p_array);
        free(p_array->pp_array);
        free(p_array);
    }
}

void
array_clear (array_t *p_array)
{
    if ((NULL != p_array) && (NULL != p_array->pp_array))
    {
        for (size_t idx = p_array->len; idx-- > 0U;)
        {
            (void)array_remove(p_array, idx);
        }
    }
}

void
array_delete_element (array_t *p_array, void *p_value)
{
    if (NULL != p_array)
    {
        p_array->del_f(p_value);
    }
}

ssize_t
array_insert (array_t *p_array, size_t index, void *p_value)
{
    ssize_t ret = ARRAY_SUCCESS;

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

ssize_t
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

ssize_t
array_push (array_t *p_array, void *p_value)
{
    if ((NULL == p_array) || (NULL == p_value))
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    return array_insert(p_array, p_array->len, p_value);
}

ssize_t
array_pop (array_t *p_array, void **p_out)
{
    if (NULL == p_array)
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    if (0U == p_array->len)
    {
        return ARRAY_OUT_OF_BOUNDS;
    }

    size_t last_index = p_array->len - 1U;
    *p_out            = p_array->pp_array[last_index];

    // Do NOT call del_f — the caller now owns the element.
    p_array->pp_array[last_index] = NULL;
    p_array->len--;

    return array_shrink_to_fit(p_array);
}

ssize_t
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

ssize_t
array_set (array_t *p_array, size_t index, void *p_value)
{
    if ((NULL == p_array) || (NULL == p_value))
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    if ((index > p_array->len) || (index >= p_array->cap))
    {
        array_delete_element(p_array, p_value);
        return ARRAY_OUT_OF_BOUNDS;
    }
    else if (index == p_array->len)
    {
        p_array->len++;
    }

    if (NULL != p_array->pp_array[index])
    {
        array_delete_element(p_array, p_array->pp_array[index]);
    }

    p_array->pp_array[index] = p_value;
    return ARRAY_SUCCESS;
}

ssize_t
array_find (const array_t *p_array, void *p_key)
{
    if ((NULL == p_array) || (NULL == p_key) || (NULL == p_array->cmp_f))
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    for (size_t idx = 0U; idx < p_array->len; idx++)
    {
        void *p_item = p_array->pp_array[idx];

        if (0 == p_array->cmp_f(p_key, p_item))
        {
            return (ssize_t)idx;
        }
    }

    return ARRAY_NOT_FOUND;
}

ssize_t
array_size (const array_t *p_array)
{
    if (NULL == p_array)
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    return (ssize_t)p_array->len;
}

ssize_t
array_capacity (const array_t *p_array)
{
    if (NULL == p_array)
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    return (ssize_t)p_array->cap;
}

bool
array_is_empty (const array_t *p_array)
{
    return (array_size(p_array) <= 0);
}

bool
array_is_full (const array_t *p_array)
{
    return (array_size(p_array) >= array_capacity(p_array));
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

    if ((array_size(p_array_a) != array_size(p_array_b))
        || (p_array_a->cmp_f != p_array_b->cmp_f))
    {
        is_equal = false;
        goto EXIT;
    }

    size_t len = array_size(p_array_a);

    for (size_t idx = 0U; idx < len; idx++)
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

ssize_t
array_reserve (array_t *p_array, size_t new_cap)
{
    ssize_t ret = ARRAY_SUCCESS;

    if (NULL == p_array)
    {
        ret = ARRAY_INVALID_ARGUMENT;
        goto EXIT;
    }

    if (p_array->cap >= new_cap)
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

ssize_t
array_shrink_to_fit (array_t *p_array)
{
    ssize_t ret = ARRAY_SUCCESS;

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

ssize_t
array_foreach (array_t *p_array, foreach_func func)
{
    if ((NULL == p_array) || (NULL == func))
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    for (size_t idx = 0; idx < p_array->len; idx++)
    {
        func(p_array->pp_array[idx], idx);
    }

    return ARRAY_SUCCESS;
}

array_t *
array_clone (const array_t *p_ori)
{
    array_t *p_new = NULL;

    if ((NULL == p_ori) || (NULL == p_ori->cpy_f))
    {
        goto EXIT;
    }

    p_new = array_create(
        p_ori->cap, p_ori->del_f, p_ori->cmp_f, p_ori->print_f, p_ori->cpy_f);

    if (NULL == p_new)
    {
        goto EXIT;
    }

    p_new->len = p_ori->len;

    for (size_t idx = 0; idx < p_ori->len; ++idx)
    {
        void *copied_elem = p_ori->cpy_f(p_ori->pp_array[idx]);

        if (NULL == copied_elem)
        {
            array_destroy(p_new); // clean up partially created clone
            p_new = NULL;
            goto EXIT;
        }

        p_new->pp_array[idx] = copied_elem;
    }

EXIT:
    return p_new;
}

ssize_t
array_bubblesort (array_t *p_array)
{
    if ((NULL == p_array) || (NULL == p_array->cmp_f))
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    for (size_t i = 0U; i < p_array->len - 1U; ++i)
    {
        for (size_t j = 0U; j < p_array->len - i - 1U; ++j)
        {
            if (0 < p_array->cmp_f(p_array->pp_array[j],
                                   p_array->pp_array[j + 1U]))
            {
                void *tmp                 = p_array->pp_array[j];
                p_array->pp_array[j]      = p_array->pp_array[j + 1U];
                p_array->pp_array[j + 1U] = tmp;
            }
        }
    }

    return ARRAY_SUCCESS;
}

ssize_t
array_sorted_search (const array_t *p_array, void *p_key)
{
    if ((NULL == p_array) || (NULL == p_key) || (NULL == p_array->cmp_f))
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    return array_binary_search(p_array, 0U, p_array->len - 1U, p_key);
}

/**
 * @brief Perform a recursive binary search on a sorted array.
 *
 * This internal utility function performs binary search recursively over the
 * range [low, high] in a dynamic array that is assumed to be sorted. It uses
 * the array's comparison function (`cmp_f`) to compare elements with the given
 * key.
 *
 * @note This function assumes the array is sorted and inputs are valid.
 *
 * @param p_array Pointer to the dynamic array.
 * @param low The lower index of the search range (inclusive).
 * @param high The upper index of the search range (inclusive).
 * @param p_key Pointer to the key to search for.
 * @return Index of the found element, or a negative error code if not found.
 */
static ssize_t
array_binary_search (const array_t *p_array,
                     size_t         low,
                     size_t         high,
                     void          *p_key)
{
    if (low > high)
    {
        return ARRAY_NOT_FOUND;
    }

    ssize_t mid     = low + (high - low) / 2U;
    void   *mid_val = p_array->pp_array[mid];
    ssize_t cmp     = p_array->cmp_f(mid_val, p_key);

    if (0 == cmp)
    {
        return mid;
    }
    else if (cmp > 0)
    {
        if (mid == 0) // check underflow
        {
            return ARRAY_NOT_FOUND;
        }

        return array_binary_search(p_array, low, mid - 1U, p_key);
    }
    else
    {
        return array_binary_search(p_array, mid + 1U, high, p_key);
    }
}

/*** end of file ***/
