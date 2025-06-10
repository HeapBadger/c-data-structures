/**
 * @file    array.h
 * @brief   Header file for `array.c`.
 *
 * @author  heapbadger
 * @date    June 09, 2025
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <stdint.h>
#include "auxiliary.h"

typedef enum
{
    ARRAY_SUCCESS = 0,        /**< Operation completed successfully. */
    ARRAY_NOT_FOUND,          /**< The requested element was not found. */
    ARRAY_OUT_OF_BOUNDS,      /**< An index was out of the valid range. */
    ARRAY_INVALID_ARGUMENT,   /**< An invalid argument was provided. */
    ARRAY_ALLOCATION_FAILURE, /**< Unable to allocate memory. */
} array_error_code_t;

typedef struct
{
    void     **pp_data; // Array of pointers to elements
    uint64_t   len;     // Current number of elements
    uint64_t   cap;     // Allocated capacity
    del_func   del_f;   // Data cleanup
    cmp_func   cmp_f;   // Data comparison
    print_func print_f; // Data printing
} array_t;

array_t *array_create(size_t initial_capacity);
void     array_destroy(array_t *p_array);
size_t   array_size(const array_t *p_array);
size_t   array_capacity(const array_t *p_array);

#endif // ARRAY_H
