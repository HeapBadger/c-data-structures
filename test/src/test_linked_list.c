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
static void test_linked_list_swap(void);
static void test_linked_list_update(void);
static void test_linked_list_extreme_cases(void);
static void test_linked_list_null(void);
static void print_int(void *p_data);
static void delete_int(void *p_data);
static int  compare_ints(void *p_lhs, void *p_rhs);

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
        == (CU_add_test(suite, "test_linked_list_swap", test_linked_list_swap)))
    {
        ERROR_LOG("Failed to add test_linked_list_swap to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_linked_list_update", test_linked_list_update)))
    {
        ERROR_LOG("Failed to add test_linked_list_update to suite\n");
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
    p_test = linked_list_create(delete_int, compare_ints, print_int);

    // Test case 1: Preappend value when list is empty
    int *value_1 = calloc(1, sizeof(int));
    *value_1     = 5;
    CU_ASSERT_EQUAL(linked_list_preappend(p_test, value_1),
                    LINKED_LIST_SUCCESS); // Expect only "5"

    ll_size = linked_list_size(p_test);
    CU_ASSERT_EQUAL(ll_size, 1);

    p_node = linked_list_at(p_test, 0);
    CU_ASSERT_EQUAL(0, compare_ints(p_node->p_data, (void *)value_1));

    // Test case 2: Preappend value when a list exists
    int *value_2 = calloc(1, sizeof(int));
    *value_2     = 4;
    CU_ASSERT_EQUAL(linked_list_preappend(p_test, value_2),
                    LINKED_LIST_SUCCESS); // Expect "4 5"

    ll_size = linked_list_size(p_test);
    CU_ASSERT_EQUAL(ll_size, 2);

    p_node = linked_list_at(p_test, 0);
    CU_ASSERT_EQUAL(0, compare_ints(p_node->p_data, (void *)value_2));
    p_node = linked_list_at(p_test, 1);
    CU_ASSERT_EQUAL(0, compare_ints(p_node->p_data, (void *)value_1));

    // Test case 3: Insert value at end of list
    int *value_3 = calloc(1, sizeof(int));
    *value_3     = 10;
    CU_ASSERT_EQUAL(linked_list_insert(p_test, value_3, 2),
                    LINKED_LIST_SUCCESS); // Expect "4 5 10"

    ll_size = linked_list_size(p_test);
    CU_ASSERT_EQUAL(ll_size, 3);

    p_node = linked_list_at(p_test, 0);
    CU_ASSERT_EQUAL(0, compare_ints(p_node->p_data, (void *)value_2));
    p_node = linked_list_at(p_test, 1);
    CU_ASSERT_EQUAL(0, compare_ints(p_node->p_data, (void *)value_1));
    p_node = linked_list_at(p_test, 2);
    CU_ASSERT_EQUAL(0, compare_ints(p_node->p_data, (void *)value_3));

    // Test case 4: Insert value in the middle of the list
    int *value_4 = calloc(1, sizeof(int));
    *value_4     = 23;
    CU_ASSERT_EQUAL(linked_list_insert(p_test, value_4, 2),
                    LINKED_LIST_SUCCESS); // Expect "4 5 23 10"

    ll_size = linked_list_size(p_test);
    CU_ASSERT_EQUAL(ll_size, 4);

    p_node = linked_list_at(p_test, 0);
    CU_ASSERT_EQUAL(0, compare_ints(p_node->p_data, (void *)value_2));
    p_node = linked_list_at(p_test, 1);
    CU_ASSERT_EQUAL(0, compare_ints(p_node->p_data, (void *)value_1));
    p_node = linked_list_at(p_test, 2);
    CU_ASSERT_EQUAL(0, compare_ints(p_node->p_data, (void *)value_4));
    p_node = linked_list_at(p_test, 3);
    CU_ASSERT_EQUAL(0, compare_ints(p_node->p_data, (void *)value_3));

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
 *
 * @return  None.
 */
static void
test_linked_list_create (void)
{
    linked_list_t *p_list
        = linked_list_create(delete_int, compare_ints, print_int);
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
 *
 * @return  None.
 */
static void
test_linked_list_destroy (void)
{
    linked_list_t *p_list
        = linked_list_create(delete_int, compare_ints, print_int);
    int *value = calloc(1, sizeof(int));
    *value     = 5;
    CU_ASSERT_EQUAL(linked_list_preappend(p_list, value), LINKED_LIST_SUCCESS);

    linked_list_destroy(p_list);
}

/**
 * @brief Tests prepending a node to an empty linked list.
 *
 * This test verifies that a new node can be prepended to an empty linked list.
 * It checks that the new node becomes the head of the list and that the list
 * size is updated correctly.
 *
 * @return  None.
 */
static void
test_linked_list_preappend (void)
{
    linked_list_t *p_test = NULL;
    int           *value  = calloc(1, sizeof(int));
    *value                = 7;

    p_test = linked_list_create(delete_int, compare_ints, print_int);
    CU_ASSERT_EQUAL(linked_list_preappend(p_test, value), LINKED_LIST_SUCCESS);

    CU_ASSERT_PTR_NOT_NULL(p_test);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 0)->p_data, 7);

    linked_list_node_t *node0 = linked_list_at(p_test, 0);
    CU_ASSERT_PTR_NULL(node0->p_next); // node 0 should have no next node

    // Clean up
    linked_list_destroy(p_test);
}

