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
static void test_ll_foreach_clone(void);
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
        == (CU_add_test(suite, "test_ll_foreach_clone", test_ll_foreach_clone)))
    {
        ERROR_LOG("Failed to add test_ll_foreach_clone to suite\n");
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
    ll_t *p_list = ll_create(delete_int, compare_ints, print_int, copy_int);

    int *a = malloc(sizeof(int));
    *a     = 1;
    int *b = malloc(sizeof(int));
    *b     = 2;
    int *c = malloc(sizeof(int));
    *c     = 3;

    CU_ASSERT_EQUAL(ll_prepend(p_list, a), LL_SUCCESS);
    CU_ASSERT_EQUAL(ll_append(p_list, b), LL_SUCCESS);
    CU_ASSERT_EQUAL(ll_insert(p_list, c, 1), LL_SUCCESS);

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

    int *x = malloc(sizeof(int));
    *x     = 10;
    int *y = malloc(sizeof(int));
    *y     = 20;
    int *z = malloc(sizeof(int));
    *z     = 30;

    ll_append(p_list, x);
    ll_append(p_list, y);
    ll_append(p_list, z);

    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 1)->p_data, 20);

    size_t idx = 0;
    CU_ASSERT_EQUAL(ll_find(p_list, y, &idx), LL_SUCCESS);
    CU_ASSERT_EQUAL(idx, 1);

    int val_not_found = 100;
    CU_ASSERT_EQUAL(ll_find(p_list, &val_not_found, &idx), LL_NOT_FOUND);

    ll_destroy(p_list);
}

static void
test_ll_foreach_clone (void)
{
    ll_t *p_list = ll_create(delete_int, compare_ints, print_int, copy_int);

    int *a = malloc(sizeof(int));
    *a     = 2;
    int *b = malloc(sizeof(int));
    *b     = 4;
    int *c = malloc(sizeof(int));
    *c     = 6;

    ll_append(p_list, a);
    ll_append(p_list, b);
    ll_append(p_list, c);

    CU_ASSERT_EQUAL(ll_foreach(p_list, multiply_by_five), LL_SUCCESS);

    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 0)->p_data, 10);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 1)->p_data, 20);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 2)->p_data, 30);

    ll_t *p_clone = ll_clone(p_list);
    CU_ASSERT_PTR_NOT_NULL(p_clone);

    size_t size1, size2;
    CU_ASSERT_EQUAL(ll_size(p_list, &size1), LL_SUCCESS);
    CU_ASSERT_EQUAL(ll_size(p_clone, &size2), LL_SUCCESS);
    CU_ASSERT_EQUAL(size1, size2);

    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 1)->p_data,
                    *(int *)ll_at(p_clone, 1)->p_data);
    CU_ASSERT_PTR_NOT_EQUAL(ll_at(p_list, 1)->p_data,
                            ll_at(p_clone, 1)->p_data);

    ll_destroy(p_list);
    ll_destroy(p_clone);
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
