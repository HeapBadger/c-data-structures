/**
 * @file    test_matrix.c
 * @brief   Test suite for matrix.
 *
 * @author  heapbadger
 */

#include "test_matrix.h"
#include "matrix.h"
#include "test_auxiliary.h"
#include <CUnit/Basic.h>
#include <stdlib.h>

static void test_matrix_create_destroy(void);
static void test_matrix_set_get(void);
static void test_matrix_find_copy(void);
static void test_matrix_arithmetic(void);
static void test_matrix_null_inputs(void);

CU_pSuite
matrix_suite (void)
{
    CU_pSuite suite = NULL;
    suite           = CU_add_suite("matrix-suite", 0, 0);

    if (NULL == suite)
    {
        ERROR_LOG("Failed to add matrix-suite\n");
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_matrix_create_destroy", test_matrix_create_destroy)))
    {
        ERROR_LOG("Failed to add test_matrix_create_destroy to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(suite, "test_matrix_set_get", test_matrix_set_get)))
    {
        ERROR_LOG("Failed to add test_matrix_set_get to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(suite, "test_matrix_find_copy", test_matrix_find_copy)))
    {
        ERROR_LOG("Failed to add test_matrix_find_copy to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_matrix_arithmetic", test_matrix_arithmetic)))
    {
        ERROR_LOG("Failed to add test_matrix_arithmetic to suite\n");
        suite = NULL;
        goto CLEANUP;
    }

    if (NULL
        == (CU_add_test(
            suite, "test_matrix_null_inputs", test_matrix_null_inputs)))
    {
        ERROR_LOG("Failed to add test_matrix_null_inputs to suite\n");
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
 * @brief   Test creation and destruction of the matrix.
 */
static void
test_matrix_create_destroy (void)
{
    matrix_t *p_matrix = matrix_create(5, 6);
    CU_ASSERT_PTR_NOT_NULL(p_matrix);

    size_t size = 0U;
    CU_ASSERT_EQUAL(matrix_row_size(p_matrix, &size), MATRIX_SUCCESS);
    CU_ASSERT(size == 5);
    CU_ASSERT_EQUAL(matrix_column_size(p_matrix, &size), MATRIX_SUCCESS);
    CU_ASSERT(size == 6);
    matrix_destroy(p_matrix);

    p_matrix = matrix_create(0, 5);
    CU_ASSERT_PTR_NULL(p_matrix);

    p_matrix = matrix_create(5, 0);
    CU_ASSERT_PTR_NULL(p_matrix);
    return;
}

static void
test_matrix_set_get (void)
{
    matrix_t *p_matrix = matrix_create(5, 6);
    CU_ASSERT_PTR_NOT_NULL(p_matrix);

    // Set values
    for (size_t idx = 0; idx < 5; ++idx)
    {
        for (size_t jdx = 0; jdx < 6; ++jdx)
        {
            CU_ASSERT_EQUAL(matrix_set(p_matrix, idx, jdx, 0), MATRIX_SUCCESS);
        }
    }

    // Attempt to set past the matrix bounds
    CU_ASSERT_EQUAL(matrix_set(p_matrix, 5, 0, 0), MATRIX_OUT_OF_BOUNDS);
    CU_ASSERT_EQUAL(matrix_set(p_matrix, 0, 6, 0), MATRIX_OUT_OF_BOUNDS);
    matrix_destroy(p_matrix);

    // Fill matrix and check values with get
    p_matrix = matrix_create(10, 10);
    CU_ASSERT_EQUAL(matrix_fill(p_matrix, 1), MATRIX_SUCCESS);
    double out = 0.0;
    CU_ASSERT_EQUAL(matrix_get(p_matrix, 1, 1, &out), MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(out, 1.0);
    matrix_destroy(p_matrix);

    // Fill a partially filled matrix (should overwrite values)
    p_matrix = matrix_create(10, 10);
    CU_ASSERT_EQUAL(matrix_set(p_matrix, 0, 0, 0), MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(matrix_get(p_matrix, 0, 0, &out), MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(out, 0);
    CU_ASSERT_EQUAL(matrix_fill(p_matrix, 1), MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(matrix_get(p_matrix, 0, 0, &out), MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(out, 1.0);
    matrix_destroy(p_matrix);
}

static void
test_matrix_find_copy (void)
{
    matrix_t *p_ori = NULL;
    matrix_t *p_new = NULL;

    // Create matrix with vlaues
    p_ori = matrix_create(2, 1);
    CU_ASSERT_PTR_NOT_NULL(p_ori);
    CU_ASSERT_EQUAL(matrix_set(p_ori, 0, 0, 0), MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(matrix_set(p_ori, 1, 0, 10), MATRIX_SUCCESS);

    // Clone matrix
    p_new = matrix_clone(p_ori);
    size_t size;
    CU_ASSERT_EQUAL(matrix_row_size(p_new, &size), MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(size, 2);
    CU_ASSERT_EQUAL(matrix_column_size(p_new, &size), MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(size, 1);

    // Check if values are equal but pointers are different
    CU_ASSERT_PTR_NOT_EQUAL(p_new, p_ori);
    CU_ASSERT_TRUE(matrix_is_equal(p_new, p_ori));

    // Set one value different in the clone
    CU_ASSERT_EQUAL(matrix_set(p_new, 0, 0, 4), MATRIX_SUCCESS);
    CU_ASSERT_FALSE(matrix_is_equal(p_new, p_ori));

    // Find a value that exists and doesnt exist
    size_t row = 0;
    size_t col = 0;
    CU_ASSERT_EQUAL(matrix_find(p_ori, 4, &row, &col), MATRIX_NOT_FOUND);
    CU_ASSERT_EQUAL(matrix_find(p_new, 4, &row, &col), MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(row, 0);
    CU_ASSERT_EQUAL(col, 0);

    matrix_destroy(p_ori);
    matrix_destroy(p_new);
    return;
}

static void
test_matrix_arithmetic (void)
{
    return;
}

static void
test_matrix_null_inputs (void)
{
    //     CU_ASSERT_PTR_NULL(matrix_create(0, 0, NULL, NULL, NULL, NULL));
    //     CU_ASSERT_EQUAL(matrix_row_size(NULL, NULL),
    //     MATRIX_INVALID_ARGUMENT); CU_ASSERT_EQUAL(matrix_column_size(NULL,
    //     NULL), MATRIX_INVALID_ARGUMENT);

    //     CU_ASSERT_EQUAL(matrix_set(NULL, 0, 0, NULL),
    //     MATRIX_INVALID_ARGUMENT); CU_ASSERT_EQUAL(matrix_remove(NULL, 0, 0),
    //     MATRIX_INVALID_ARGUMENT); CU_ASSERT_EQUAL(matrix_get(NULL, 0, 0,
    //     NULL), MATRIX_INVALID_ARGUMENT); CU_ASSERT_EQUAL(matrix_set(NULL, 0,
    //     0, NULL), MATRIX_INVALID_ARGUMENT);

    //     CU_ASSERT_EQUAL(matrix_find(NULL, NULL, NULL, NULL),
    //                     MATRIX_INVALID_ARGUMENT);
    //     CU_ASSERT_PTR_NULL(matrix_clone(NULL));
    //     CU_ASSERT_TRUE(matrix_is_equal(NULL, NULL));

    return;
}

/*** end of file ***/