/**
 * @brief Tests inserting a node at a specific index in the linked list.
 *
 * This test verifies that a new node can be inserted at a specified index in
 * a linked list. It checks insertion at the head, at the end, and in the middle
 * of the list, ensuring that the list is correctly updated and the size is
 * accurate.
 *
 * @return  None.
 */
static void
test_linked_list_insert (void)
{
    linked_list_t *p_test = NULL;
    int           *value0 = calloc(1, sizeof(int));
    int           *value1 = calloc(1, sizeof(int));
    int           *value2 = calloc(1, sizeof(int));
    int           *value3 = calloc(1, sizeof(int));
    int           *value4 = calloc(1, sizeof(int));
    *value0               = 1;
    *value1               = 1;
    *value2               = 2;
    *value3               = 3;
    *value4               = 4;

    p_test = linked_list_create(delete_int, compare_ints, print_int);
    CU_ASSERT_EQUAL(linked_list_preappend(p_test, value0),
                    LINKED_LIST_SUCCESS); // List: 1
    CU_ASSERT_EQUAL(linked_list_preappend(p_test, value1),
                    LINKED_LIST_SUCCESS); // List: 1 1
    CU_ASSERT_EQUAL(linked_list_insert(p_test, value2, 2),
                    LINKED_LIST_SUCCESS); // List: 1 1 2
    CU_ASSERT_EQUAL(linked_list_insert(p_test, value3, 2),
                    LINKED_LIST_SUCCESS); // List: 1 1 3 2
    CU_ASSERT_EQUAL(linked_list_insert(p_test, value4, 4),
                    LINKED_LIST_SUCCESS); // List: 1 1 3 2 4

    // Verify the values at each position
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 0)->p_data, 1);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 1)->p_data, 1);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 2)->p_data, 3);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 3)->p_data, 2);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 4)->p_data, 4);

    // Verify p_next pointers
    linked_list_node_t *node0 = linked_list_at(p_test, 0);
    linked_list_node_t *node1 = linked_list_at(p_test, 1);
    linked_list_node_t *node2 = linked_list_at(p_test, 2);
    linked_list_node_t *node3 = linked_list_at(p_test, 3);
    linked_list_node_t *node4 = linked_list_at(p_test, 4);

    // Check node 0
    CU_ASSERT_PTR_EQUAL(node0->p_next, node1); // node 0's next should be node 1

    // Check node 1
    CU_ASSERT_PTR_EQUAL(node1->p_next, node2); // node 1's next should be node 2

    // Check node 2
    CU_ASSERT_PTR_EQUAL(node2->p_next, node3); // node 2's next should be node 3

    // Check node 3
    CU_ASSERT_PTR_EQUAL(node3->p_next, node4); // node 3's next should be node 4

    // Check node 4
    CU_ASSERT_PTR_NULL(node4->p_next); // node 4 should have no next node

    // Clean up
    linked_list_destroy(p_test);
}

/**
 * @brief Tests deleting a node at a specific index in the linked list.
 *
 * This test verifies that a node can be deleted from a linked list at a
 * specified index. It ensures that the list is updated correctly, including
 * handling deletion of the head node and nodes in the middle or end of the
 * list.
 *
 * @return  None.
 */
