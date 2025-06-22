/**
 * @file    test_queue.c
 * @brief   Test suite for teh queue implementation.
 *
 * @author  heapbadger
 */

#include "test_queue.h"
#include "queue.h"
#include "test_auxiliary.h"
#include <CUnit/Basic.h>
#include <stdlib.h>

static void test_queue_create_destroy(void);
static void test_queue_enqueue_dequeue_peek_size(void);
static void test_queue_clone(void);
static void test_queue_null_inputs(void);

CU_pSuite
queue_suite (void)
{
    CU_pSuite suite = NULL;
    suite           = CU_add_suite("queue-suite", 0, 0);

    if (NULL == suite)
    {
        ERROR_LOG("Failed to add queue-suite\n");
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_queue_create_destroy", test_queue_create_destroy)))
    {
        ERROR_LOG("Failed to add test_queue_create_destroy to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(suite,
                        "test_queue_enqueue_dequeue_peek_size",
                        test_queue_enqueue_dequeue_peek_size)))
    {
        ERROR_LOG(
            "Failed to add test_queue_enqueue_dequeue_peek_size to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL == (CU_add_test(suite, "test_queue_clone", test_queue_clone)))
    {
        ERROR_LOG("Failed to add test_queue_clone to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_queue_null_inputs", test_queue_null_inputs)))
    {
        ERROR_LOG("Failed to add test_queue_null_inputs to suite\n");
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
test_queue_create_destroy (void)
{
    queue_t *p_queue
        = queue_create(delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_queue);
    size_t size;
    CU_ASSERT_EQUAL(queue_size(p_queue, &size), 0);
    CU_ASSERT_EQUAL(size, 0);
    queue_destroy(p_queue);
}

static void
test_queue_enqueue_dequeue_peek_size (void)
{
    queue_t *p_queue = NULL;
    size_t   size;
    int     *val = malloc(sizeof(int));
    *val         = 1;

    // Attempt append to null queue
    CU_ASSERT_EQUAL(queue_enqueue(p_queue, val), QUEUE_INVALID_ARGUMENT);

    // Attempt peek to null queue
    int   peak   = 0;
    void *p_data = &peak;
    CU_ASSERT_EQUAL(queue_peek(p_queue, &p_data), QUEUE_INVALID_ARGUMENT);

    // Append to valid queue
    p_queue = queue_create(delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_queue);
    CU_ASSERT_EQUAL(queue_enqueue(p_queue, val), QUEUE_SUCCESS);

    // Peek at queue with one index
    CU_ASSERT_EQUAL(queue_peek(p_queue, &p_data), QUEUE_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)p_data, 1);

    // Size at 1
    CU_ASSERT_EQUAL(queue_size(p_queue, &size), QUEUE_SUCCESS);
    CU_ASSERT_EQUAL(size, 1);

    // Append with many values
    int **values = calloc(500, sizeof(int *));

    for (size_t idx = 0; idx < 500; idx++)
    {
        values[idx]  = calloc(1, sizeof(int));
        *values[idx] = idx * 5;
        CU_ASSERT_EQUAL(queue_enqueue(p_queue, values[idx]), QUEUE_SUCCESS);
    }

    // Check new size
    CU_ASSERT_EQUAL(queue_size(p_queue, &size), QUEUE_SUCCESS);
    CU_ASSERT_EQUAL(size, 501);

    // Peek at queue with large queue
    CU_ASSERT_EQUAL(queue_peek(p_queue, &p_data), QUEUE_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)p_data, 1);

    // Dequeue and check size/peek
    void *p_dequeue_data = NULL;
    CU_ASSERT_EQUAL(queue_dequeue(p_queue, &p_dequeue_data), QUEUE_SUCCESS);
    CU_ASSERT_EQUAL(1, *(int *)p_dequeue_data);
    queue_del_ele(p_queue, p_dequeue_data);
    CU_ASSERT_EQUAL(queue_peek(p_queue, &p_data), QUEUE_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)p_data, 0);
    CU_ASSERT_EQUAL(queue_size(p_queue, &size), QUEUE_SUCCESS);
    CU_ASSERT_EQUAL(size, 500);

    // Dequeue with many values
    for (size_t idx = 0; idx < 500; idx++)
    {
        CU_ASSERT_EQUAL(queue_dequeue(p_queue, &p_dequeue_data), QUEUE_SUCCESS);
        CU_ASSERT_EQUAL(idx * 5, *(int *)p_dequeue_data);
        queue_del_ele(p_queue, p_dequeue_data);
    }

    // Dequeue one more time (queue should be empty)
    CU_ASSERT_EQUAL(queue_dequeue(p_queue, &p_dequeue_data), QUEUE_EMPTY);

    queue_destroy(p_queue);
    free(values);
}

static void
test_queue_clone (void)
{
    queue_t *p_ori = NULL;
    queue_t *p_new = NULL;
    size_t   size_ori;
    size_t   size_new;

    // Create queue with many values
    p_ori = queue_create(delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_ori);
    CU_ASSERT_TRUE(queue_is_empty(p_ori));

    int **values = calloc(500, sizeof(int *));

    for (size_t idx = 0; idx < 15; idx++)
    {
        values[idx]  = calloc(1, sizeof(int));
        *values[idx] = idx * 5;
        CU_ASSERT_EQUAL(queue_enqueue(p_ori, values[idx]), QUEUE_SUCCESS);
    }

    CU_ASSERT_FALSE(queue_is_empty(p_ori));

    // Clone queue
    p_new = queue_clone(p_ori);
    CU_ASSERT_PTR_NOT_NULL(p_new);
    CU_ASSERT_PTR_NOT_EQUAL(p_new, p_ori);
    CU_ASSERT_EQUAL(queue_size(p_ori, &size_ori), QUEUE_SUCCESS);
    CU_ASSERT_EQUAL(queue_size(p_new, &size_new), QUEUE_SUCCESS);
    CU_ASSERT_EQUAL(size_new, size_ori);
    void *p_data_ori = NULL;
    void *p_data_new = NULL;
    CU_ASSERT_EQUAL(queue_peek(p_ori, &p_data_ori), QUEUE_SUCCESS);
    CU_ASSERT_EQUAL(queue_peek(p_new, &p_data_new), QUEUE_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)p_data_ori, *(int *)p_data_new);

    // Free queues
    queue_destroy(p_ori);
    queue_destroy(p_new);
    free(values);
}

static void
test_queue_null_inputs (void)
{
    CU_ASSERT_PTR_NULL(queue_create(NULL, NULL, NULL, NULL));
    CU_ASSERT_EQUAL(queue_enqueue(NULL, NULL), QUEUE_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(queue_dequeue(NULL, NULL), QUEUE_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(queue_peek(NULL, NULL), QUEUE_INVALID_ARGUMENT);
    CU_ASSERT_FALSE(queue_is_empty(NULL));
    CU_ASSERT_EQUAL(queue_size(NULL, NULL), QUEUE_INVALID_ARGUMENT);
    CU_ASSERT_PTR_NULL(queue_clone(NULL));
}

/*** end of file ***/
