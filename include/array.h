/**
 * @file    array.h
 * @brief   Header file for `array.c`.
 *
 * @author  heapbadger
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>
#include <sys/types.h>
#include "auxiliary.h"

/**
 * Growth factor used when expanding array capacity.
 * Should be tuned to minimize reallocations during heavy insertion.
 */
#define ARRAY_RESIZE_FACTOR 2

/**
 * Shrink threshold divisor: shrink only if size < capacity /
 * ARRAY_SHRINK_THRESHOLD_DIVISOR. Prevents frequent shrinking when usage
 * temporarily dips, reducing realloc overhead
 */
#define ARRAY_SHRINK_THRESHOLD_DIVISOR 6

/**
 * Shrink factor used when reducing capacity.
 */
#define ARRAY_SHRINK_FACTOR 2

/**
 * Minimum allowed capacity after shrinking.
 * Should be tuned to avoid thrashing by maintaining a reasonable base size.
 */
#define ARRAY_MIN_CAPACITY 16

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
    copy_func  cpy_f;
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
array_t *array_create(size_t           initial_capacity,
                      const del_func   del_f,
                      const cmp_func   cmp_f,
                      const print_func print_f,
                      const copy_func  cpy_f);

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
 * @brief Deletes a single element using the array's delete function.
 *
 * @param p_array Pointer to the array.
 * @param p_value Pointer to the element to delete.
 */
void array_delete_element(array_t *p_array, void *p_value);

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
 * @return 0 on success, negative error code on failure.
 */
ssize_t array_push(array_t *p_array, void *p_value);

/**
 * @brief Remove the last element of the array.
 *
 * @note Caller is responsible for freeing data.
 *
 * @param p_array Pointer to the array.
 * @return 0 on success, negative error code if array is empty or NULL.
 */
ssize_t array_pop(array_t *p_array, void **p_out);

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
 * @param p_key Key to search for.
 * @return Index of the element if found, negative error code otherwise.
 */
ssize_t array_find(const array_t *p_array, void *p_key);

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
 * @return Maximum number of elements the array can hold, or negative error
 * code.
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
 * @brief Check whether two arrays are equal.
 *
 * @param p_array_a Pointer to the array a.
 * @param p_array_b Pointer to the array b.
 * @return true if equal, false otherwise.
 */
bool array_is_equal(const array_t *p_array_a, const array_t *p_array_b);

/**
 * @brief Ensure the array has at least the given capacity.
 *
 * @param p_array Pointer to the array.
 * @param new_cap Minimum capacity to reserve.
 * @return 0 on success, negative error code on failure.
 */
ssize_t array_reserve(array_t *p_array, size_t new_cap);

/**
 * @brief Shrink the array's allocated memory to match its size.
 *
 * @param p_array Pointer to the array.
 * @return 0 on success, negative error code on failure.
 */
ssize_t array_shrink_to_fit(array_t *p_array);

/**
 * @brief Apply a function to each element in the array.
 *
 * @param p_array Pointer to the array.
 * @param func    Function to apply to each element. The function should
 *                accept a `void *` to the element and a `size_t` index.
 *
 * @return 0 on success, or a negative error code on failure.
 */
ssize_t array_foreach(array_t *p_array, foreach_func func);

/**
 * @brief Create a deep copy of the array structure using a user-provided copy
 * function.
 *
 * @param p_array Pointer to the source array.
 *
 * @return Pointer to a new array on success, or NULL on failure.
 */
array_t *array_clone(const array_t *p_ori);

/**
 * @brief Sort the array using bubble sort and a custom comparator.
 *
 * This function performs an in-place bubble sort on the dynamic array using the
 * user-provided comparison function (`cmp_f`) stored in the array structure.
 *
 * @param p_array Pointer to the dynamic array.
 * @return 0 on success, or a negative error code on failure (e.g., invalid
 * input).
 */
ssize_t array_bubblesort(array_t *p_array);

/**
 * @brief Find an element using binary search (requires sorted array).
 *
 * @param p_array Pointer to the array.
 * @param p_key Key to search for.
 * @return Index of the element if found, negative error code otherwise.
 */
ssize_t array_sorted_search(const array_t *p_array, void *p_key);

#endif // ARRAY_H

/*** end of file ***/