static void
test_linked_list_del_at (void)
{
    linked_list_t *p_test = NULL;
    int           *value1 = calloc(1, sizeof(int));
    int           *value2 = calloc(1, sizeof(int));
    int           *value3 = calloc(1, sizeof(int));
    int           *value4 = calloc(1, sizeof(int));
    int           *value5 = calloc(1, sizeof(int));
    *value1               = 1;
    *value2               = 2;
    *value3               = 3;
    *value4               = 4;
    *value5               = 5;

    p_test = linked_list_create(delete_int, compare_ints, print_int);
    CU_ASSERT_EQUAL(linked_list_preappend(p_test, value1),
                    LINKED_LIST_SUCCESS); // List: 1
    CU_ASSERT_EQUAL(linked_list_insert(p_test, value2, 1),
                    LINKED_LIST_SUCCESS); // List: 1 2
    CU_ASSERT_EQUAL(linked_list_insert(p_test, value3, 2),
                    LINKED_LIST_SUCCESS); // List: 1 2 3
    CU_ASSERT_EQUAL(linked_list_insert(p_test, value4, 3),
                    LINKED_LIST_SUCCESS); // List: 1 2 3 4
    CU_ASSERT_EQUAL(linked_list_insert(p_test, value5, 4),
                    LINKED_LIST_SUCCESS); // List: 1 2 3 4 5

    // Test deletion at the middle index
    CU_ASSERT_EQUAL(linked_list_del_at(p_test, 2),
                    LINKED_LIST_SUCCESS); // Remove 3, List: 1 2 4 5
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 0)->p_data, 1);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 1)->p_data, 2);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 2)->p_data, 4);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 3)->p_data, 5);

    // Verify p_next pointer after middle deletion
    linked_list_node_t *node1 = linked_list_at(p_test, 0);
    linked_list_node_t *node2 = linked_list_at(p_test, 1);
    linked_list_node_t *node3 = linked_list_at(p_test, 2);
    linked_list_node_t *node4 = linked_list_at(p_test, 3);

    CU_ASSERT_PTR_EQUAL(node1->p_next, node2); // node1's next should be node2
    CU_ASSERT_PTR_EQUAL(node2->p_next, node3); // node2's next should be node3
    CU_ASSERT_PTR_EQUAL(node3->p_next, node4); // node3's next should be node4
    CU_ASSERT_PTR_NULL(node4->p_next);         // node4 should have no next node

    // Test deletion at the first index
    CU_ASSERT_EQUAL(linked_list_del_at(p_test, 0),
                    LINKED_LIST_SUCCESS); // Remove 1, List: 2 4 5
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 0)->p_data, 2);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 1)->p_data, 4);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 2)->p_data, 5);

    // Verify p_next pointer after first deletion
    node2 = linked_list_at(p_test, 0);
    node3 = linked_list_at(p_test, 1);
    node4 = linked_list_at(p_test, 2);

    CU_ASSERT_PTR_EQUAL(node2->p_next, node3); // node2's next should be node3
    CU_ASSERT_PTR_EQUAL(node3->p_next, node4); // node3's next should be node4
    CU_ASSERT_PTR_NULL(node4->p_next);         // node4 should have no next node

    // Test deletion at the last index
    CU_ASSERT_EQUAL(linked_list_del_at(p_test, 2),
                    LINKED_LIST_SUCCESS); // Remove 5, List: 2 4
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 0)->p_data, 2);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 1)->p_data, 4);

    // Verify p_next pointer after last deletion
    node2 = linked_list_at(p_test, 0);
    node3 = linked_list_at(p_test, 1);

    CU_ASSERT_PTR_EQUAL(node2->p_next, node3); // node2's next should be node3
    CU_ASSERT_PTR_NULL(node3->p_next);         // node3 should have no next node

    // Clean up
    linked_list_destroy(p_test);
}

/**
 * @brief Tests retrieving a node at a specific index in the linked list.
 *
 * This test verifies that a node can be correctly retrieved from a linked list
 * at a specified index. It checks that the node's data matches the expected
 * values and handles edge cases such as out-of-bounds indices.
 *
 * @return  None.
 */
