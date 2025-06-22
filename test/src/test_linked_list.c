/**
 * @file    test_linked_list.c
 * @brief   Test suite for singly linked list implementation.
 *
 * @author  heapbadger
 */

#include "test_linked_list.h"
#include "linked_list.h"
#include "test_auxiliary.h"
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <stdio.h>

static void test_ll_create_destroy(void);
static void test_ll_insert_remove(void);
static void test_ll_find_at(void);
static void test_ll_foreach_clone_reverse(void);
static void test_ll_null_invalid_inputs(void);
static void test_ll_head_tail_contains_is_empty(void);

CU_pSuite
ll_suite (void)
{
    CU_pSuite suite = NULL;
    suite           = CU_add_suite("linked-list-suite", 0, 0);

    if (NULL == suite)
    {
        ERROR_LOG("Failed to add linked-list-suite\n");
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_ll_create_destroy", test_ll_create_destroy)))
    {
        ERROR_LOG("Failed to add test_ll_create_destroy to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(suite, "test_ll_insert_remove", test_ll_insert_remove)))
    {
        ERROR_LOG("Failed to add test_ll_insert_remove to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL == (CU_add_test(suite, "test_ll_find_at", test_ll_find_at)))
    {
        ERROR_LOG("Failed to add test_ll_find_at to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(suite, "test_ll_foreach_clone_reverse", test_ll_foreach_clone_reverse)))
    {
        ERROR_LOG("Failed to add test_ll_foreach_clone_reverse to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_ll_null_invalid_inputs", test_ll_null_invalid_inputs)))
    {
        ERROR_LOG("Failed to add test_ll_null_invalid_inputs to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(suite,
                        "test_ll_head_tail_contains_is_empty",
                        test_ll_head_tail_contains_is_empty)))
    {
        ERROR_LOG(
            "Failed to add test_ll_head_tail_contains_is_empty to "
            "suite\n");
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

static void
test_ll_create_destroy (void)
{
    ll_t *p_list = ll_create(delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_list);
    size_t size;
    CU_ASSERT_EQUAL(ll_size(p_list, &size), 0);
    CU_ASSERT_EQUAL(size, 0);
    ll_destroy(p_list);
    CU_ASSERT_PTR_NULL(ll_create(NULL, NULL, NULL, NULL));
}

static void
test_ll_insert_remove (void)
{
    ll_t *p_list = NULL;
    int *val1 = malloc(sizeof(int));
    *val1     = 1;
    int *val2 = malloc(sizeof(int));
    *val2     = 2;
    int *val3 = malloc(sizeof(int));
    *val3     = 3;

    // Attempt append and insert with NULL list
    CU_ASSERT_EQUAL(ll_append(p_list, val1), LL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(ll_insert(p_list, val3, 1), LL_INVALID_ARGUMENT);

    // Append and insert values
    p_list = ll_create(delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_EQUAL(ll_append(p_list, val1), LL_SUCCESS);
    CU_ASSERT_EQUAL(ll_append(p_list, val2), LL_SUCCESS);
    CU_ASSERT_EQUAL(ll_insert(p_list, val3, 1), LL_SUCCESS);

    // Size list
    size_t size;
    CU_ASSERT_EQUAL(ll_size(p_list, &size), LL_SUCCESS);
    CU_ASSERT_EQUAL(size, 3);

    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 0)->p_data, 1);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 1)->p_data, 3);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 2)->p_data, 2);

    CU_ASSERT_EQUAL(ll_del_at(p_list, 1), LL_SUCCESS);
    CU_ASSERT_EQUAL(ll_size(p_list, &size), LL_SUCCESS);
    CU_ASSERT_EQUAL(size, 2);

    ll_destroy(p_list);
}

static void
test_ll_find_at (void)
{
    ll_t *p_list = ll_create(delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_TRUE(ll_is_empty(p_list));

    int *val1 = malloc(sizeof(int));
    *val1     = 10;
    int *val2 = malloc(sizeof(int));
    *val2     = 20;
    int *val3 = malloc(sizeof(int));
    *val3     = 30;

    // Append values and check linked list values
    CU_ASSERT_EQUAL(ll_append(p_list, val1), LL_SUCCESS);
    CU_ASSERT_EQUAL(ll_append(p_list, val2), LL_SUCCESS);
    CU_ASSERT_EQUAL(ll_append(p_list, val3), LL_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 0)->p_data, 10);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 0)->p_next->p_data, 20);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 1)->p_next->p_data, 30);
    CU_ASSERT_PTR_NULL(ll_at(p_list, 2)->p_next);
    CU_ASSERT_FALSE(ll_is_empty(p_list));

    // Find value
    size_t idx = 0;
    CU_ASSERT_EQUAL(ll_find(p_list, val2, &idx), LL_SUCCESS);
    CU_ASSERT_EQUAL(idx, 1);

    // Attempt to find non-existent value
    int val_not_found = 100;
    CU_ASSERT_EQUAL(ll_find(p_list, &val_not_found, &idx), LL_NOT_FOUND);

    ll_destroy(p_list);
}

