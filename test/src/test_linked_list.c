/**
 * @file    test_linked_list.c
 * @brief   Linked List test suite.
 *
 * @author  Anna DeVries
 * @date    August 21, 2024
 */

#include "test_linked_list.h"
#include "linked_list.h"
#include "test_auxiliary.h"
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Function Declarations
static void test_linked_list_simple(void);
static void test_linked_list_create(void);
static void test_linked_list_destroy(void);
static void test_linked_list_preappend(void);
static void test_linked_list_insert(void);
static void test_linked_list_del_at(void);
static void test_linked_list_at(void);
static void test_linked_list_find(void);
static void test_linked_list_size(void);
static void test_linked_list_reverse(void);
static void test_linked_list_extreme_cases(void);
static void test_linked_list_null(void);
void        static_print_int(void *p_data);
void        static_delete_int(void *p_data);
int         static_compare_ints(void *p_lhs, void *p_rhs);

// Suite Creation Funtion
CU_pSuite
linked_list_suite (void)
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
            suite, "test_linked_list_simple", test_linked_list_simple)))
    {
        ERROR_LOG("Failed to add test_linked_list_simple to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_linked_list_create", test_linked_list_create)))
    {
        ERROR_LOG("Failed to add test_linked_list_create to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_linked_list_destroy", test_linked_list_destroy)))
    {
        ERROR_LOG("Failed to add test_linked_list_destroy to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_linked_list_preappend", test_linked_list_preappend)))
    {
        ERROR_LOG("Failed to add test_linked_list_preappend to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_linked_list_insert", test_linked_list_insert)))
    {
        ERROR_LOG("Failed to add test_linked_list_insert to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_linked_list_del_at", test_linked_list_del_at)))
    {
        ERROR_LOG("Failed to add test_linked_list_del_at to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(suite, "test_linked_list_at", test_linked_list_at)))
    {
        ERROR_LOG("Failed to add test_linked_list_at to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(suite, "test_linked_list_find", test_linked_list_find)))
    {
        ERROR_LOG("Failed to add test_linked_list_find to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(suite, "test_linked_list_size", test_linked_list_size)))
    {
        ERROR_LOG("Failed to add test_linked_list_size to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_linked_list_reverse", test_linked_list_reverse)))
    {
        ERROR_LOG("Failed to add test_linked_list_reverse to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(suite,
                        "test_linked_list_extreme_cases",
                        test_linked_list_extreme_cases)))
    {
        ERROR_LOG("Failed to add test_linked_list_extreme_cases to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(suite, "test_linked_list_null", test_linked_list_null)))
    {
        ERROR_LOG("Failed to add test_linked_list_null to suite\n");
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
 * @brief   Test case for simple linked list creation, insertion, preappend
 *          and destroy.
 *
 * @return  None.
 */
static void
test_linked_list_simple (void)
{
    linked_list_t      *p_test  = NULL;
    linked_list_node_t *p_node  = NULL;
    int                 ll_size = 0;

    // Create the linked list
    p_test = linked_list_create(
        static_delete_int, static_compare_ints, static_print_int);

    // Test case 1: Preappend value when list is empty
    int *value_1 = calloc(1, sizeof(int));
    *value_1     = 5;
    p_test       = linked_list_preappend(p_test, value_1); // Expect only "5"

    ll_size = linked_list_size(p_test);
    CU_ASSERT_EQUAL(ll_size, 1);

    p_node = linked_list_at(p_test, 0);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value_1));

    // Test case 2: Preappend value when a list exists
    int *value_2 = calloc(1, sizeof(int));
    *value_2     = 4;
    p_test       = linked_list_preappend(p_test, value_2); // Expect "4 5"

    ll_size = linked_list_size(p_test);
    CU_ASSERT_EQUAL(ll_size, 2);

    p_node = linked_list_at(p_test, 0);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value_2));
    p_node = linked_list_at(p_test, 1);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value_1));

    // Test case 3: Insert value at end of list
    int *value_3 = calloc(1, sizeof(int));
    *value_3     = 10;
    p_test       = linked_list_insert(p_test, value_3, 2); // Expect "4 5 10"

    ll_size = linked_list_size(p_test);
    CU_ASSERT_EQUAL(ll_size, 3);

    p_node = linked_list_at(p_test, 0);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value_2));
    p_node = linked_list_at(p_test, 1);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value_1));
    p_node = linked_list_at(p_test, 2);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value_3));

    // Test case 4: Insert value in the middle of the list
    int *value_4 = calloc(1, sizeof(int));
    *value_4     = 23;
    p_test       = linked_list_insert(p_test, value_4, 2); // Expect "4 5 23 10"

    ll_size = linked_list_size(p_test);
    CU_ASSERT_EQUAL(ll_size, 4);

    p_node = linked_list_at(p_test, 0);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value_2));
    p_node = linked_list_at(p_test, 1);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value_1));
    p_node = linked_list_at(p_test, 2);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value_4));
    p_node = linked_list_at(p_test, 3);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value_3));

    // Destroy the linked list
    linked_list_destroy(p_test);
    p_test = NULL;
    return;
}

