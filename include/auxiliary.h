/**
 * @file    auxiliary.h
 * @brief   Header file containing auxiliary definitions.
 *
 * @author  heapbadger
 */

#ifndef AUXILIARY_H
#define AUXILIARY_H

#include <stdio.h>
#include <string.h>

/**
 * @brief Function pointer type for a generic data cleanup function.
 *
 * This function pointer defines the signature for a function responsible for
 * releasing or cleaning up the memory/resources associated with a single data
 * element. It should properly handle freeing dynamically allocated memory or
 * any other necessary teardown steps for the element.
 *
 * @param p_data Pointer to the data element to be deleted.
 *
 * @note Implementations must ensure no memory leaks occur.
 *
 * @example
 * To delete integers/chars/strings:
 * @code
 * void delete_function(void *p_data)
 * {
 *     free(p_data);
 *     p_data = NULL;
 * }
 * @endcode
 */
typedef void (*del_func)(void *p_data);

/**
 * @brief Function pointer type for a generic data print function.
 *
 * This function pointer defines the signature for a function responsible for
 * printing or displaying the contents of a single data element in a human-
 * readable format.
 *
 * @param p_data Pointer to the data element to be printed.
 *
 * @note The function should handle formatting according to the data type.
 *
 * @example
 * To print integers:
 * @code
 * void print_int(void *p_data) {
 *     printf("%d\n", *(int *)p_data);
 * }
 * @endcode
 *
 * To print characters:
 * @code
 * void print_char(void *p_data) {
 *     printf("%c\n", *(char *)p_data);
 * }
 * @endcode
 *
 * To print strings:
 * @code
 * void print_string(void *p_data) {
 *     printf("%s\n", (char *)p_data);
 * }
 * @endcode
 */
typedef void (*print_func)(void *p_data);

/**
 * @brief Function pointer type for a generic data comparison function.
 *
 * This function pointer defines the signature for a function that compares
 * two data elements and determines their relative ordering.
 *
 * @param p_lhs Pointer to the left-hand side data element.
 * @param p_rhs Pointer to the right-hand side data element.
 *
 * @return An integer less than, equal to, or greater than zero if the left-hand
 *         element is considered respectively less than, equal to, or greater
 *         than the right-hand element.
 *
 * @note The return value semantics should be compatible with standard functions
 *       like strcmp().
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
 * To compare characters:
 * @code
 * int compare_chars(void *p_lhs, void *p_rhs) {
 *     char lhs = *(char *)p_lhs;
 *     char rhs = *(char *)p_rhs;
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

#endif // AUXILIARY_H

/*** end of file ***/