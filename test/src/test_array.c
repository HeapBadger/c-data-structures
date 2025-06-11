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
static void test_array_insert_remove(void);
static void test_array_size_capacity(void);
static void test_array_set_get(void);
static void test_array_clear_empty(void);

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

    if (NULL == (CU_add_test(suite, "test_array_create_destroy", test_array_create_destroy)))
    {
        ERROR_LOG("Failed to add test_linked_list_simple to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL == (CU_add_test(suite, "test_array_insert_remove", test_array_insert_remove)))
    {
        ERROR_LOG("Failed to add test_array_insert_remove to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL == (CU_add_test(suite, "test_array_size_capacity", test_array_size_capacity)))
    {
        ERROR_LOG("Failed to add test_array_size_capacity to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL == (CU_add_test(suite, "test_array_set_get", test_array_set_get)))
    {
        ERROR_LOG("Failed to add test_array_set_get to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL == (CU_add_test(suite, "test_array_clear_empty", test_array_clear_empty)))
    {
        ERROR_LOG("Failed to add test_array_clear_empty to suite\n");
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
    array_t *p_array = array_create(3, delete_int, compare_ints, print_int);
    CU_ASSERT_PTR_NOT_NULL(p_array);
    CU_ASSERT_EQUAL(array_size(p_array), 0);
    CU_ASSERT(array_capacity(p_array) >= 3);
    CU_ASSERT_EQUAL(array_is_empty(p_array), true);

    int *val_1 = calloc(1, sizeof(int));
    *val_1     = 100;
    array_insert(p_array, 0, val_1);

    int *val_2 = calloc(1, sizeof(int));
    *val_2     = 3;
    array_insert(p_array, 1, val_2);

    int *val_3 = calloc(1, sizeof(int));
    *val_3     = 7;
    array_insert(p_array, 2, val_3);

    int *val_4 = calloc(1, sizeof(int));
    *val_4     = 0;
    array_insert(p_array, 3, val_4);

    CU_ASSERT_EQUAL(array_size(p_array), 3);

    array_remove(p_array, 2);
    CU_ASSERT_EQUAL(array_size(p_array), 2);

    int *val_5 = calloc(1, sizeof(int));
    *val_5     = 10;
    array_push(p_array, val_5);
    CU_ASSERT_EQUAL(array_size(p_array), 3);

    int *val_6 = calloc(1, sizeof(int));
    *val_6     = 10;
    array_push(p_array, val_6);
    array_pop(p_array);
    CU_ASSERT_EQUAL(array_size(p_array), 2);

    void *p_val = NULL;
    array_get(p_array, p_array->len-1, &p_val);
    p_array->print_f(p_val);

    array_destroy(p_array);
}

/**
 * @brief   Test insert, push, remove and pop operations.
 */
static void
test_array_insert_remove (void)
{
    return;
}

/**
 * @brief   Test size and capacity functions.
 */
static void
test_array_size_capacity (void)
{
    return;
}

/**
 * @brief   Test set and get functions.
 */
static void
test_array_set_get (void)
{
    array_t *p_array = array_create(10, delete_int, compare_ints, print_int);
    CU_ASSERT_PTR_NOT_NULL(p_array);
    CU_ASSERT_EQUAL(array_size(p_array), 0);
    CU_ASSERT(array_capacity(p_array) >= 10);
    array_destroy(p_array);
}

/**
 * @brief   Test clear and is_empty behavior.
 */
static void
test_array_clear_empty (void)
{
    return;
}

/*** end of file ***/