static void
test_linked_list_at (void)
{
    linked_list_t *p_list
        = linked_list_create(delete_int, compare_ints, print_int);
    int *value1 = calloc(1, sizeof(int));
    *value1     = 5;
    int *value2 = calloc(1, sizeof(int));
    *value2     = 10;

    CU_ASSERT_EQUAL(linked_list_preappend(p_list, value1),
                    LINKED_LIST_SUCCESS); // List: 5
    CU_ASSERT_EQUAL(linked_list_preappend(p_list, value2),
                    LINKED_LIST_SUCCESS); // List: 10 5

    linked_list_node_t *p_node = linked_list_at(p_list, 0);
    CU_ASSERT_EQUAL(0, compare_ints(p_node->p_data, (void *)value2));
    p_node = linked_list_at(p_list, 1);
    CU_ASSERT_EQUAL(0, compare_ints(p_node->p_data, (void *)value1));

    linked_list_destroy(p_list);
}

/**
 * @brief Tests finding the index of a node containing specific data.
 *
 * This test verifies that the index of a node containing the specified data
 * can be found in the linked list. It checks the correctness of the index
 * returned by the `linked_list_find` function for existing and non-existing
 * data.
 *
 * @return  None.
 */
static void
test_linked_list_find (void)
{
    linked_list_t *p_list
        = linked_list_create(delete_int, compare_ints, print_int);
    int *value1 = calloc(1, sizeof(int));
    *value1     = 5;
    int *value2 = calloc(1, sizeof(int));
    *value2     = 10;

    CU_ASSERT_EQUAL(linked_list_preappend(p_list, value1),
                    LINKED_LIST_SUCCESS); // List: 5
    CU_ASSERT_EQUAL(linked_list_preappend(p_list, value2),
                    LINKED_LIST_SUCCESS); // List: 10 5

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
 *
 * @return  None.
 */
static void
test_linked_list_size (void)
{
    linked_list_t *p_list
        = linked_list_create(delete_int, compare_ints, print_int);
    int *value1 = calloc(1, sizeof(int));
    *value1     = 5;
    int *value2 = calloc(1, sizeof(int));
    *value2     = 10;

    CU_ASSERT_EQUAL(linked_list_preappend(p_list, value1),
                    LINKED_LIST_SUCCESS); // List: 5
    CU_ASSERT_EQUAL(linked_list_preappend(p_list, value2),
                    LINKED_LIST_SUCCESS); // List: 10 5

    CU_ASSERT_EQUAL(linked_list_size(p_list), 2);

    CU_ASSERT_EQUAL(linked_list_insert(p_list, calloc(1, sizeof(int)), 2),
                    LINKED_LIST_SUCCESS); // List: 10 5 X
    CU_ASSERT_EQUAL(linked_list_size(p_list), 3);

    linked_list_destroy(p_list);
}

/**
 * @brief Tests reversing a linked list.
 *
 * This test verifies that the linked list is successfully reversed by checking
 * if the order of elements is correctly inverted after the reversal operation.
 *
 * @return  None.
 */
static void
test_linked_list_reverse (void)
{
    linked_list_t *p_test = NULL;
    int           *value1 = calloc(1, sizeof(int));
    int           *value2 = calloc(1, sizeof(int));
    int           *value3 = calloc(1, sizeof(int));
    *value1               = 1;
    *value2               = 2;
    *value3               = 3;

    p_test = linked_list_create(delete_int, compare_ints, print_int);
    CU_ASSERT_EQUAL(linked_list_preappend(p_test, value1),
                    LINKED_LIST_SUCCESS); // List: 1
    CU_ASSERT_EQUAL(linked_list_insert(p_test, value2, 1),
                    LINKED_LIST_SUCCESS); // List: 1 2
    CU_ASSERT_EQUAL(linked_list_insert(p_test, value3, 1),
                    LINKED_LIST_SUCCESS); // List: 1 3 2

    CU_ASSERT_EQUAL(linked_list_reverse(p_test),
                    LINKED_LIST_SUCCESS); // Reversed List: 2 3 1

    // Verify the values at each position
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 0)->p_data, 2);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 1)->p_data, 3);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 2)->p_data, 1);

    // Verify p_next pointers
    linked_list_node_t *node0 = linked_list_at(p_test, 0);
    linked_list_node_t *node1 = linked_list_at(p_test, 1);
    linked_list_node_t *node2 = linked_list_at(p_test, 2);

    // Check node 0 (should be the new head)
    CU_ASSERT_PTR_EQUAL(node0->p_next, node1); // node 0's next should be node 1

    // Check node 1
    CU_ASSERT_PTR_EQUAL(node1->p_next, node2); // node 1's next should be node 2

    // Check node 2 (should be the new tail)
    CU_ASSERT_PTR_NULL(node2->p_next); // node 2 should have no next node

    // Clean up
    linked_list_destroy(p_test);
}

