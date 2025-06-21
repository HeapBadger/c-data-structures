/**
 * @file    test_auxiliary.c
 * @brief   File contains helper functions for test suites.
 *
 * @author  heapbadger
 */

#include "test_auxiliary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 256

void *
copy_int (const void *p_data)
{
    if (NULL == p_data)
    {
        return NULL;
    }

    int *p_copy = malloc(sizeof(int));

    if (NULL != p_copy)
    {
        *p_copy = *(const int *)p_data;
    }

    return p_copy;
}

void
delete_int (void *p_data)
{
    free(p_data);
    p_data = NULL;
    return;
}

int
compare_ints (void *p_lhs, void *p_rhs)
{
    int lhs = *(int *)p_lhs;
    int rhs = *(int *)p_rhs;
    return (lhs > rhs) - (lhs < rhs);
}

void
print_int (void *p_data, size_t index)
{
    if (p_data != NULL)
    {
        printf("Node[%zu] = %d\n", index, *(int *)p_data);
    }

    return;
}

void
multiply_by_five (void *data, size_t index)
{
    (void)index; // unused
    int *val = (int *)data;
    *val *= 5;
}

bool
is_name_match (const char *p_str1, const char *p_str2)
{
    // Check if strings are null-terminated
    bool b_is_null_terminated;
    int  length_1;
    int  length_2;

    length_1 = strnlen(p_str1, MAX_STRING_LENGTH);
    length_2 = strnlen(p_str2, MAX_STRING_LENGTH);
    b_is_null_terminated
        = ('\0' == p_str1[length_1]) && ('\0' == p_str2[length_2]);

    // Check if strings are the same length
    bool b_same_length;
    b_same_length = (length_1 == length_2);

    // Check if the strings are the same length and match in content
    bool b_name_matches;
    b_name_matches = b_same_length && (0 == strncmp(p_str1, p_str2, length_1));
    return b_name_matches && b_is_null_terminated;
}

/*** end of file ***/