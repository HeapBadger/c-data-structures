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
test_queue_null_inputs (void)
{
    // CU_ASSERT_EQUAL(ll_size(NULL, NULL), LL_INVALID_ARGUMENT);
    return;
}

/*** end of file ***/
