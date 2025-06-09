/**
 * @file    test_auxiliary.c
 * @brief   File contains helper functions for test suites.
 *
 * @author  heapbadger
 * @date    August 20, 2024
 */

#include "test_auxiliary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Macros
#define OUTPUT_BUFFER_SIZE 1024
#define MAX_STRING_LENGTH  256
#define TEMP_FILE_PATH     "/tmp/test_output.txt"

// Compares two strings
bool
is_name_match (const char *string_1, const char *string_2)
{
    // Check if strings are null-terminated
    bool b_is_null_terminated;
    int  length_1;
    int  length_2;

    length_1 = strnlen(string_1, MAX_STRING_LENGTH);
    length_2 = strnlen(string_2, MAX_STRING_LENGTH);
    b_is_null_terminated
        = ('\0' == string_1[length_1]) && ('\0' == string_2[length_2]);

    // Check if strings are the same length
    bool b_same_length;
    b_same_length = (length_1 == length_2);

    // Check if the strings are the same length and match in content
    bool b_name_matches;
    b_name_matches
        = b_same_length && (0 == strncmp(string_1, string_2, length_1));
    return b_name_matches && b_is_null_terminated;
}

// Captures stdout output and saves to buffer
char *
capture_stdout (void (*func)(void *), void *input)
{
    int   fd;              // File descriptor to save current stdout
    FILE *tmp_file = NULL; // File to hold function output
    char *output   = NULL; // Captured output

    // Save current stdout fd
    fd = dup(fileno(stdout));

    if (-1 == fd)
    {
        ERROR_LOG("Failed to duplicate stdout");
        goto CLEANUP;
    }

    // Open a file for capturing output
    tmp_file = fopen(TEMP_FILE_PATH, "w");

    if (NULL == tmp_file)
    {
        ERROR_LOG("Failed to open temporary output file");
        goto CLEANUP;
    }

    // Redirect stdout to file
    if (-1 == dup2(fileno(tmp_file), fileno(stdout)))
    {
        ERROR_LOG("Failed to redirect stdout");
        goto CLEANUP;
    }

    // Execute the function and capture its output
    (void)func(input);

    // Flush stdout (ensure all output is written)
    fflush(stdout);

    // Restore original stdout
    if (-1 == dup2(fd, fileno(stdout)))
    {
        ERROR_LOG("Failed to restore original stdout");
        goto CLEANUP;
    }

    // Close the stdout fd and file we saved output to
    close(fd);
    fclose(tmp_file);
    tmp_file = NULL;

    // Reopen file and save output to buffer
    tmp_file = fopen(TEMP_FILE_PATH, "r");

    if (NULL == tmp_file)
    {
        ERROR_LOG("Failed to open temporary output file for reading");
        goto CLEANUP;
    }

    output = (char *)malloc(OUTPUT_BUFFER_SIZE);

    if (NULL == output)
    {
        ERROR_LOG("Failed to allocate memory for output");
        goto CLEANUP;
    }

    if (NULL == fgets(output, OUTPUT_BUFFER_SIZE, tmp_file))
    {
        ERROR_LOG("Failed to read output from temporary file");
        goto CLEANUP;
    }

    fclose(tmp_file);
    tmp_file = NULL;

    // Remove the file
    if (EXIT_SUCCESS != remove(TEMP_FILE_PATH))
    {
        ERROR_LOG("Failed to remove temporary file");
        goto CLEANUP;
    }

    return output;

CLEANUP:
    if (-1 != fd)
    {
        close(fd);
    }

    if (NULL != tmp_file)
    {
        fclose(tmp_file);
        tmp_file = NULL;
    }

    if (NULL != output)
    {
        free(output);
        output = NULL;
    }

    return NULL;
}
