/**
 * @file    test_array.c
 * @brief   Test suite for the dynamic array.
 *
 * This file contains the test suite creation for 'array.c'.
 * Each test validates the behavior of core dynamic array operations.
 *
 * @author  heapbadger
 * @date    June 09, 2025
 */

#include "test_array.h"
#include "array.h"
#include "test_auxiliary.h"
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function Declarations
static void test_array_create_destroy(void);

// Suite Creation Function
CU_pSuite
array_suite (void)
{
    CU_pSuite suite = NULL;
    suite           = CU_add_suite("array-suite", 0, 0);

    if (NULL == suite)
    {
        ERROR_LOG("Failed to add array-suite\n");
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_array_create_destroy", test_array_create_destroy)))
    {
        ERROR_LOG("Failed to add test_linked_list_simple to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

CLEANUP:
    if (NULL == suite)
    {
        CU_cleanup_registry();
    }

    return suite;
}

/**
 * @brief   Test creation and destruction of the array.
 */
static void
test_array_create_destroy (void)
{
    array_t *p_array = array_create(10);
    CU_ASSERT_PTR_NOT_NULL(p_array);
    CU_ASSERT_EQUAL(array_size(p_array), 0);
    CU_ASSERT(array_capacity(p_array) >= 10);
    array_destroy(p_array);
}
