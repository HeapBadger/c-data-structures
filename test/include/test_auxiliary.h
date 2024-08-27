/**
 * @file    test_auxiliary.h
 * @brief   Header file for `test_auxiliary.c`.
 *
 * @author  Anna DeVries
 * @date    August 20, 2024
 */

#ifndef TEST_AUXILIARY_H
#define TEST_AUXILIARY_H

#include <stdbool.h>

/**
 * @brief Provides global error logging.
 *
 * This definition allows additional arguments to be passed as an option.
 */
#define ERROR_LOG(fmt, ...)                                                    \
  fprintf(stderr, "\n[Error]: " fmt "\n", ##__VA_ARGS__)

/**
 * @brief Provides global information logging.
 *
 * This definition allows additional arguments to be passed as an option.
 */
#define INFO_LOG(fmt, ...) fprintf(stderr, "\n[Info]: " fmt "\n", ##__VA_ARGS__)

/**
 * @brief Captures the stdout output of a function that takes a void* argument.
 *
 * This function temporarily redirects stdout to a temporary file, executes the
 * provided function, captures its output, and then restores stdout. The output
 * is returned as a string.
 *
 * @param func   The function whose output is to be captured. The function
 * should accept a single void* argument and return void.
 * @param input  The input parameter to be passed to the function `func`.
 *               This parameter is of type void* and can be any data type.
 *
 * @return A pointer to a string containing the captured output. If an error
 *         occurs, NULL is returned. The caller is responsible for freeing the
 *         returned string.
 *
 * @note The function returns NULL in case of failure. Make sure to check the
 *       return value before using the captured output.
 *
 * @note The function removes the temporary file after capturing the output.
 *
 * @warning The caller is responsible for ensuring that the temporary file path
 *          defined by TEMP_FILE_PATH is valid and writable.
 *
 * Example usage:
 * @code
 * void my_function(void *input) {
 *     printf("Input is: %s\n", (char *)input);
 * }
 *
 * char *output = capture_stdout(my_function, "Hello, World!");
 * if (output) {
 *     printf("Captured output: %s\n", output);
 *     free(output);
 * } else {
 *     printf("Failed to capture output.\n");
 * }
 * @endcode
 */
char *capture_stdout(void (*func)(void *), void *input);

/**
 * @brief   Compare two strings for equality, checking length and null
 * termination.
 *
 * This function checks if two strings are of the same length, null-terminated,
 * and if their content matches up to the length of the shorter string.
 *
 * @param string_1  Pointer to the first string to compare.
 * @param string_2  Pointer to the second string to compare.
 *
 * @return  True if both strings are of the same length, null-terminated,
 *          and match in content. False otherwise.
 */
bool is_name_match(const char *string_1, const char *string_2);

#endif // TEST_AUXILIARY_H