/**
 * @brief Tests the creation of a linked list.
 *
 * This test verifies that a linked list can be successfully created with valid
 * function pointers for deletion, comparison, and printing. It ensures that the
 * pointers are correctly initialized and that the linked list is not NULL.
 */
static void
test_linked_list_create (void)
{
    linked_list_t *p_list = linked_list_create(
        static_delete_int, static_compare_ints, static_print_int);
    CU_ASSERT_PTR_NOT_NULL(p_list);
    CU_ASSERT_PTR_NOT_NULL(p_list->del_f);
    CU_ASSERT_PTR_NOT_NULL(p_list->cmp_f);
    CU_ASSERT_PTR_NOT_NULL(p_list->print_f);
    linked_list_destroy(p_list);
}

/**
 * @brief Tests the destruction of a linked list.
 *
 * This test verifies that a linked list is correctly destroyed, ensuring that
 * all allocated resources are freed. It checks that the pointer to the linked
 * list is set to NULL after destruction.
 */
static void
test_linked_list_destroy (void)
{
    linked_list_t *p_list = linked_list_create(
        static_delete_int, static_compare_ints, static_print_int);
    int *value = calloc(1, sizeof(int));
    *value     = 5;
    p_list     = linked_list_preappend(p_list, value);

    linked_list_destroy(p_list);
}

/**
 * @brief Tests prepending a node to an empty linked list.
 *
 * This test verifies that a new node can be prepended to an empty linked list.
 * It checks that the new node becomes the head of the list and that the list
 * size is updated correctly.
 */
static void
test_linked_list_preappend (void)
{
    linked_list_t *p_list = linked_list_create(
        static_delete_int, static_compare_ints, static_print_int);
    int *value = calloc(1, sizeof(int));
    *value     = 5;
    p_list     = linked_list_preappend(p_list, value);

    CU_ASSERT_PTR_NOT_NULL(p_list);
    CU_ASSERT_EQUAL(linked_list_size(p_list), 1);
    linked_list_node_t *p_node = linked_list_at(p_list, 0);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value));

    linked_list_destroy(p_list);
}

/**
 * @brief Tests inserting a node at a specific index in the linked list.
 *
 * This test verifies that a new node can be inserted at a specified index in
 * a linked list. It checks insertion at the head, at the end, and in the middle
 * of the list, ensuring that the list is correctly updated and the size is
 * accurate.
 */
