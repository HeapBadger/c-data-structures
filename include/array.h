/**
 * @file    array.h
 * @brief   Header file for `array.c`.
 *
 * @author  heapbadger
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>
#include "auxiliary.h"

/**
 * Max number of elements that array can have.
 */
#define ARRAY_MAX_SIZE 500

/**
 * Growth factor used when expanding array capacity.
 * Should be tuned to minimize reallocations during heavy insertion.
 */
#define ARRAY_RESIZE_FACTOR 2

/**
 * Shrink threshold divisor: shrink only if size < capacity /
 * ARRAY_SHRINK_THRESHOLD_DIVISOR. Prevents frequent shrinking when usage
 * temporarily dips, reducing realloc overhead.
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
 *
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
 * @brief Deletes a single element using the registered delete function.
 *
 * @param p_array Pointer to the array.
 * @param p_value Pointer to the element to delete.
 */
void array_del_ele(array_t *p_array, void *p_value);

/**
 * @brief Fill the entire array with copies of the given value. Overwrites
 * any previous data.
 *
 * @param p_array Pointer to the array to fill.
 * @param p_value Pointer to the value to copy into each element.
 *
 * @note Caller is responsible for freeing p_value.
 *
 * @return ARRAY_SUCCESS on success, appropriate error code otherwise.
 */
array_error_code_t array_fill(array_t *p_array, void *p_value);

/**
 * @brief Insert an element at the specified index.
 *
 * @param p_array Pointer to the array.
 * @param index Index to insert at.
 * @param p_value Pointer to the value to insert.
 *
 * @return ARRAY_SUCCESS on success, appropriate error code otherwise.
 */
array_error_code_t array_insert(array_t *p_array, size_t index, void *p_value);

/**
 * @brief Remove the element at the specified index.
 *
 * @param p_array Pointer to the array.
 * @param index Index to remove from.
 *
 * @return ARRAY_SUCCESS on success, appropriate error code otherwise.
 */
array_error_code_t array_remove(array_t *p_array, size_t index);

/**
 * @brief Append an element to the end of the array.
 *
 * @param p_array Pointer to the array.
 * @param p_value Pointer to the value to append.
 *
 * @return ARRAY_SUCCESS on success, appropriate error code otherwise.
 */
array_error_code_t array_push(array_t *p_array, void *p_value);

/**
 * @brief Remove the last element from the array.
 *
 * @param p_array Pointer to the array.
 * @param p_out Output parameter to hold the removed element.
 *
 * @note Caller must free p_out appropriately.
 *
 * @return ARRAY_SUCCESS on success, appropriate error code otherwise.
 */
array_error_code_t array_pop(array_t *p_array, void **p_out);

/**
 * @brief Retrieve the element at the given index.
 *
 * @param p_array Pointer to the array.
 * @param index Index of the element to retrieve.
 * @param p_out Output parameter to hold the retrieved element.
 *
 * @return ARRAY_SUCCESS on success, appropriate error code otherwise.
 */
array_error_code_t array_get(const array_t *p_array,
                             size_t         index,
                             void         **p_out);

/**
 * @brief Replace the element at the given index.
 *
 * @param p_array Pointer to the array.
 * @param index Index of the element to replace.
 * @param p_value Pointer to the new value.
 *
 * @return ARRAY_SUCCESS on success, appropriate error code otherwise.
 */
array_error_code_t array_set(array_t *p_array, size_t index, void *p_value);

/**
 * @brief Find the index of the given key using the comparison function.
 *
 * @param p_array Pointer to the array.
 * @param p_key Pointer to the key to find.
 * @param p_idx Output parameter for the found index.
 *
 * @return ARRAY_SUCCESS on success, appropriate error code otherwise.
 */
array_error_code_t array_find(const array_t *p_array,
                              void          *p_key,
                              size_t        *p_idx);

/**
 * @brief Get the number of elements in the array.
 *
 * @param p_array Pointer to the array.
 * @param p_size Output parameter to store the array's current size.
 *
 * @return ARRAY_SUCCESS on success, appropriate error code otherwise.
 */
array_error_code_t array_size(const array_t *p_array, size_t *p_size);

/**
 * @brief Get the total capacity of the array.
 *
 * @param p_array Pointer to the array.
 * @param p_cap Output parameter to store the array's capacity.
 *
 * @return ARRAY_SUCCESS on success, appropriate error code otherwise.
 */
array_error_code_t array_capacity(const array_t *p_array, size_t *p_cap);

/**
 * @brief Check whether the array is empty.
 *
 * @param p_array Pointer to the array.
 *
 * @return true if empty, false otherwise.
 */
bool array_is_empty(const array_t *p_array);

/**
 * @brief Check whether the array is full.
 *
 * @param p_array Pointer to the array.
 *
 * @return true if full, false otherwise.
 */
bool array_is_full(const array_t *p_array);

/**
 * @brief Check whether two arrays are equal.
 *
 * @param p_array_a Pointer to the array a.
 * @param p_array_b Pointer to the array b.
 * 
 * @note Assumes that both arrays have same registered comparison
 * function!
 *
 * @return true if equal, false otherwise.
 */
bool array_is_equal(const array_t *p_array_a, const array_t *p_array_b);

/**
 * @brief Apply a function to each element in the array.
 *
 * @param p_array Pointer to the array.
 * @param func Function to apply to each element.
 *
 * @return ARRAY_SUCCESS on success, appropriate error code otherwise.
 */
array_error_code_t array_foreach(array_t *p_array, foreach_func func);

/**
 * @brief Create a deep copy of the array structure.
 *
 * @param p_ori Pointer to the source array.
 *
 * @return Pointer to a new array on success, or NULL on failure.
 */
array_t *array_clone(const array_t *p_ori);

/**
 * @brief Prints an array.
 *
 * @param p_array Pointer to the array.
 */
void array_print(const array_t *p_array);

#endif // ARRAY_H

/*** end of file ***/