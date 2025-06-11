/**
 * @file    array.h
 * @brief   Header file for `array.c`.
 *
 * @author  heapbadger
 * @date    June 09, 2025
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>
#include <sys/types.h>
#include "auxiliary.h"

#define ARRAY_RESIZE_FACTOR 2

typedef enum
{
    ARRAY_SUCCESS            = 0,  /**< Operation succeeded. */
    ARRAY_NOT_FOUND          = -1, /**< Element not found. */
    ARRAY_OUT_OF_BOUNDS      = -2, /**< Index out of range. */
    ARRAY_INVALID_ARGUMENT   = -3, /**< Invalid argument provided. */
    ARRAY_ALLOCATION_FAILURE = -4, /**< Memory allocation failed. */
} array_error_code_t;

typedef struct
{
    void     **pp_array;
    size_t     len;
    size_t     cap;
    del_func   del_f;
    cmp_func   cmp_f;
    print_func print_f;
} array_t;

/**
 * @brief Create a new dynamic array.
 *
 * @param initial_capacity Initial number of elements to allocate.
 * @param del_f Pointer to a function that frees an element.
 * @param cmp_f Pointer to a comparison function for elements.
 * @param print_f Pointer to a function that prints an element.
 * @return Pointer to the new array, or NULL on failure.
 */
array_t *array_create(size_t initial_capacity,
                      const del_func del_f,
                      const cmp_func cmp_f,
                      const print_func print_f);

/**
 * @brief Free all memory and destroy the array.
 *
 * @param p_array Pointer to the array to destroy.
 */
void array_destroy(array_t *p_array);

/**
 * @brief Remove all elements from the array, keeping capacity.
 *
 * @param p_array Pointer to the array.
 */
void array_clear(array_t *p_array);

/**
 * @brief Insert an element at a specific index.
 *
 * @param p_array Pointer to the array.
 * @param index Index at which to insert the new element.
 * @param p_value Value to insert.
 * @return 0 on success, negative error code on failure.
 */
ssize_t array_insert(array_t *p_array, size_t index, void *p_value);

/**
 * @brief Remove an element at a specific index.
 *
 * @param p_array Pointer to the array.
 * @param index Index of the element to remove.
 * @return 0 on success, negative error code on failure.
 */
ssize_t array_remove(array_t *p_array, size_t index);

/**
 * @brief Append an element to the end of the array.
 *
 * @param p_array Pointer to the array.
 * @param p_value The value to append.
 * @return New length of the array on success, negative error code on failure.
 */
ssize_t array_push(array_t *p_array, void *p_value);

/**
 * @brief Remove the last element of the array.
 *
 * @param p_array Pointer to the array.
 * @return 0 on success, negative error code if array is empty or NULL.
 */
ssize_t array_pop(array_t *p_array);

/**
 * @brief Retrieve an element from the array.
 *
 * @param p_array Pointer to the array.
 * @param index Index of the element to retrieve.
 * @param out Output pointer for the element.
 * @return 0 on success, negative error code on failure.
 */
ssize_t array_get(const array_t *p_array, size_t index, void **p_out);

/**
 * @brief Set an element at a given index in the array.
 *
 * @param p_array Pointer to the array.
 * @param index Index to set the element at.
 * @param p_value The value to insert.
 * @return 0 on success, negative error code on failure.
 */
ssize_t array_set(array_t *p_array, size_t index, void *p_value);

/**
 * @brief Find the index of a value using linear search.
 *
 * @param p_array Pointer to the array.
 * @param key Key to search for.
 * @return Index of the element if found, negative error code otherwise.
 */
ssize_t array_find(const array_t *p_array, void *key);

/**
 * @brief Get the current number of elements in the array.
 *
 * @param p_array Pointer to the array.
 * @return Number of elements, or negative error code if p_array is NULL.
 */
ssize_t array_size(const array_t *p_array);

/**
 * @brief Get the current capacity of the array.
 *
 * @param p_array Pointer to the array.
 * @return Maximum number of elements the array can hold, or negative error code.
 */
ssize_t array_capacity(const array_t *p_array);

/**
 * @brief Check whether the array is empty.
 *
 * @param p_array Pointer to the array.
 * @return true if empty, false otherwise.
 */
bool array_is_empty(const array_t *p_array);

/**
 * @brief Check whether the array is full.
 *
 * @param p_array Pointer to the array.
 * @return true if full, false otherwise.
 */
bool array_is_full(const array_t *p_array);

/**
 * @brief Ensure the array has at least the given capacity.
 *
 * @param p_array Pointer to the array.
 * @param new_capacity Minimum capacity to reserve.
 * @return 0 on success, negative error code on failure.
 */
ssize_t array_reserve(array_t *p_array, size_t new_capacity);

/**
 * @brief Shrink the array's allocated memory to match its size.
 *
 * @param p_array Pointer to the array.
 * @return 0 on success, negative error code on failure.
 */
ssize_t array_shrink_to_fit(array_t *p_array);

/**
 * @brief Sort the array in-place using a custom comparator.
 *
 * @param p_array Pointer to the array.
 */
void array_sort(array_t *p_array);

/**
 * @brief Find an element using binary search (requires sorted array).
 *
 * @param p_array Pointer to the array.
 * @param key Key to search for.
 * @return Index of the element if found, negative error code otherwise.
 */
ssize_t array_binary_search(const array_t *p_array, void *key);


#endif // ARRAY_H

/*** end of file ***/