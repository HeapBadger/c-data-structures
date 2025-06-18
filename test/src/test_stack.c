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

static void test_stack_create_destroy_clear(void);
static void test_stack_push_pop_size(void);
static void test_stack_peak_isempty(void);
static void test_stack_clone_and_resize(void);
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
        == (CU_add_test(suite,
                        "test_stack_create_destroy_clear",
                        test_stack_create_destroy_clear)))
    {
        ERROR_LOG("Failed to add test_stack_create_destroy_clear to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_stack_push_pop_size", test_stack_push_pop_size)))
    {
        ERROR_LOG("Failed to add test_stack_push_pop_size to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_stack_peak_isempty", test_stack_peak_isempty)))
    {
        ERROR_LOG("Failed to add test_stack_peak_isempty to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_stack_clone_and_resize", test_stack_clone_and_resize)))
    {
        ERROR_LOG("Failed to add test_stack_clone_and_resize to suite\n");
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
test_stack_create_destroy_clear (void)
{
    stack_t *p_stack
        = stack_create(5, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_stack);

    size_t size = 0U;
    CU_ASSERT_EQUAL(stack_size(p_stack, &size), STACK_SUCCESS);
    CU_ASSERT_EQUAL(size, 0);

    // Fill the stack with value '0' (as int)
    int val = 0;
    CU_ASSERT_EQUAL(stack_fill(p_stack, &val), STACK_SUCCESS);

    CU_ASSERT_EQUAL(stack_size(p_stack, &size), STACK_SUCCESS);
    CU_ASSERT_EQUAL(size, 5);

    // Clear the stack
    CU_ASSERT_EQUAL(stack_clear(p_stack), STACK_SUCCESS);
    CU_ASSERT_EQUAL(stack_size(p_stack, &size), STACK_SUCCESS);
    CU_ASSERT_EQUAL(size, 0);

    stack_destroy(p_stack);

    // Creating with 0 capacity should return NULL
    p_stack = stack_create(0, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NULL(p_stack);

    // Creating with NULL function pointers should fail (assuming your create
    // checks that)
    p_stack = stack_create(5, NULL, NULL, NULL, NULL);
    CU_ASSERT_PTR_NULL(p_stack);
}

static void
test_stack_push_pop_size (void)
{
    stack_t *p_stack
        = stack_create(5, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_stack);

    // Push three values
    int vals[] = { 10, 20, 30 };

    for (size_t i = 0; i < 3; ++i)
    {
        CU_ASSERT_EQUAL(stack_push(p_stack, &vals[i]), STACK_SUCCESS);
    }

    size_t size = 0;
    CU_ASSERT_EQUAL(stack_size(p_stack, &size), STACK_SUCCESS);
    CU_ASSERT_EQUAL(size, 3);

    // Pop and check LIFO order
    void *p_out = NULL;

    CU_ASSERT_EQUAL(stack_pop(p_stack, &p_out), STACK_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(p_out);
    CU_ASSERT_EQUAL(*(int *)p_out, 30);

    CU_ASSERT_EQUAL(stack_pop(p_stack, &p_out), STACK_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(p_out);
    CU_ASSERT_EQUAL(*(int *)p_out, 20);

    CU_ASSERT_EQUAL(stack_pop(p_stack, &p_out), STACK_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(p_out);
    CU_ASSERT_EQUAL(*(int *)p_out, 10);

    // Now the stack should be empty
    CU_ASSERT_TRUE(stack_is_empty(p_stack));
    CU_ASSERT_EQUAL(stack_pop(p_stack, &p_out), STACK_OUT_OF_BOUNDS);

    stack_destroy(p_stack);
}

static void
test_stack_peak_isempty (void)
{
    stack_t *p_stack
        = stack_create(5, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_stack);

    // Initially empty
    CU_ASSERT_TRUE(stack_is_empty(p_stack));

    // Peek on empty stack should return STACK_EMPTY
    void *p_top = NULL;
    CU_ASSERT_EQUAL(stack_peek(p_stack, &p_top), STACK_EMPTY);

    int *val1 = calloc(1, sizeof(int));
    *val1     = 42;
    CU_ASSERT_EQUAL(stack_push(p_stack, val1), STACK_SUCCESS);

    int *val2 = calloc(1, sizeof(int));
    *val2     = 100;
    CU_ASSERT_EQUAL(stack_push(p_stack, val2), STACK_SUCCESS);

    // Now peek should succeed and give val
    CU_ASSERT_EQUAL(stack_peek(p_stack, &p_top), STACK_SUCCESS);
    CU_ASSERT_PTR_NOT_NULL(p_top);
    CU_ASSERT_EQUAL(*(int *)p_top, 100);

    // Stack is not empty
    CU_ASSERT_FALSE(stack_is_empty(p_stack));

    stack_destroy(p_stack);
}

static void
test_stack_clone_and_resize (void)
{
    const size_t initial_capacity = 5;
    stack_t     *p_stack          = stack_create(
        initial_capacity, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_stack);

    // Push initial_capacity elements
    for (int i = 0; i < (int)initial_capacity; i++)
    {
        int *val = calloc(1, sizeof(int));
        CU_ASSERT_PTR_NOT_NULL(val);
        *val = i * 10;
        CU_ASSERT_EQUAL(stack_push(p_stack, val), STACK_SUCCESS);
    }

    // Stack should be full now
    size_t size = 0;
    CU_ASSERT_EQUAL(stack_size(p_stack, &size), STACK_SUCCESS);
    CU_ASSERT_EQUAL(size, initial_capacity);

    // Push more elements to trigger resize
    for (int i = (int)initial_capacity; i < (int)(initial_capacity + 3); i++)
    {
        int *val = calloc(1, sizeof(int));
        CU_ASSERT_PTR_NOT_NULL(val);
        *val = i * 10;
        CU_ASSERT_EQUAL(stack_push(p_stack, val), STACK_SUCCESS);
    }

    // Check size is initial_capacity + 3
    CU_ASSERT_EQUAL(stack_size(p_stack, &size), STACK_SUCCESS);
    CU_ASSERT_EQUAL(size, initial_capacity + 3);

    // Clone the stack
    stack_t *p_clone = stack_clone(p_stack);
    CU_ASSERT_PTR_NOT_NULL(p_clone);

    // Sizes should match
    size_t clone_size = 0;
    CU_ASSERT_EQUAL(stack_size(p_clone, &clone_size), STACK_SUCCESS);
    CU_ASSERT_EQUAL(clone_size, size);

    // Check that all elements are equal in order (LIFO order)
    for (size_t i = 0; i < size; i++)
    {
        void *orig_val  = NULL;
        void *clone_val = NULL;

        CU_ASSERT_EQUAL(array_get(p_stack->p_array, i, &orig_val),
                        ARRAY_SUCCESS);
        CU_ASSERT_EQUAL(array_get(p_clone->p_array, i, &clone_val),
                        ARRAY_SUCCESS);
        CU_ASSERT_PTR_NOT_NULL(orig_val);
        CU_ASSERT_PTR_NOT_NULL(clone_val);

        CU_ASSERT_EQUAL(compare_ints(orig_val, clone_val), 0);
    }

    stack_destroy(p_stack);
    stack_destroy(p_clone);
}

static void
test_stack_null_inputs (void)
{
    CU_ASSERT_PTR_NULL(stack_create(0, NULL, NULL, NULL, NULL));
    CU_ASSERT_EQUAL(stack_clear(NULL), STACK_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(stack_push(NULL, NULL), STACK_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(stack_pop(NULL, NULL), STACK_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(stack_peek(NULL, NULL), STACK_INVALID_ARGUMENT);
    CU_ASSERT_TRUE(stack_is_empty(NULL));
    CU_ASSERT_EQUAL(stack_size(NULL, NULL), STACK_INVALID_ARGUMENT);
    return;
}

/*** end of file ***/
