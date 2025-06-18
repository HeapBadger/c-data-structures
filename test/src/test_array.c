/**
 * @file    test_array.c
 * @brief   Test suite for the dynamic array.
 *
 * @author  heapbadger
 */

#include "test_array.h"
#include "array.h"
#include "test_auxiliary.h"
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void test_array_create_destroy(void);
static void test_array_insert_remove(void);
static void test_array_size_capacity(void);
static void test_array_set_get(void);
static void test_array_clear_empty(void);
static void test_array_null_inputs(void);
static void test_array_bounds(void);
static void test_array_resize_behavior(void);
static void test_array_foreach_clone(void);

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
        ERROR_LOG("Failed to add test_array_create_destroy to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_array_insert_remove", test_array_insert_remove)))
    {
        ERROR_LOG("Failed to add test_array_insert_remove to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_array_size_capacity", test_array_size_capacity)))
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

    if (NULL
        == (CU_add_test(
            suite, "test_array_clear_empty", test_array_clear_empty)))
    {
        ERROR_LOG("Failed to add test_array_clear_empty to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL == (CU_add_test(suite, "test_array_bounds", test_array_bounds)))
    {
        ERROR_LOG("Failed to add test_array_bounds to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_array_resize_behavior", test_array_resize_behavior)))
    {
        ERROR_LOG("Failed to add test_array_resize_behavior to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_array_foreach_clone", test_array_foreach_clone)))
    {
        ERROR_LOG("Failed to add test_array_foreach_clone to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_array_null_inputs", test_array_null_inputs)))
    {
        ERROR_LOG("Failed to add test_array_null_inputs to suite\n");
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
    array_t *p_array1
        = array_create(10, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_array1);
    size_t size = 0U;
    CU_ASSERT_EQUAL(array_size(p_array1, &size), ARRAY_SUCCESS);
    CU_ASSERT_EQUAL(size, 0);
    CU_ASSERT_EQUAL(array_capacity(p_array1, &size), ARRAY_SUCCESS);
    CU_ASSERT(size >= 10);
    array_destroy(p_array1);

    array_t *p_array2
        = array_create(0, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NULL(p_array2);

    p_array2 = array_create(10, NULL, NULL, NULL, NULL);
    CU_ASSERT_PTR_NULL(p_array2);
}

/**
 * @brief   Test insert, push, remove and pop operations.
 */
static void
test_array_insert_remove (void)
{
    array_t *p_array
        = array_create(4, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_array);
    size_t size = 0U;
    CU_ASSERT_EQUAL(array_capacity(p_array, &size), ARRAY_SUCCESS);
    CU_ASSERT(size >= 4);
    CU_ASSERT_TRUE(array_is_empty(p_array));
    CU_ASSERT_FALSE(array_is_full(p_array));

    int *val1 = malloc(sizeof(int));
    int *val2 = malloc(sizeof(int));
    int *val3 = malloc(sizeof(int));
    int *val4 = malloc(sizeof(int));
    *val1     = 10;
    *val2     = 20;
    *val3     = 30;
    *val4     = 40;

    // Insert at index 0 (empty array)
    array_error_code_t ret = array_insert(p_array, 0, val1);
    CU_ASSERT_EQUAL(ret, ARRAY_SUCCESS);
    CU_ASSERT_EQUAL(array_size(p_array, &size), ARRAY_SUCCESS);
    CU_ASSERT(size == 1);

    // Push elements
    ret = array_push(p_array, val2);
    CU_ASSERT_EQUAL(ret, ARRAY_SUCCESS);
    CU_ASSERT_EQUAL(array_size(p_array, &size), ARRAY_SUCCESS);
    CU_ASSERT(size == 2);
    ret = array_push(p_array, val3);
    CU_ASSERT_EQUAL(ret, ARRAY_SUCCESS);
    CU_ASSERT_EQUAL(array_size(p_array, &size), ARRAY_SUCCESS);
    CU_ASSERT(size == 3);

    // Insert in the middle
    ret = array_insert(p_array, 1, val4); // between val1 and val2
    CU_ASSERT_EQUAL(ret, ARRAY_SUCCESS);
    CU_ASSERT_EQUAL(array_size(p_array, &size), ARRAY_SUCCESS);
    CU_ASSERT(size == 4);
    CU_ASSERT_TRUE(array_is_full(p_array));
    CU_ASSERT_FALSE(array_is_empty(p_array));

    // Verify elements order via get
    void *out = NULL;
    array_get(p_array, 0, &out);
    CU_ASSERT_EQUAL(*(int *)out, 10);
    array_get(p_array, 1, &out);
    CU_ASSERT_EQUAL(*(int *)out, 40);
    array_get(p_array, 2, &out);
    CU_ASSERT_EQUAL(*(int *)out, 20);
    array_get(p_array, 3, &out);
    CU_ASSERT_EQUAL(*(int *)out, 30);

    // Insert out of bound index
    int *val6 = malloc(sizeof(int));
    *val6     = 1;
    CU_ASSERT_EQUAL(array_insert(p_array, 20, val6), ARRAY_OUT_OF_BOUNDS);

    // Remove element at index 1 (val4)
    ret = array_remove(p_array, 1);
    CU_ASSERT_EQUAL(ret, ARRAY_SUCCESS);
    CU_ASSERT_EQUAL(array_size(p_array, &size), ARRAY_SUCCESS);
    CU_ASSERT(size == 3);

    // Pop last element (val3)
    void *p_elem = NULL;
    ret          = array_pop(p_array, &p_elem);
    CU_ASSERT_EQUAL(ret, ARRAY_SUCCESS);
    CU_ASSERT_EQUAL(array_size(p_array, &size), ARRAY_SUCCESS);
    CU_ASSERT(size == 2);
    CU_ASSERT_EQUAL(*(int *)p_elem, 30);
    p_array->del_f(p_elem);

    // Cleanup
    array_destroy(p_array);

    // Test filling the array
    p_array   = array_create(4, delete_int, compare_ints, print_int, copy_int);
    int *val0 = malloc(sizeof(int));
    *val0     = 0;
    CU_ASSERT_EQUAL(array_fill(p_array, val0), ARRAY_SUCCESS);
    free(val0);

    for (size_t idx = 0; idx < p_array->cap; ++idx)
    {
        array_get(p_array, 0, &out);
        CU_ASSERT_EQUAL(*(int *)out, 0);
    }

    CU_ASSERT_EQUAL(p_array->cap, p_array->len);

    array_destroy(p_array);
}

/**
 * @brief   Test size and capacity functions.
 */
static void
test_array_size_capacity (void)
{
    array_t *p_array
        = array_create(2, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_array);
    size_t size = 0U;
    CU_ASSERT_EQUAL(array_size(p_array, &size), ARRAY_SUCCESS);
    CU_ASSERT(size == 0);
    CU_ASSERT_EQUAL(array_capacity(p_array, &size), ARRAY_SUCCESS);
    CU_ASSERT(size >= 2);

    int *val1 = malloc(sizeof(int));
    int *val2 = malloc(sizeof(int));
    *val1     = 5;
    *val2     = 6;
    array_push(p_array, val1);
    array_push(p_array, val2);
    CU_ASSERT_EQUAL(array_size(p_array, &size), ARRAY_SUCCESS);
    CU_ASSERT(size == 2);
    CU_ASSERT_EQUAL(array_capacity(p_array, &size), ARRAY_SUCCESS);
    CU_ASSERT(size >= 2);

    void *p_elem = NULL;
    CU_ASSERT_EQUAL(array_pop(p_array, &p_elem), ARRAY_SUCCESS);
    CU_ASSERT_EQUAL(array_size(p_array, &size), ARRAY_SUCCESS);
    CU_ASSERT(size == 1);
    CU_ASSERT_EQUAL(*(int *)p_elem, 6);
    p_array->del_f(p_elem);

    array_clear(p_array);
    CU_ASSERT_EQUAL(array_size(p_array, &size), ARRAY_SUCCESS);
    CU_ASSERT(size == 0);

    array_destroy(p_array);
}

/**
 * @brief   Test set and get functions.
 */
static void
test_array_set_get (void)
{
    array_t *p_array
        = array_create(5, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_array);

    int *val1 = malloc(sizeof(int));
    int *val2 = malloc(sizeof(int));
    *val1     = 100;
    *val2     = 200;

    array_error_code_t ret = array_push(p_array, val1);

    CU_ASSERT_EQUAL(ret, ARRAY_SUCCESS);

    ret = array_set(p_array, 0, val2); // Should delete old val1 internally
    CU_ASSERT_EQUAL(ret, ARRAY_SUCCESS);

    void *out = NULL;
    ret       = array_get(p_array, 0, &out);
    CU_ASSERT_EQUAL(ret, ARRAY_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(out);
    CU_ASSERT_EQUAL(*(int *)out, 200);

    // Invalid get/set tests
    ret = array_get(p_array, 5, &out);
    CU_ASSERT_EQUAL(ret, ARRAY_OUT_OF_BOUNDS);

    int *val3 = malloc(sizeof(int));
    *val3     = 300;
    ret       = array_set(p_array, 5, val3);
    CU_ASSERT_EQUAL(ret, ARRAY_OUT_OF_BOUNDS);

    array_destroy(p_array);
}

/**
 * @brief   Test clear and is_empty behavior.
 */
static void
test_array_clear_empty (void)
{
    array_t *p_array
        = array_create(3, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_array);

    CU_ASSERT_TRUE(array_is_empty(p_array));

    int *val = malloc(sizeof(int));
    *val     = 42;
    array_push(p_array, val);

    CU_ASSERT_FALSE(array_is_empty(p_array));

    array_clear(p_array);
    CU_ASSERT_TRUE(array_is_empty(p_array));
    size_t size = 0U;
    CU_ASSERT_EQUAL(array_size(p_array, &size), ARRAY_SUCCESS);
    CU_ASSERT(size == 0);

    array_destroy(p_array);
}

/**
 * @brief Test sorting and searching with valid and invalid input.
 */
static void
test_array_bounds (void)
{
    array_t *p_array
        = array_create(4, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_array);

    int *vals[] = { malloc(sizeof(int)),
                    malloc(sizeof(int)),
                    malloc(sizeof(int)),
                    malloc(sizeof(int)) };
    *vals[0]    = 5;
    *vals[1]    = 2;
    *vals[2]    = 8;
    *vals[3]    = 1;

    for (int i = 0; i < 4; ++i)
    {
        array_push(p_array, vals[i]);
    }

    // Invalid index access
    void *out  = NULL;
    int  *val4 = malloc(sizeof(int));
    *val4      = 300;
    CU_ASSERT_EQUAL(array_get(p_array, 999, &out), ARRAY_OUT_OF_BOUNDS);
    CU_ASSERT_EQUAL(array_set(p_array, 999, val4), ARRAY_OUT_OF_BOUNDS);
    CU_ASSERT_EQUAL(array_remove(p_array, 999), ARRAY_OUT_OF_BOUNDS);

    array_destroy(p_array);
}

/**
 * @brief   Test dynamic capacity changes via insertions and removals.
 */
static void
test_array_resize_behavior (void)
{
    array_t *p_array
        = array_create(4, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_array);
    size_t cap_before = 0U;
    CU_ASSERT_EQUAL(array_capacity(p_array, &cap_before), ARRAY_SUCCESS);
    CU_ASSERT(cap_before >= 4);
    CU_ASSERT_TRUE(array_is_empty(p_array));

    // Insert values to exceed initial capacity and trigger growth
    for (int i = 0; i < 100; ++i)
    {
        int *val = malloc(sizeof(int));
        *val     = i * 10;
        CU_ASSERT_EQUAL(array_push(p_array, val), ARRAY_SUCCESS);
    }

    size_t cap_after_growth = 0U;
    CU_ASSERT_EQUAL(array_capacity(p_array, &cap_after_growth), ARRAY_SUCCESS);
    CU_ASSERT(cap_after_growth > cap_before);

    // Remove enough elements to trigger shrinking condition
    for (int i = 0; i < 92;
         ++i) // Remove most items to drop length below capacity/6
    {
        void *p_ele = NULL;
        CU_ASSERT_EQUAL(array_pop(p_array, &p_ele), ARRAY_SUCCESS);
        p_array->del_f(p_ele);
    }

    // Capacity should have decreased but not below minimum capacity
    size_t cap_after_shrink = 0U;
    CU_ASSERT_EQUAL(array_capacity(p_array, &cap_after_shrink), ARRAY_SUCCESS);
    CU_ASSERT(cap_after_shrink < cap_after_growth);
    CU_ASSERT(cap_after_shrink >= ARRAY_MIN_CAPACITY);

    // Cleanup
    array_destroy(p_array);
}

/**
 * @brief   Tests cloning and foreach operations.
 */
static void
test_array_foreach_clone (void)
{
    array_t *p_array
        = array_create(5, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_array);

    // Push some integers
    for (int i = 0; i < 5; ++i)
    {
        int *val = malloc(sizeof(int));
        *val     = i + 1;
        CU_ASSERT_EQUAL(array_push(p_array, val), ARRAY_SUCCESS);
    }

    // Test foreach
    array_foreach(p_array, multiply_by_five);
    void *out = NULL;
    CU_ASSERT_EQUAL(array_get(p_array, 0, &out), ARRAY_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)out, 5);
    CU_ASSERT_EQUAL(array_get(p_array, 1, &out), ARRAY_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)out, 10);
    CU_ASSERT_EQUAL(array_get(p_array, 2, &out), ARRAY_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)out, 15);
    CU_ASSERT_EQUAL(array_get(p_array, 3, &out), ARRAY_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)out, 20);
    CU_ASSERT_EQUAL(array_get(p_array, 4, &out), ARRAY_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)out, 25);

    // Test deep clone: should have same elements but different pointers
    array_t *p_clone = array_clone(p_array);
    CU_ASSERT_PTR_NOT_NULL(p_clone);
    size_t size_ori = 0U, size_new = 0U;
    CU_ASSERT_EQUAL(array_size(p_array, &size_ori), ARRAY_SUCCESS);
    CU_ASSERT_EQUAL(array_size(p_clone, &size_new), ARRAY_SUCCESS);
    CU_ASSERT(size_new == size_ori);

    for (size_t i = 0; i < size_ori; ++i)
    {
        void *orig = NULL, *cloned = NULL;
        array_get(p_array, i, &orig);
        array_get(p_clone, i, &cloned);
        CU_ASSERT_PTR_NOT_EQUAL(orig, cloned);
        CU_ASSERT_EQUAL(*(int *)orig, *(int *)cloned);
    }

    // Clean up
    array_destroy(p_array);
    array_destroy(p_clone);
}

/**
 * @brief Test behavior with NULL input and invalid conditions.
 */
static void
test_array_null_inputs (void)
{
    CU_ASSERT_PTR_NULL(array_create(0, NULL, NULL, NULL, NULL));
    CU_ASSERT_EQUAL(array_size(NULL, NULL), ARRAY_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(array_capacity(NULL, NULL), ARRAY_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(array_push(NULL, NULL), ARRAY_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(array_insert(NULL, 0, NULL), ARRAY_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(array_set(NULL, 0, NULL), ARRAY_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(array_get(NULL, 0, NULL), ARRAY_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(array_remove(NULL, 0), ARRAY_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(array_pop(NULL, NULL), ARRAY_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(array_find(NULL, NULL, NULL), ARRAY_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(array_foreach(NULL, NULL), ARRAY_INVALID_ARGUMENT);
    CU_ASSERT_PTR_NULL(array_clone(NULL));
}

/*** end of file ***/