/**
 * @brief Tests swapping two nodes in a linked list.
 *
 * This test verifies that two nodes can successfully have
 * their data swapped in a list.
 *
 * @return  None.
 */
void
test_linked_list_swap (void)
{
    // Create a new linked list
    linked_list_t *p_test
        = linked_list_create(delete_int, compare_ints, print_int);

    // Add some elements to the list
    int *value0 = calloc(1, sizeof(int));
    int *value1 = calloc(1, sizeof(int));
    int *value2 = calloc(1, sizeof(int));
    *value0     = 1;
    *value1     = 2;
    *value2     = 3;
    linked_list_insert(p_test, value0, 0);
    linked_list_insert(p_test, value1, 1);
    linked_list_insert(p_test, value2, 2);

    // Swap elements at index 0 and 2
    int result = linked_list_swap(p_test, 0, 2);
    CU_ASSERT_EQUAL(result, LINKED_LIST_SUCCESS);

    // Check that the elements have been swapped
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 0)->p_data, 3);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 2)->p_data, 1);

    // Clean up
    linked_list_destroy(p_test);
}

/**
 * @brief Tests updating a node in a linked list.
 *
 * This test verifies that a node's data can be updated
 * within a list.
 *
 * @return  None.
 */
static void
test_linked_list_update (void)
{
    // Create a new linked list
    linked_list_t *p_test
        = linked_list_create(delete_int, compare_ints, print_int);

    // Add some elements to the list
    int *value0 = calloc(1, sizeof(int));
    int *value1 = calloc(1, sizeof(int));
    int *value2 = calloc(1, sizeof(int));
    *value0     = 1;
    *value1     = 2;
    *value2     = 3;
    linked_list_insert(p_test, value0, 0);
    linked_list_insert(p_test, value1, 1);
    linked_list_insert(p_test, value2, 2);

    // Update element
    int *update = calloc(1, sizeof(int));
    *update     = 50;
    int result  = linked_list_update(p_test, 1, update);
    CU_ASSERT_EQUAL(result, LINKED_LIST_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_test, 1)->p_data, 50);

    // Clean up
    linked_list_destroy(p_test);
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
    linked_list_t *p_list
        = linked_list_create(delete_int, compare_ints, print_int);
    int *value = NULL;
    int  i;

    // Test 1: Large Number of Elements
    for (i = 0; i < 10000; ++i)
    {
        value  = calloc(1, sizeof(int));
        *value = i;
        CU_ASSERT_EQUAL(linked_list_preappend(p_list, value),
                        LINKED_LIST_SUCCESS);
    }

    CU_ASSERT_EQUAL(linked_list_size(p_list), 10000);

    // Reverse the list
    CU_ASSERT_EQUAL(linked_list_reverse(p_list), LINKED_LIST_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_list, 0)->p_data, 0);
    CU_ASSERT_EQUAL(*(int *)linked_list_at(p_list, 9999)->p_data, 9999);

    // Test 2: Boundary Cases - Accessing Out-of-Bounds Indices
    linked_list_node_t *p_node = linked_list_at(p_list, 9999);
    CU_ASSERT_PTR_NOT_NULL(p_node);
    p_node = linked_list_at(p_list, -10); // Should be NULL or error
    CU_ASSERT_PTR_NULL(p_node);
    p_node = linked_list_at(p_list, 10000); // Should be NULL or error
    CU_ASSERT_PTR_NULL(p_node);
    int index = linked_list_swap(p_list, -10, 5);
    CU_ASSERT_EQUAL(index, LINKED_LIST_OUT_OF_BOUNDS);
    index = linked_list_swap(p_list, 5, -10);
    CU_ASSERT_EQUAL(index, LINKED_LIST_OUT_OF_BOUNDS);
    index = linked_list_swap(p_list, 5, 10000);
    CU_ASSERT_EQUAL(index, LINKED_LIST_OUT_OF_BOUNDS);
    index = linked_list_swap(p_list, 10000, 5);
    CU_ASSERT_EQUAL(index, LINKED_LIST_OUT_OF_BOUNDS);
    value  = calloc(1, sizeof(int));
    *value = 10;
    index  = linked_list_update(p_list, -5, value);
    CU_ASSERT_EQUAL(index, LINKED_LIST_OUT_OF_BOUNDS);
    free(value);
    value  = calloc(1, sizeof(int));
    *value = 10;
    index  = linked_list_update(p_list, 10000, value);
    CU_ASSERT_EQUAL(index, LINKED_LIST_OUT_OF_BOUNDS);
    free(value);

    // Test 3: Insertion at Invalid Position
    value  = calloc(1, sizeof(int));
    *value = 10;
    CU_ASSERT_EQUAL(linked_list_insert(p_list, value, 20000),
                    LINKED_LIST_OUT_OF_BOUNDS); // Should be handled gracefully
    CU_ASSERT_PTR_NOT_NULL(p_list);
    value  = calloc(1, sizeof(int));
    *value = 20;
    CU_ASSERT_EQUAL(linked_list_insert(p_list, value, -10),
                    LINKED_LIST_OUT_OF_BOUNDS); // inserting at negative index
    CU_ASSERT_PTR_NOT_NULL(p_list);

    // Test 4: Deletion at Invalid Position
    CU_ASSERT_EQUAL(linked_list_del_at(p_list, 10000),
                    LINKED_LIST_OUT_OF_BOUNDS); // Deleting out-of-bounds
    CU_ASSERT_PTR_NOT_NULL(p_list);
    CU_ASSERT_EQUAL(linked_list_del_at(p_list, -10),
                    LINKED_LIST_OUT_OF_BOUNDS); // Deleting negative index
    CU_ASSERT_PTR_NOT_NULL(p_list);

    // Test 5: Memory Management
    linked_list_destroy(p_list); // Check if all memory is properly freed
    p_list = NULL;

    // Test 6: Performance - Insert and Delete in Bulk
    p_list = linked_list_create(delete_int, compare_ints, print_int);
    for (i = 0; i < 10000; ++i)
    {
        value  = calloc(1, sizeof(int));
        *value = i;
        CU_ASSERT_EQUAL(linked_list_preappend(p_list, value),
                        LINKED_LIST_SUCCESS);
    }

    for (i = 0; i < 10000; ++i)
    {
        CU_ASSERT_EQUAL(linked_list_del_at(p_list, 0),
                        LINKED_LIST_SUCCESS); // Deleting head each time
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
    CU_ASSERT_EQUAL(linked_list_preappend(NULL, NULL),
                    LINKED_LIST_INVALID_ARGUMENT);
    CU_ASSERT_PTR_NULL(p_list);

    // Test 3: Attempt to insert NULL value into a non-existent list
    CU_ASSERT_EQUAL(linked_list_insert(NULL, NULL, 0),
                    LINKED_LIST_INVALID_ARGUMENT);
    CU_ASSERT_PTR_NULL(p_list);

    // Test 4: Attempt to delete at position from a NULL list
    CU_ASSERT_EQUAL(linked_list_del_at(NULL, 0), LINKED_LIST_INVALID_ARGUMENT);
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
    CU_ASSERT_EQUAL(linked_list_reverse(NULL), LINKED_LIST_INVALID_ARGUMENT);
    CU_ASSERT_PTR_NULL(p_list);

    // Test 9: Attempt to print a NULL list
    linked_list_print(p_list); // Should handle gracefully without crashing

    // Test 10: Attempt to find an index for a NULL list
    int index = linked_list_find(NULL, NULL);
    CU_ASSERT_EQUAL(index, LINKED_LIST_INVALID_ARGUMENT);

    // Test 11: Attempt to swap elements in a NULL list
    index = linked_list_swap(NULL, 0, 1);
    CU_ASSERT_EQUAL(index, LINKED_LIST_INVALID_ARGUMENT);

    // Test 12: Attempt to update node in a NULL list
    index = linked_list_update(NULL, 0, NULL);
    CU_ASSERT_EQUAL(index, LINKED_LIST_INVALID_ARGUMENT);
}

// Function to delete an integer (free the memory)
static void
delete_int (void *p_data)
{
    free(p_data);
    p_data = NULL;
    return;
}

// Function to compare two integers
static int
compare_ints (void *p_lhs, void *p_rhs)
{
    int lhs = *(int *)p_lhs;
    int rhs = *(int *)p_rhs;
    return (lhs > rhs) - (lhs < rhs);
}

// Function to print an integer
static void
print_int (void *p_data)
{
    printf("%d ", *(int *)p_data);
    return;
}
