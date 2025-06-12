/**
 * @file    test_doubly_linked_list.c
 * @brief   Test suite for doubly linked list implementation.
 *
 * @author  heapbadger
 */

#include "test_doubly_linked_list.h"
#include "doubly_linked_list.h"
#include "test_auxiliary.h"
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <stdio.h>

static void test_dll_create_destroy(void);
static void test_dll_insert_remove(void);
static void test_dll_find_at(void);
static void test_dl_foreach_clone(void);
static void test_dll_reverse_swap_update(void);
static void test_dll_null_invalid_inputs(void);

CU_pSuite
dl_suite (void)
{
    CU_pSuite suite = NULL;
    suite           = CU_add_suite("doubly-linked-list-suite", 0, 0);

    if (NULL == suite)
    {
        ERROR_LOG("Failed to add doubly-linked-list-suite\n");
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_dll_create_destroy", test_dll_create_destroy)))
    {
        ERROR_LOG("Failed to add test_dll_create_destroy to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_dll_insert_remove", test_dll_insert_remove)))
    {
        ERROR_LOG("Failed to add test_dll_insert_remove to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL == (CU_add_test(suite, "test_dll_find_at", test_dll_find_at)))
    {
        ERROR_LOG("Failed to add test_dll_find_at to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_dl_foreach_clone", test_dl_foreach_clone)))
    {
        ERROR_LOG("Failed to add test_dl_foreach_clone to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(suite,
                        "test_dll_reverse_swap_update",
                        test_dll_reverse_swap_update)))
    {
        ERROR_LOG("Failed to add test_dll_reverse_swap_update to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(suite,
                        "test_dll_null_invalid_inputs",
                        test_dll_null_invalid_inputs)))
    {
        ERROR_LOG("Failed to add test_dll_null_invalid_inputs to suite\n");
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
 * @brief   Test creation and destruction of the doubly linked list.
 */
static void
test_dll_create_destroy (void)
{
    dl_t *p_list = dl_create(delete_int, compare_ints, print_int);
    CU_ASSERT_PTR_NOT_NULL(p_list);
    CU_ASSERT_EQUAL(dl_size(p_list), 0);
    dl_destroy(p_list);
    CU_ASSERT_PTR_NULL(dl_create(NULL, NULL, NULL));
}

/**
 * @brief   Test insert, prepend, and delete operations.
 */
static void
test_dll_insert_remove (void)
{
    dl_t *p_list = dl_create(delete_int, compare_ints, print_int);
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
    CU_ASSERT_EQUAL(dl_prepend(p_list, val1), DL_SUCCESS);
    CU_ASSERT_EQUAL(dl_insert(p_list, val2, 0U), DL_SUCCESS);
    CU_ASSERT_EQUAL(dl_insert(p_list, val3, 0U), DL_SUCCESS);

    // Insert out of bounds
    CU_ASSERT_EQUAL(dl_insert(p_list, val4, 5U), DL_OUT_OF_BOUNDS);
    CU_ASSERT_EQUAL(dl_size(p_list), 3);

    // Insert in the middle
    int *val5 = malloc(sizeof(int));
    *val5     = 50;
    CU_ASSERT_EQUAL(dl_insert(p_list, val5, 1U), DL_SUCCESS);
    CU_ASSERT_EQUAL(dl_size(p_list), 4);

    // Verify elements order
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 0U)->p_data, 30);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 1U)->p_data, 50);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 2U)->p_data, 20);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 3U)->p_data, 10);
    CU_ASSERT_PTR_NULL(dl_at(p_list, 4U));

    // Verify prev and next nodes
    CU_ASSERT_EQUAL(dl_at(p_list, 0U)->p_prev, NULL);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 0U)->p_next->p_data, 50);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 1U)->p_prev->p_data, 30);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 1U)->p_next->p_data, 20);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 2U)->p_prev->p_data, 50);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 2U)->p_next->p_data, 10);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 3U)->p_prev->p_data, 20);
    CU_ASSERT_EQUAL(dl_at(p_list, 3U)->p_next, NULL);

    // Delete node and test elements
    CU_ASSERT_EQUAL(dl_del_at(p_list, 1U), DL_SUCCESS);
    CU_ASSERT_EQUAL(dl_size(p_list), 3);
    CU_ASSERT_EQUAL(dl_at(p_list, 0U)->p_prev, NULL);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 0U)->p_next->p_data, 20);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 1U)->p_prev->p_data, 30);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 1U)->p_next->p_data, 10);

    dl_destroy(p_list);
}

/**
 * @brief   Test find by index and key operations.
 */