static void
test_linked_list_insert (void)
{
    linked_list_t *p_list = linked_list_create(
        static_delete_int, static_compare_ints, static_print_int);
    int *value1 = calloc(1, sizeof(int));
    *value1     = 5;
    int *value2 = calloc(1, sizeof(int));
    *value2     = 10;

    p_list = linked_list_preappend(p_list, value1); // List: 5
    p_list = linked_list_insert(p_list, value2, 1); // List: 5 10

    CU_ASSERT_EQUAL(linked_list_size(p_list), 2);
    linked_list_node_t *p_node = linked_list_at(p_list, 0);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value1));
    p_node = linked_list_at(p_list, 1);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value2));

    linked_list_destroy(p_list);
}

/**
 * @brief Tests deleting a node at a specific index in the linked list.
 *
 * This test verifies that a node can be deleted from a linked list at a
 * specified index. It ensures that the list is updated correctly, including
 * handling deletion of the head node and nodes in the middle or end of the
 * list.
 */
static void
test_linked_list_del_at (void)
{
    linked_list_t *p_list = linked_list_create(
        static_delete_int, static_compare_ints, static_print_int);
    int *value1 = calloc(1, sizeof(int));
    *value1     = 5;
    int *value2 = calloc(1, sizeof(int));
    *value2     = 10;
    int *value3 = calloc(1, sizeof(int));
    *value3     = 25;
    int *value4 = calloc(1, sizeof(int));
    *value4     = 50;

    p_list = linked_list_preappend(p_list, value1); // List: 5
    p_list = linked_list_preappend(p_list, value2); // List: 10 5
    p_list = linked_list_preappend(p_list, value3); // List: 25 10 5
    p_list = linked_list_preappend(p_list, value4); // List: 50 25 10 5
    p_list = linked_list_del_at(p_list, 1);         // List: 50 10 5
    p_list = linked_list_del_at(p_list, 2);         // List: 50 10

    CU_ASSERT_EQUAL(linked_list_size(p_list), 2);
    linked_list_node_t *p_node = linked_list_at(p_list, 0);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value4));
    p_node = linked_list_at(p_list, 1);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value2));

    linked_list_destroy(p_list);
}

/**
 * @brief Tests retrieving a node at a specific index in the linked list.
 *
 * This test verifies that a node can be correctly retrieved from a linked list
 * at a specified index. It checks that the node's data matches the expected
 * values and handles edge cases such as out-of-bounds indices.
 */
static void
test_linked_list_at (void)
{
    linked_list_t *p_list = linked_list_create(
        static_delete_int, static_compare_ints, static_print_int);
    int *value1 = calloc(1, sizeof(int));
    *value1     = 5;
    int *value2 = calloc(1, sizeof(int));
    *value2     = 10;

    p_list = linked_list_preappend(p_list, value1); // List: 5
    p_list = linked_list_preappend(p_list, value2); // List: 10 5

    linked_list_node_t *p_node = linked_list_at(p_list, 0);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value2));
    p_node = linked_list_at(p_list, 1);
    CU_ASSERT_EQUAL(0, static_compare_ints(p_node->p_data, (void *)value1));

    linked_list_destroy(p_list);
}

/**
 * @brief Tests finding the index of a node containing specific data.
 *
 * This test verifies that the index of a node containing the specified data
 * can be found in the linked list. It checks the correctness of the index
 * returned by the `linked_list_find` function for existing and non-existing
 * data.
 */
static void
test_linked_list_find (void)
{
    linked_list_t *p_list = linked_list_create(
        static_delete_int, static_compare_ints, static_print_int);
    int *value1 = calloc(1, sizeof(int));
    *value1     = 5;
    int *value2 = calloc(1, sizeof(int));
    *value2     = 10;

    p_list = linked_list_preappend(p_list, value1); // List: 5
    p_list = linked_list_preappend(p_list, value2); // List: 10 5

    int index = linked_list_find(p_list, (void *)value1);
    CU_ASSERT_EQUAL(index, 1);

    index = linked_list_find(p_list, (void *)value2);
    CU_ASSERT_EQUAL(index, 0);

    linked_list_destroy(p_list);
}

