/**
 * @file    test_stack.c
 * @brief   Test suite for matrix.
 *
 * @author  heapbadger
 */

#include "test_stack.h"
#include "stack.h"
#include "test_auxiliary.h"
#include <CUnit/Basic.h>
#include <stdlib.h>

static void test_stack_create_destroy(void);
static void test_stack_push_pop_peek_size(void);
static void test_stack_clone(void);
static void test_stack_null_inputs(void);

CU_pSuite
stack_suite (void)
{
    CU_pSuite suite = NULL;
    suite           = CU_add_suite("matrix-suite", 0, 0);

    if (NULL == suite)
    {
        ERROR_LOG("Failed to add matrix-suite\n");
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(suite, "test_stack_create_destroy", test_stack_create_destroy)))
    {
        ERROR_LOG("Failed to add test_stack_create_destroy to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_stack_push_pop_peek_size", test_stack_push_pop_peek_size)))
    {
        ERROR_LOG("Failed to add test_stack_push_pop_peek_size to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_stack_clone", test_stack_clone)))
    {
        ERROR_LOG("Failed to add test_stack_clone to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_stack_null_inputs", test_stack_null_inputs)))
    {
        ERROR_LOG("Failed to add test_stack_null_inputs to suite\n");
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
test_stack_create_destroy (void)
{
    stack_t *p_stack = stack_create(5, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_stack);
    size_t size = 0U;
    CU_ASSERT_EQUAL(stack_size(p_stack, &size), STACK_SUCCESS);
    CU_ASSERT_EQUAL(size, 0);
    stack_destroy(p_stack);

    // Attempt creation with 0 capacity
    p_stack = stack_create(0, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NULL(p_stack);

    // Attempt creation with NULL funcs
    p_stack = stack_create(5, NULL, NULL, NULL, NULL);
    CU_ASSERT_PTR_NULL(p_stack);
}

static void
test_stack_push_pop_peek_size (void)
{
    stack_t *p_stack = NULL;
    size_t   size;
    int     *val = malloc(sizeof(int));
    *val         = 1;
    void *p_data = NULL;

    // Attempt push/pop to null stack
    CU_ASSERT_EQUAL(stack_push(p_stack, val), STACK_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(stack_pop(p_stack, &p_data), STACK_INVALID_ARGUMENT);
    

    // Create stack and push
    p_stack = stack_create(5, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_stack);
    CU_ASSERT_EQUAL(stack_push(p_stack, val), STACK_SUCCESS);

    // Peek at stack with one index
    CU_ASSERT_EQUAL(stack_peek(p_stack, &p_data), STACK_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)p_data, 1);

    // Size at 1
    CU_ASSERT_EQUAL(stack_size(p_stack, &size), STACK_SUCCESS);
    CU_ASSERT_EQUAL(size, 1);

    // Push past capacity (force resize)
    int **values = calloc(100, sizeof(int *));

    for (size_t idx = 0; idx < 100; idx++)
    {
        values[idx]  = calloc(1, sizeof(int));
        *values[idx] = idx * 5;
        CU_ASSERT_EQUAL(stack_push(p_stack, values[idx]), STACK_SUCCESS);
    }

    free(values);

    // Check new size
    CU_ASSERT_EQUAL(stack_size(p_stack, &size), STACK_SUCCESS);
    CU_ASSERT_EQUAL(size, 101);

    // Peek at large stack
    CU_ASSERT_EQUAL(stack_peek(p_stack, &p_data), STACK_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)p_data, 495);

    // Pop and few elements and check
    CU_ASSERT_EQUAL(stack_pop(p_stack, &p_data), STACK_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)p_data, 99*5);
    stack_del_ele(p_stack, p_data);

    CU_ASSERT_EQUAL(stack_pop(p_stack, &p_data), STACK_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)p_data, 98*5);
    stack_del_ele(p_stack, p_data);

    CU_ASSERT_EQUAL(stack_pop(p_stack, &p_data), STACK_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)p_data, 97*5);
    stack_del_ele(p_stack, p_data);

    CU_ASSERT_EQUAL(stack_pop(p_stack, &p_data), STACK_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)p_data, 96*5);
    stack_del_ele(p_stack, p_data);

    // Peek at stack again
    CU_ASSERT_EQUAL(stack_peek(p_stack, &p_data), STACK_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)p_data, 95*5);

    stack_destroy(p_stack);
}

static void
test_stack_clone (void)
{
    stack_t *p_ori = NULL;
    stack_t *p_new = NULL;    
    
    // Create a stack
    p_ori = stack_create(5, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_ori);
    CU_ASSERT_TRUE(stack_is_empty(p_ori));

    // Push some values to the stack
    int **values = calloc(5, sizeof(int *));

    for (size_t idx = 0; idx < 5; idx++)
    {
        values[idx]  = calloc(1, sizeof(int));
        *values[idx] = idx * 5;
        CU_ASSERT_EQUAL(stack_push(p_ori, values[idx]), STACK_SUCCESS);
    }

    free(values);

    // Check size and is_empty
    CU_ASSERT_FALSE(stack_is_empty(p_ori));

    // Clone stack
    p_new = stack_clone(p_ori);
    CU_ASSERT_PTR_NOT_NULL(p_new);
    CU_ASSERT_PTR_NOT_EQUAL(p_ori, p_new);

    // Sizes should match
    size_t size_ori = 0;
    size_t size_new = 0;
    CU_ASSERT_EQUAL(stack_size(p_ori, &size_ori), STACK_SUCCESS);
    CU_ASSERT_EQUAL(stack_size(p_new, &size_new), STACK_SUCCESS);
    CU_ASSERT_PTR_NOT_EQUAL(p_new, p_ori);

    // Check that all elements are equal in order (LIFO order)
    for (size_t idx = 0; idx < 5; idx++)
    {
        void *p_data_ori = NULL;
        void *p_data_new = NULL;
        CU_ASSERT_EQUAL(stack_pop(p_ori, &p_data_ori), STACK_SUCCESS);
        CU_ASSERT_EQUAL(stack_pop(p_new, &p_data_new), STACK_SUCCESS);
        CU_ASSERT_EQUAL(*(int *)p_data_ori, *(int *)p_data_new);
        stack_del_ele(p_ori, p_data_ori);
        stack_del_ele(p_new, p_data_new);
    }

    stack_destroy(p_ori);
    stack_destroy(p_new);
}

static void
test_stack_null_inputs (void)
{
    CU_ASSERT_PTR_NULL(stack_create(0, NULL, NULL, NULL, NULL));
    CU_ASSERT_EQUAL(stack_push(NULL, NULL), STACK_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(stack_pop(NULL, NULL), STACK_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(stack_peek(NULL, NULL), STACK_INVALID_ARGUMENT);
    CU_ASSERT_TRUE(stack_is_empty(NULL));
    CU_ASSERT_EQUAL(stack_size(NULL, NULL), STACK_INVALID_ARGUMENT);
    return;
}

/*** end of file ***/