static void
test_dll_find_at (void)
{
    dl_t *p_list = dl_create(delete_int, compare_ints, print_int);
    CU_ASSERT_PTR_NOT_NULL(p_list);

    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    int *c = malloc(sizeof(int));
    *a     = 1;
    *b     = 2;
    *c     = 3;

    dl_insert(p_list, a, 0U);
    dl_insert(p_list, b, 1U);
    dl_insert(p_list, c, 2U);

    // Verify elements order
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 0U)->p_data, 1);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 1U)->p_data, 2);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 2U)->p_data, 3);
    CU_ASSERT_PTR_NULL(dl_at(p_list, 3U));

    // Verify find
    int key1 = 1, key2 = 2, key3 = 3, key4 = 4;
    CU_ASSERT_EQUAL(dl_find(p_list, &key1), 0);
    CU_ASSERT_EQUAL(dl_find(p_list, &key2), 1);
    CU_ASSERT_EQUAL(dl_find(p_list, &key3), 2);
    CU_ASSERT_EQUAL(dl_find(p_list, &key4), DL_NOT_FOUND);

    dl_destroy(p_list);
}

/**
 * @brief Test foreach by multiplying all values by 5.
 */
static void
test_dl_foreach_clone (void)
{
    dl_t *p_list = dl_create(delete_int, compare_ints, print_int);

    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    int *c = malloc(sizeof(int));
    *a     = 2;
    *b     = 4;
    *c     = 6;

    dl_insert(p_list, a, 0); // 2
    dl_insert(p_list, b, 1); // 4
    dl_insert(p_list, c, 2); // 6

    // Test foreach
    CU_ASSERT_EQUAL(dl_foreach(p_list, multiply_by_five), 0);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 0)->p_data, 10);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 1)->p_data, 20);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 2)->p_data, 30);

    // Test deep clone: should have same elements but different pointers
    dl_t *p_clone = dl_clone(p_list, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_clone);
    CU_ASSERT_EQUAL(dl_size(p_clone), dl_size(p_list));

    CU_ASSERT_PTR_NOT_EQUAL(dl_at(p_list, 0)->p_data, dl_at(p_clone, 0)->p_data);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 0)->p_data, *(int *)dl_at(p_clone, 0)->p_data);
    CU_ASSERT_PTR_NOT_EQUAL(dl_at(p_list, 1)->p_data, dl_at(p_clone, 1)->p_data);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 1)->p_data, *(int *)dl_at(p_clone, 1)->p_data);
    CU_ASSERT_PTR_NOT_EQUAL(dl_at(p_list, 2)->p_data, dl_at(p_clone, 2)->p_data);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 2)->p_data, *(int *)dl_at(p_clone, 2)->p_data);

    dl_destroy(p_list);
    dl_destroy(p_clone);
}

/**
 * @brief   Test reverse, update and swap operations.
 */
static void
test_dll_reverse_swap_update (void)
{
    dl_t *p_list = dl_create(delete_int, compare_ints, print_int);

    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    int *c = malloc(sizeof(int));
    *a     = 1;
    *b     = 2;
    *c     = 3;

    // List: 1 -> 2 -> 3
    dl_insert(p_list, a, 0U);
    dl_insert(p_list, b, 1U);
    dl_insert(p_list, c, 2U);

    // Reverse: 3 -> 2 -> 1
    CU_ASSERT_EQUAL(dl_reverse(p_list), 0);
    CU_ASSERT_EQUAL(dl_at(p_list, 0U)->p_prev, NULL);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 0U)->p_data, 3);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 0U)->p_next->p_data, 2);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 1U)->p_prev->p_data, 3);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 1U)->p_data, 2);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 1U)->p_next->p_data, 1);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 2U)->p_prev->p_data, 2);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 2U)->p_data, 1);
    CU_ASSERT_EQUAL(dl_at(p_list, 2U)->p_next, NULL);

    // Swap: 1 -> 2 -> 3
    CU_ASSERT_EQUAL(dl_swap(p_list, 0U, 2U), DL_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 0U)->p_data, 1);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 2U)->p_data, 3);

    // Update: 1 -> 42 -> 3
    int *new_val = malloc(sizeof(int));
    *new_val     = 42;
    CU_ASSERT_EQUAL(dl_update(p_list, 1U, new_val), DL_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 1U)->p_prev->p_data, 1);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 1U)->p_next->p_data, 3);
    CU_ASSERT_EQUAL(*(int *)dl_at(p_list, 1U)->p_data, 42);

    dl_destroy(p_list);
}

/**
 * @brief   Test clear and is_empty behavior.
 */
static void
test_dll_null_invalid_inputs (void)
{
    CU_ASSERT_EQUAL(dl_insert(NULL, NULL, 0U), DL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(dl_prepend(NULL, NULL), DL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(dl_del_at(NULL, 0U), DL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(dl_at(NULL, 0U), NULL);
    CU_ASSERT_EQUAL(dl_find(NULL, NULL), DL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(dl_size(NULL), DL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(dl_reverse(NULL), DL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(dl_swap(NULL, 0U, 1U), DL_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(dl_update(NULL, 0U, NULL), DL_INVALID_ARGUMENT);
}

/*** end of file ***/