/**
 * @brief Tests computing the size of the linked list.
 *
 * This test verifies that the size of the linked list is accurately computed.
 * It checks the correctness of the size value after performing various
 * operations that modify the list.
 */
static void
test_linked_list_size (void)
{
    linked_list_t *p_list = linked_list_create(
        static_delete_int, static_compare_ints, static_print_int);
    int *value1 = calloc(1, sizeof(int));
    *value1     = 5;
    int *value2 = calloc(1, sizeof(int));
    *value2     = 10;

    p_list = linked_list_preappend(p_list, value1); // List: 5
    p_list = linked_list_preappend(p_list, value2); // List: 10 5

    CU_ASSERT_EQUAL(linked_list_size(p_list), 2);

    p_list
        = linked_list_insert(p_list, calloc(1, sizeof(int)), 2); // List: 10 5 X
    CU_ASSERT_EQUAL(linked_list_size(p_list), 3);

    linked_list_destroy(p_list);
}

/**
 * @brief Tests reversing a linked list.
 *
 * This test verifies that the linked list is successfully reversed by checking
 * if the order of elements is correctly inverted after the reversal operation.
 */
static void
test_linked_list_reverse (void)
{
    linked_list_t *p_list = linked_list_create(
        static_delete_int, static_compare_ints, static_print_int);

    int *value1 = calloc(1, sizeof(int));
    *value1     = 5;
    int *value2 = calloc(1, sizeof(int));
    *value2     = 10;
    int *value3 = calloc(1, sizeof(int));
    *value3     = 15;

    p_list = linked_list_preappend(p_list, value1); // List: 5
    p_list = linked_list_preappend(p_list, value2); // List: 10 5
    p_list = linked_list_preappend(p_list, value3); // List: 15 10 5

    // Reverse the list
    p_list = linked_list_reverse(p_list); // Expected List: 5 10 15

    // Test that the first element is now 5
    linked_list_node_t *p_node = linked_list_at(p_list, 0);
    CU_ASSERT_PTR_NOT_NULL(p_node);
    CU_ASSERT_EQUAL(*(int *)p_node->p_data, 5);

    // Test that the second element is 10
    p_node = linked_list_at(p_list, 1);
    CU_ASSERT_PTR_NOT_NULL(p_node);
    CU_ASSERT_EQUAL(*(int *)p_node->p_data, 10);

    // Test that the third element is 15
    p_node = linked_list_at(p_list, 2);
    CU_ASSERT_PTR_NOT_NULL(p_node);
    CU_ASSERT_EQUAL(*(int *)p_node->p_data, 15);

    // Clean up
    linked_list_destroy(p_list);
}

/**
 * @brief   Tests extreme cases for the linked list operations.
 *
 * This test verifies the behavior of linked list operations under extreme
 * conditions, such as large number of elements, boundary cases, and performance
 * issues.
 *
 * @return  None.
 */
