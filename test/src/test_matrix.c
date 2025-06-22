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
static void test_matrix_set_get_insert_remove(void);
static void test_matrix_find_copy(void);
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
        == (CU_add_test(suite,
                        "test_matrix_set_get_insert_remove",
                        test_matrix_set_get_insert_remove)))
    {
        ERROR_LOG("Failed to add test_matrix_set_get_insert_remove to suite\n");
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
    matrix_t *p_matrix
        = matrix_create(5, 6, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_matrix);

    size_t size = 0U;
    CU_ASSERT_EQUAL(matrix_row_size(p_matrix, &size), MATRIX_SUCCESS);
    CU_ASSERT(size == 5);
    CU_ASSERT_EQUAL(matrix_column_size(p_matrix, &size), MATRIX_SUCCESS);
    CU_ASSERT(size == 6);
    matrix_destroy(p_matrix);

    p_matrix
        = matrix_create(0, 5, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NULL(p_matrix);

    p_matrix
        = matrix_create(5, 0, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NULL(p_matrix);

    p_matrix = matrix_create(5, 5, NULL, NULL, NULL, NULL);
    CU_ASSERT_PTR_NULL(p_matrix);
    return;
}

static void
test_matrix_set_get_insert_remove (void)
{
    matrix_t *p_matrix
        = matrix_create(5, 5, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_matrix);

    // Fill in all 0s
    int *val0 = malloc(sizeof(int));
    *val0     = 0;
    CU_ASSERT_EQUAL(matrix_fill(p_matrix, val0), MATRIX_SUCCESS);
    free(val0);

    // Insert value at (1,1)
    int *val1 = malloc(sizeof(int));
    *val1     = 10;
    CU_ASSERT_EQUAL(matrix_insert(p_matrix, 1, 1, val1), MATRIX_SUCCESS);

    // Insert value at (4,4)
    int *val2 = malloc(sizeof(int));
    *val2     = 20;
    CU_ASSERT_EQUAL(matrix_insert(p_matrix, 4, 4, val2), MATRIX_SUCCESS);

    // Get value at (0,0)
    void *p_out = NULL;
    CU_ASSERT_EQUAL(matrix_get(p_matrix, 0, 0, &p_out), MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)p_out, 0);

    // Get new value at (1,1)
    CU_ASSERT_EQUAL(matrix_get(p_matrix, 1, 1, &p_out), MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)p_out, 10);

    // Get new value at (4,4)
    CU_ASSERT_EQUAL(matrix_get(p_matrix, 4, 4, &p_out), MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)p_out, 20);

    // Remove value at (1,1)
    CU_ASSERT_EQUAL(matrix_remove(p_matrix, 1, 1), MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(matrix_get(p_matrix, 1, 1, &p_out), MATRIX_SUCCESS);
    CU_ASSERT_PTR_NULL(*(int *)p_out);

    // Set new value at (4,4)
    int *val3 = malloc(sizeof(int));
    *val3     = 30;
    CU_ASSERT_EQUAL(matrix_set(p_matrix, 4, 4, val3), MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(matrix_get(p_matrix, 4, 4, &p_out), MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(*(int *)p_out, 30);

    // Overload matrix
    for (size_t idx = 0; idx < 10; ++idx)
    {
        for (size_t jdx = 0; jdx < 10; ++jdx)
        {
            int *val = malloc(sizeof(int));
            *val     = (int)(idx * 5 + jdx);
            if (jdx >= 5 || idx >= 5)
            {
                CU_ASSERT_EQUAL(matrix_insert(p_matrix, idx, jdx, val),
                                MATRIX_OUT_OF_BOUNDS);
                matrix_del_ele(p_matrix, val);
            }
            else
            {
                CU_ASSERT_EQUAL(matrix_insert(p_matrix, idx, jdx, val),
                                MATRIX_SUCCESS);
            }
        }
    }

    // Destroy matrix
    matrix_destroy(p_matrix);
    return;
}

static void
test_matrix_find_copy (void)
{
    matrix_t *p_matrix = NULL;
    matrix_t *p_copy   = NULL;

    p_matrix
        = matrix_create(3, 3, delete_int, compare_ints, print_int, copy_int);
    CU_ASSERT_PTR_NOT_NULL(p_matrix);

    // Fill in matrix with zeros
    int *val0 = malloc(sizeof(int));
    *val0     = 0;
    CU_ASSERT_EQUAL(matrix_fill(p_matrix, val0), MATRIX_SUCCESS);
    free(val0);

    // Insert value at (3, 2)
    int *val1 = malloc(sizeof(int));
    *val1     = 42;
    CU_ASSERT_EQUAL(matrix_insert(p_matrix, 2, 1, val1), MATRIX_SUCCESS);

    // Find value
    int    target = 42;
    size_t row, col;
    CU_ASSERT_EQUAL(matrix_find(p_matrix, &target, &row, &col), MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(row, 2);
    CU_ASSERT_EQUAL(col, 1);

    // Insert another value
    int *val2 = malloc(sizeof(int));
    *val2     = 99;
    CU_ASSERT_EQUAL(matrix_insert(p_matrix, 0, 1, val2), MATRIX_SUCCESS);

    // Find second value
    int target2 = 99;
    CU_ASSERT_EQUAL(matrix_find(p_matrix, &target2, &row, &col),
                    MATRIX_SUCCESS);
    CU_ASSERT_EQUAL(row, 0);
    CU_ASSERT_EQUAL(col, 1);

    // Try finding a non-existent value
    int target3 = 999;
    CU_ASSERT_EQUAL(matrix_find(p_matrix, &target3, &row, &col),
                    MATRIX_NOT_FOUND);

    // Copy matrix
    p_copy = matrix_clone(p_matrix);
    CU_ASSERT_PTR_NOT_NULL(p_copy);

    // Check equality
    CU_ASSERT_TRUE(matrix_is_equal(p_matrix, p_copy));

    // Check with a single NULL matrix
    CU_ASSERT_FALSE(matrix_is_equal(p_matrix, NULL));

    // Modify original matrix
    int *val3 = malloc(sizeof(int));
    *val3     = 100;
    CU_ASSERT_EQUAL(matrix_set(p_copy, 0, 0, val3), MATRIX_SUCCESS);

    // After modification, matrices should no longer be equal
    CU_ASSERT_FALSE(matrix_is_equal(p_matrix, p_copy));

    // Clean up
    matrix_destroy(p_matrix);
    matrix_destroy(p_copy);
}

/**
 * @brief Test behavior with NULL input and invalid conditions.
 */
static void
test_matrix_null_inputs (void)
{
    CU_ASSERT_PTR_NULL(matrix_create(0, 0, NULL, NULL, NULL, NULL));
    CU_ASSERT_EQUAL(matrix_row_size(NULL, NULL), MATRIX_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(matrix_column_size(NULL, NULL), MATRIX_INVALID_ARGUMENT);

    CU_ASSERT_EQUAL(matrix_insert(NULL, 0, 0, NULL), MATRIX_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(matrix_remove(NULL, 0, 0), MATRIX_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(matrix_get(NULL, 0, 0, NULL), MATRIX_INVALID_ARGUMENT);
    CU_ASSERT_EQUAL(matrix_set(NULL, 0, 0, NULL), MATRIX_INVALID_ARGUMENT);

    CU_ASSERT_EQUAL(matrix_find(NULL, NULL, NULL, NULL),
                    MATRIX_INVALID_ARGUMENT);
    CU_ASSERT_PTR_NULL(matrix_clone(NULL));
    CU_ASSERT_TRUE(matrix_is_equal(NULL, NULL));

    return;
}

/*** end of file ***/
