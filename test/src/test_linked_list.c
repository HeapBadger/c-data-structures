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
static void test_ll_reverse_swap_update(void);
static void test_ll_null_invalid_inputs(void);

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
        == (CU_add_test(
            suite, "test_ll_foreach_clone", test_ll_foreach_clone)))
    {
        ERROR_LOG("Failed to add test_ll_foreach_clone to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_ll_reverse_swap_update", test_ll_reverse_swap_update)))
    {
        ERROR_LOG("Failed to add test_ll_reverse_swap_update to suite\n");
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

CLEANUP:
    if (NULL == suite)
    {
        CU_cleanup_registry();
    }

    return suite;
}

/**
 * @brief   Test creation and destruction of the linked list.
 */
static void
test_ll_create_destroy (void)
{
    ll_t *p_list = ll_create(delete_int, compare_ints, print_int);
    CU_ASSERT_PTR_NOT_NULL(p_list);
    CU_ASSERT_EQUAL(ll_size(p_list), 0);
    ll_destroy(p_list);
    CU_ASSERT_PTR_NULL(ll_create(NULL, NULL, NULL));
}

/**
 * @brief   Test insert, prepend, and delete operations.
 */
static void
test_ll_insert_remove (void)
{
    ll_t *p_list = ll_create(delete_int, compare_ints, print_int);
    CU_ASSERT_PTR_NOT_NULL(p_list);

    int *val1 = malloc(sizeof(int));
    int *val2 = malloc(sizeof(int));
    int *val3 = malloc(sizeof(int));
    int *val4 = malloc(sizeof(int));
    *val1     = 10;
    *val2     = 20;
    *val3     = 30;
    *val4     = 40;

    // Insert at index 0
    CU_ASSERT_EQUAL(ll_prepend(p_list, val1), LL_SUCCESS);
    CU_ASSERT_EQUAL(ll_insert(p_list, val2, 0U), LL_SUCCESS);
    CU_ASSERT_EQUAL(ll_insert(p_list, val3, 0U), LL_SUCCESS);

    // Insert out of bounds
    CU_ASSERT_EQUAL(ll_insert(p_list, val4, 5U), LL_OUT_OF_BOUNDS);
    CU_ASSERT_EQUAL(ll_size(p_list), 3);

    // Insert in the middle
    int *val5 = malloc(sizeof(int));
    *val5     = 50;
    CU_ASSERT_EQUAL(ll_insert(p_list, val5, 1U), LL_SUCCESS);
    CU_ASSERT_EQUAL(ll_size(p_list), 4);

    // Verify elements order
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 0U)->p_data, 30);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 1U)->p_data, 50);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 2U)->p_data, 20);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 3U)->p_data, 10);
    CU_ASSERT_PTR_NULL(ll_at(p_list, 4U));

    // Verify next nodes
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 0U)->p_next->p_data, 50);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 1U)->p_next->p_data, 20);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 2U)->p_next->p_data, 10);
    CU_ASSERT_EQUAL(ll_at(p_list, 3U)->p_next, NULL);

    // Delete node and test elements
    CU_ASSERT_EQUAL(ll_del_at(p_list, 1U), LL_SUCCESS);
    CU_ASSERT_EQUAL(ll_size(p_list), 3);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 0U)->p_next->p_data, 20);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 1U)->p_next->p_data, 10);

    // Test Append
    int *val6 = malloc(sizeof(int));
    *val6     = 60;
    CU_ASSERT_EQUAL(ll_append(p_list, val6), LL_SUCCESS);
    CU_ASSERT_EQUAL(ll_size(p_list), 4);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 3U)->p_data, 60);
    ll_destroy(p_list);

    // Try append to empty list
    ll_t *p_list2 = ll_create(delete_int, compare_ints, print_int);
    CU_ASSERT_PTR_NOT_NULL(p_list2);
    int *val7 = malloc(sizeof(int));
    *val7     = 600;
    CU_ASSERT_EQUAL(ll_append(p_list2, val7), LL_SUCCESS);
    CU_ASSERT_EQUAL(ll_size(p_list2), 1);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list2, 0U)->p_data, 600);
    CU_ASSERT_PTR_NULL(ll_at(p_list2, 0U)->p_next);
    ll_destroy(p_list2);
}

/**
 * @brief   Test find by index and key operations.
 */
