/**
 * @file    test_auxiliary.h
 * @brief   Header file for `test_auxiliary.c`.
 *
 * @author  heapbadger
 */

#ifndef TEST_AUXILIARY_H
#define TEST_AUXILIARY_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Provides global error logging.
 *
 * This definition allows additional arguments to be passed as an option.
 */
#define ERROR_LOG(fmt, ...) \
    fprintf(stderr, "\n[Error]: " fmt "\n", ##__VA_ARGS__)

/**
 * @brief Provides global information logging.
 *
 * This definition allows additional arguments to be passed as an option.
 */
#define INFO_LOG(fmt, ...) fprintf(stderr, "\n[Info]: " fmt "\n", ##__VA_ARGS__)

/**
 * @brief   Copies an integer into another allocated pointer.
 *
 * @param p_data  Pointer to the dynamically allocated integer to copy.
 */
void *copy_int(const void *p_data);

/**
 * @brief   Deletes an integer pointer by freeing its allocated memory.
 *
 * @param p_data  Pointer to the dynamically allocated integer to delete.
 */
void delete_int(void *p_data);

/**
 * @brief   Compares two integer values.
 *
 * @param p_lhs  Pointer to the left-hand-side integer.
 * @param p_rhs  Pointer to the right-hand-side integer.
 *
 * @return Result of integer comparison.
 */
int compare_ints(void *p_lhs, void *p_rhs);

/**
 * @brief   Prints an integer value to standard output.
 *
 * @param p_data  Pointer to the integer to print.
 * @param index   Index for the node.
 */
void print_int(void *p_data, size_t index);

/**
 * @brief Helper function to multiply int data by 5.
 */
void multiply_by_five(void *data, size_t index);

/**
 * @brief   Compare two strings for equality, checking length and null
 * termination.
 *
 * This function checks if two strings are of the same length, null-terminated,
 * and if their content matches up to the length of the shorter string.
 *
 * @param p_str1  Pointer to the first string to compare.
 * @param p_str2  Pointer to the second string to compare.
 *
 * @return  True if both strings are of the same length, null-terminated,
 *          and match in content. False otherwise.
 */
bool is_name_match(const char *p_str1, const char *p_str2);

#endif // TEST_AUXILIARY_H

/*** end of file ***/