static void
test_ll_foreach_clone_reverse (void)
{
    ll_t *p_ori = NULL;
    p_ori = ll_create(delete_int, compare_ints, print_int, copy_int);
    int **values = calloc(5, sizeof(int *));

    for (int idx = 0; idx < 5; ++idx)
    {
        values[idx]  = calloc(1, sizeof(int));
        *values[idx] = idx * 5;
        CU_ASSERT_EQUAL(ll_append(p_ori, values[idx]), LL_SUCCESS);
    }

    free(values);
    CU_ASSERT_EQUAL(ll_foreach(p_ori, multiply_by_five), LL_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_ori, 0)->p_data, 0);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_ori, 1)->p_data, 5*5);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_ori, 2)->p_data, 10*5);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_ori, 3)->p_data, 15*5);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_ori, 4)->p_data, 20*5);

    ll_t *p_new = ll_clone(p_ori);
    CU_ASSERT_PTR_NOT_NULL(p_new);
    CU_ASSERT_PTR_NOT_EQUAL(p_new, p_ori);

    size_t size_ori, size_new;
    CU_ASSERT_EQUAL(ll_size(p_ori, &size_ori), LL_SUCCESS);
    CU_ASSERT_EQUAL(ll_size(p_new, &size_new), LL_SUCCESS);
    CU_ASSERT_EQUAL(size_ori, size_new);

    CU_ASSERT_EQUAL(*(int *)ll_at(p_ori, 0)->p_data,
                    *(int *)ll_at(p_new, 0)->p_data);
    CU_ASSERT_PTR_NOT_EQUAL(ll_at(p_ori, 0)->p_data,
                            ll_at(p_new, 0)->p_data);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_ori, 1)->p_data,
                    *(int *)ll_at(p_new, 1)->p_data);
    CU_ASSERT_PTR_NOT_EQUAL(ll_at(p_ori, 1)->p_data,
                            ll_at(p_new, 1)->p_data);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_ori, 2)->p_data,
                    *(int *)ll_at(p_new, 2)->p_data);
    CU_ASSERT_PTR_NOT_EQUAL(ll_at(p_ori, 2)->p_data,
                            ll_at(p_new, 2)->p_data);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_ori, 3)->p_data,
                    *(int *)ll_at(p_new, 3)->p_data);
    CU_ASSERT_PTR_NOT_EQUAL(ll_at(p_ori, 3)->p_data,
                            ll_at(p_new, 3)->p_data);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_ori, 4)->p_data,
                    *(int *)ll_at(p_new, 4)->p_data);
    CU_ASSERT_PTR_NOT_EQUAL(ll_at(p_ori, 4)->p_data,
                            ll_at(p_new, 4)->p_data);
    
    CU_ASSERT_EQUAL(ll_reverse(p_new), LL_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_ori, 4)->p_data,
                    *(int *)ll_at(p_new, 0)->p_data);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_ori, 3)->p_data,
                    *(int *)ll_at(p_new, 1)->p_data);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_ori, 2)->p_data,
                    *(int *)ll_at(p_new, 2)->p_data);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_ori, 1)->p_data,
                    *(int *)ll_at(p_new, 3)->p_data);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_ori, 0)->p_data,
                    *(int *)ll_at(p_new, 4)->p_data);

    ll_destroy(p_ori);
    ll_destroy(p_new);
}

static void
test_ll_null_invalid_inputs (void)
{
    CU_ASSERT_EQUAL(ll_size(NULL, NULL), LL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(ll_del_at(NULL, 0), LL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(ll_insert(NULL, NULL, 0), LL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(ll_foreach(NULL, NULL), LL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(ll_find(NULL, NULL, NULL), LL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(ll_reverse(NULL), LL_INVALID_ARGUMENT);
    CU_ASSERT_TRUE(ll_is_empty(NULL));
    CU_ASSERT_PTR_NULL(ll_at(NULL, 0));
    CU_ASSERT_PTR_NULL(ll_clone(NULL));
}

static void
test_ll_head_tail_contains_is_empty (void)
{
    ll_t *p_list = ll_create(delete_int, compare_ints, print_int, copy_int);

    CU_ASSERT_TRUE(ll_is_empty(p_list));
    CU_ASSERT_PTR_NULL(ll_head(p_list));
    CU_ASSERT_PTR_NULL(ll_tail(p_list));

    int *a = malloc(sizeof(int));
    *a     = 100;
    int *b = malloc(sizeof(int));
    *b     = 200;

    ll_append(p_list, a);
    ll_append(p_list, b);

    CU_ASSERT_FALSE(ll_is_empty(p_list));
    CU_ASSERT_PTR_NOT_NULL(ll_head(p_list));
    CU_ASSERT_PTR_NOT_NULL(ll_tail(p_list));
    CU_ASSERT_EQUAL(*(int *)ll_head(p_list)->p_data, 100);
    CU_ASSERT_EQUAL(*(int *)ll_tail(p_list)->p_data, 200);

    CU_ASSERT_FALSE(ll_is_empty(p_list));
    ll_destroy(p_list);
}

/*** end of file ***/