static void
test_ll_find_at (void)
{
    ll_t *p_list = ll_create(delete_int, compare_ints, print_int);
    CU_ASSERT_PTR_NOT_NULL(p_list);

    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    int *c = malloc(sizeof(int));
    *a     = 1;
    *b     = 2;
    *c     = 3;

    ll_insert(p_list, a, 0);
    ll_insert(p_list, b, 1);
    ll_insert(p_list, c, 2);

    // Test at()
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 0)->p_data, 1);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 1)->p_data, 2);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 2)->p_data, 3);
    CU_ASSERT_PTR_NULL(ll_at(p_list, 3));

    // Test find()
    int key1 = 1, key2 = 2, key3 = 3, key4 = 4;
    CU_ASSERT_EQUAL(ll_find(p_list, &key1), 0);
    CU_ASSERT_EQUAL(ll_find(p_list, &key2), 1);
    CU_ASSERT_EQUAL(ll_find(p_list, &key3), 2);
    CU_ASSERT_EQUAL(ll_find(p_list, &key4), LL_NOT_FOUND);

    ll_destroy(p_list);
}

/**
 * @brief Test foreach by multiplying all values by 5.
 */
static void
test_ll_foreach_clone (void)
{
    ll_t *p_list = ll_create(delete_int, compare_ints, print_int);

    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    int *c = malloc(sizeof(int));
    *a     = 2;
    *b     = 4;
    *c     = 6;

    ll_insert(p_list, a, 0); // 2
    ll_insert(p_list, b, 1); // 4
    ll_insert(p_list, c, 2); // 6

    // Test foreach
    CU_ASSERT_EQUAL(ll_foreach(p_list, multiply_by_five), 0);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 0)->p_data, 10);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 1)->p_data, 20);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 2)->p_data, 30);

    // Test deep clone: should have same elements but different pointers
    ll_t *p_clone = ll_clone(p_list, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_clone);
    CU_ASSERT_EQUAL(ll_size(p_clone), ll_size(p_list));

    CU_ASSERT_PTR_NOT_EQUAL(ll_at(p_list, 0)->p_data, ll_at(p_clone, 0)->p_data);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 0)->p_data, *(int *)ll_at(p_clone, 0)->p_data);
    CU_ASSERT_PTR_NOT_EQUAL(ll_at(p_list, 1)->p_data, ll_at(p_clone, 1)->p_data);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 1)->p_data, *(int *)ll_at(p_clone, 1)->p_data);
    CU_ASSERT_PTR_NOT_EQUAL(ll_at(p_list, 2)->p_data, ll_at(p_clone, 2)->p_data);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 2)->p_data, *(int *)ll_at(p_clone, 2)->p_data);

    ll_destroy(p_list);
    ll_destroy(p_clone);
}

/**
 * @brief   Test reverse, update and swap operations.
 */
static void
test_ll_reverse_swap_update (void)
{
    ll_t *p_list = ll_create(delete_int, compare_ints, print_int);

    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    int *c = malloc(sizeof(int));
    *a     = 1;
    *b     = 2;
    *c     = 3;

    // List: 1 -> 2 -> 3
    ll_insert(p_list, a, 0);
    ll_insert(p_list, b, 1);
    ll_insert(p_list, c, 2);

    // Reverse: 3 -> 2 -> 1
    CU_ASSERT_EQUAL(ll_reverse(p_list), 0);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 0)->p_data, 3);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 0U)->p_next->p_data, 2);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 1)->p_data, 2);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 1U)->p_next->p_data, 1);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 2)->p_data, 1);
    CU_ASSERT_EQUAL(ll_at(p_list, 2U)->p_next, NULL);

    // Swap: 1 -> 2 -> 3
    CU_ASSERT_EQUAL(ll_swap(p_list, 0, 2), 0);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 0)->p_data, 1);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 2)->p_data, 3);

    // Update: 1 -> 42 -> 3
    int *new_val = malloc(sizeof(int));
    *new_val     = 42;
    CU_ASSERT_EQUAL(ll_update(p_list, 1, new_val), 0);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 1)->p_data, 42);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 0U)->p_next->p_data, 42);
    CU_ASSERT_EQUAL(*(int *)ll_at(p_list, 1U)->p_next->p_data, 3);

    ll_destroy(p_list);
}

/**
 * @brief   Test clear and is_empty behavior.
 */
static void
test_ll_null_invalid_inputs (void)
{
    CU_ASSERT_EQUAL(ll_insert(NULL, NULL, 0), LL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(ll_prepend(NULL, NULL), LL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(ll_del_at(NULL, 0), LL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(ll_at(NULL, 0), NULL);
    CU_ASSERT_EQUAL(ll_find(NULL, NULL), LL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(ll_size(NULL), LL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(ll_reverse(NULL), LL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(ll_swap(NULL, 0, 1), LL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(ll_update(NULL, 0, NULL), LL_INVALID_ARGUMENT);
}

/*** end of file ***/