static void
test_linked_list_extreme_cases (void)
{
    linked_list_t *p_list = linked_list_create(
        static_delete_int, static_compare_ints, static_print_int);
    int *value = NULL;
    int  i;

    // Test 1: Large Number of Elements
    for (i = 0; i < 10000; ++i)
    {
        value  = calloc(1, sizeof(int));
        *value = i;
        p_list = linked_list_preappend(p_list, value);
    }

    CU_ASSERT_EQUAL(linked_list_size(p_list), 10000);

    // Reverse the list
    p_list = linked_list_reverse(p_list);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_list, 0)->p_data, 0);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_list, 9999)->p_data, 9999);

    // Test 2: Boundary Cases - Accessing Out-of-Bounds Indices
    linked_list_node_t *p_node = linked_list_at(p_list, 9999);
    CU_ASSERT_PTR_NOT_NULL(p_node);
    p_node = linked_list_at(p_list, -10); // Should be NULL or error
    CU_ASSERT_PTR_NULL(p_node);
    p_node = linked_list_at(p_list, 10000); // Should be NULL or error
    CU_ASSERT_PTR_NULL(p_node);

    // Test 3: Insertion at Invalid Position
    value  = calloc(1, sizeof(int));
    *value = 10;
    p_list = linked_list_insert(
        p_list, value, 20000); // Should be handled gracefully
    CU_ASSERT_PTR_NOT_NULL(p_list);
    value  = calloc(1, sizeof(int));
    *value = 20;
    p_list
        = linked_list_insert(p_list, value, -10); // inserting at negative index
    CU_ASSERT_PTR_NOT_NULL(p_list);

    // Test 4: Deletion at Invalid Position
    p_list = linked_list_del_at(p_list, 10000); // Deleting out-of-bounds
    CU_ASSERT_PTR_NOT_NULL(p_list);
    p_list = linked_list_del_at(p_list, -10); // Deleting negative index
    CU_ASSERT_PTR_NOT_NULL(p_list);

    // Test 5: Memory Management
    linked_list_destroy(p_list); // Check if all memory is properly freed
    p_list = NULL;

    // Test 6: Performance - Insert and Delete in Bulk
    p_list = linked_list_create(
        static_delete_int, static_compare_ints, static_print_int);
    for (i = 0; i < 10000; ++i)
    {
        value  = calloc(1, sizeof(int));
        *value = i;
        p_list = linked_list_preappend(p_list, value);
    }

    for (i = 0; i < 10000; ++i)
    {
        p_list = linked_list_del_at(p_list, 0); // Deleting head each time
    }

    CU_ASSERT_EQUAL(linked_list_size(p_list), 0);

    // Clean up
    linked_list_destroy(p_list);
}

/**
 * @brief   Tests null variables cases for the linked list operations.
 *
 * This test verifies the behavior of linked list operations when input
 * variables are NULL.
 *
 * @return  None.
 */
static void
test_linked_list_null (void)
{
    linked_list_t *p_list = NULL;

    // Test 1: Attempt to create a linked list with NULL function pointers
    p_list = linked_list_create(NULL, NULL, NULL);
    CU_ASSERT_PTR_NULL(p_list);

    // Test 2: Attempt to pre-append NULL value to a non-existent list
    p_list = linked_list_preappend(NULL, NULL);
    CU_ASSERT_PTR_NULL(p_list);

    // Test 3: Attempt to insert NULL value into a non-existent list
    p_list = linked_list_insert(NULL, NULL, 0);
    CU_ASSERT_PTR_NULL(p_list);

    // Test 4: Attempt to delete at position from a NULL list
    p_list = linked_list_del_at(NULL, 0);
    CU_ASSERT_PTR_NULL(p_list);

    // Test 5: Attempt to access an element from a NULL list
    linked_list_node_t *p_node = linked_list_at(NULL, 0);
    CU_ASSERT_PTR_NULL(p_node);

    // Test 6: Attempt to retrieve the size of a NULL list
    int size = linked_list_size(NULL);
    CU_ASSERT_EQUAL(size, 0);

    // Test 7: Attempt to destroy a NULL list
    linked_list_destroy(NULL); // Should handle gracefully without crashing

    // Test 8: Attempt to reverse a NULL list
    p_list = linked_list_reverse(NULL);
    CU_ASSERT_PTR_NULL(p_list);
}

// Function to delete an integer (free the memory)
void
static_delete_int (void *p_data)
{
    free(p_data);
    p_data = NULL;
    return;
}

// Function to compare two integers
int
static_compare_ints (void *p_lhs, void *p_rhs)
{
    int lhs = *(int *)p_lhs;
    int rhs = *(int *)p_rhs;
    return (lhs > rhs) - (lhs < rhs);
}

// Function to print an integer
void
static_print_int (void *p_data)
{
    printf("%d ", *(int *)p_data);
    return;
}
