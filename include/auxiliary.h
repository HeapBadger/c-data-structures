/**
 * @file    auxiliary.h
 * @brief   Auxiliary type definitions for generic data operations.
 *
 * @author  heapbadger
 */

#ifndef AUXILIARY_H
#define AUXILIARY_H

#include <stdio.h>
#include <string.h>

/**
 * @brief Function pointer type for data cleanup (deletion).
 *
 * Defines the signature for a function responsible for releasing or cleaning up
 * resources associated with a single data element. This may involve freeing
 * dynamically allocated memory or other teardown actions.
 *
 * @param p_data Pointer to the data element to be deleted.
 *
 * @note Implementations must avoid memory leaks.
 *
 * @example
 * To delete dynamically allocated integers:
 * @code
 * void delete_int(void *p_data)
 * {
 *     free(p_data);
 * }
 * @endcode
 *
 * To delete dynamically allocated strings:
 * @code
 * void delete_string(void *p_data)
 * {
 *     free(p_data);
 * }
 * @endcode
 */
typedef void (*del_func)(void *p_data);

/**
 * @brief Function pointer type for printing a data element.
 *
 * Defines the signature for a function that prints or displays a data element
 * in a human-readable format. The function receives both the element pointer
 * and its index (useful for contextual output).
 *
 * @param p_data Pointer to the data element.
 * @param index  Zero-based index of the element.
 *
 * @note Formatting is dependent on the data type.
 *
 * @example
 * To print an integer:
 * @code
 * void print_int(void *p_data, size_t index) {
 *     printf("Node[%zu] = %d\n", index, *(int *)p_data);
 * }
 * @endcode
 *
 * To print a string:
 * @code
 * void print_string(void *p_data, size_t index) {
 *     printf("Node[%zu] = %s\n", index, (char *)p_data);
 * }
 * @endcode
 */
typedef void (*print_func)(void *p_data, size_t index);

/**
 * @brief Function pointer type for comparing two data elements.
 *
 * Defines the signature for a function that compares two elements to determine
 * their relative order.
 *
 * @param p_lhs Pointer to the left-hand side element.
 * @param p_rhs Pointer to the right-hand side element.
 *
 * @return Negative if lhs < rhs, zero if lhs == rhs, positive if lhs > rhs.
 *
 * @note Should follow standard comparison function semantics (like strcmp).
 *
 * @example
 * To compare integers:
 * @code
 * int compare_ints(void *p_lhs, void *p_rhs) {
 *     int lhs = *(int *)p_lhs;
 *     int rhs = *(int *)p_rhs;
 *     return (lhs > rhs) - (lhs < rhs);
 * }
 * @endcode
 *
 * To compare strings:
 * @code
 * int compare_strings(void *p_lhs, void *p_rhs) {
 *     return strcmp((char *)p_lhs, (char *)p_rhs);
 * }
 * @endcode
 */
typedef int (*cmp_func)(void *p_lhs, void *p_rhs);

/**
 * @brief Function pointer type for applying an operation to each element.
 *
 * Defines the signature for a function that operates on each element during
 * iteration.
 *
 * @param p_data Pointer to the current element.
 * @param index  Zero-based index of the element.
 */
typedef void (*foreach_func)(void *p_data, size_t index);

/**
 * @brief Function pointer type for deep copying a data element.
 *
 * Defines the signature for a function that creates a deep copy of a data
 * element.
 *
 * @param p_data Pointer to the source data element.
 * @return Pointer to a newly allocated copy of the data, or NULL on failure.
 *
 * @example
 * To deep copy an integer:
 * @code
 * void *copy_int(const void *p_data) {
 *     int *copy = malloc(sizeof(int));
 *     if (copy) *copy = *(const int *)p_data;
 *     return copy;
 * }
 * @endcode
 *
 * To deep copy a string:
 * @code
 * void *copy_string(const void *p_data) {
 *     return strdup((const char *)p_data);
 * }
 * @endcode
 */
typedef void *(*copy_func)(const void *p_data);

#endif // AUXILIARY_H

/*** end of file ***/
