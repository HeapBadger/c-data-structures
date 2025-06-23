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
    // // Dimensions for test matrices
    // size_t rows = 3, cols = 3;

    // // Create matrices for testing
    // matrix_t *p_a      = matrix_create(rows, cols);
    // matrix_t *p_b      = matrix_create(rows, cols);
    // matrix_t *p_result = matrix_create(rows, cols);
    // CU_ASSERT_PTR_NOT_NULL(p_a);
    // CU_ASSERT_PTR_NOT_NULL(p_b);
    // CU_ASSERT_PTR_NOT_NULL(p_result);

    // // Initialize matrices p_a and p_b with some values
    // for (size_t row = 0; row < rows; ++row)
    // {
    //     for (size_t col = 0; col < cols; ++col)
    //     {
    //         CU_ASSERT_EQUAL(matrix_set(p_a, row, col, (double)(row + col)),
    //                         MATRIX_SUCCESS);
    //         CU_ASSERT_EQUAL(matrix_set(p_b, row, col, (double)(row * col)),
    //                         MATRIX_SUCCESS);
    //     }
    // }

    // // Test Addition
    // CU_ASSERT_EQUAL(matrix_add(p_a, p_b, p_result), MATRIX_SUCCESS);
    // double val_a, val_b, val_r;

    // for (size_t row = 0; row < rows; ++row)
    // {
    //     for (size_t col = 0; col < cols; ++col)
    //     {
    //         CU_ASSERT_EQUAL(matrix_get(p_a, row, col, &val_a), MATRIX_SUCCESS);
    //         CU_ASSERT_EQUAL(matrix_get(p_b, row, col, &val_b), MATRIX_SUCCESS);
    //         CU_ASSERT_EQUAL(matrix_get(p_result, row, col, &val_r),
    //                         MATRIX_SUCCESS);
    //         CU_ASSERT_DOUBLE_EQUAL(val_r, val_a + val_b, 1e-9);
    //     }
    // }

    // // Test Subtraction
    // CU_ASSERT_EQUAL(matrix_subtract(p_a, p_b, p_result), MATRIX_SUCCESS);

    // for (size_t row = 0; row < rows; ++row)
    // {
    //     for (size_t col = 0; col < cols; ++col)
    //     {
    //         CU_ASSERT_EQUAL(matrix_get(p_a, row, col, &val_a), MATRIX_SUCCESS);
    //         CU_ASSERT_EQUAL(matrix_get(p_b, row, col, &val_b), MATRIX_SUCCESS);
    //         CU_ASSERT_EQUAL(matrix_get(p_result, row, col, &val_r),
    //                         MATRIX_SUCCESS);
    //         CU_ASSERT_DOUBLE_EQUAL(val_r, val_a - val_b, 1e-9);
    //     }
    // }

    // // Test Scalar Multiplication
    // double scalar = 2.5;
    // CU_ASSERT_EQUAL(matrix_scalar_multiply(p_a, scalar, p_result),
    //                 MATRIX_SUCCESS);

    // for (size_t row = 0; row < rows; ++row)
    // {
    //     for (size_t col = 0; col < cols; ++col)
    //     {
    //         CU_ASSERT_EQUAL(matrix_get(p_a, row, col, &val_a), MATRIX_SUCCESS);
    //         CU_ASSERT_EQUAL(matrix_get(p_result, row, col, &val_r),
    //                         MATRIX_SUCCESS);
    //         CU_ASSERT_DOUBLE_EQUAL(val_r, val_a * scalar, 1e-9);
    //     }
    // }

    // // Test Transpose
    // matrix_t *p_transpose = matrix_create(cols, rows);
    // CU_ASSERT_PTR_NOT_NULL(p_transpose);
    // CU_ASSERT_EQUAL(matrix_transpose(p_a, p_transpose), MATRIX_SUCCESS);

    // for (size_t row = 0; row < rows; ++row)
    // {
    //     for (size_t col = 0; col < cols; ++col)
    //     {
    //         CU_ASSERT_EQUAL(matrix_get(p_a, row, col, &val_a), MATRIX_SUCCESS);
    //         CU_ASSERT_EQUAL(matrix_get(p_transpose, col, row, &val_r),
    //                         MATRIX_SUCCESS);
    //         CU_ASSERT_DOUBLE_EQUAL(val_r, val_a, 1e-9);
    //     }
    // }
    // matrix_destroy(p_transpose);

    // // Test Multiplication
    // // For multiplication, use 3x2 and 2x3 matrices
    // matrix_t *p_mul_a      = matrix_create(3, 2);
    // matrix_t *p_mul_b      = matrix_create(2, 3);
    // matrix_t *p_mul_result = matrix_create(3, 3);
    // CU_ASSERT_PTR_NOT_NULL(p_mul_a);
    // CU_ASSERT_PTR_NOT_NULL(p_mul_b);
    // CU_ASSERT_PTR_NOT_NULL(p_mul_result);

    // // Initialize p_mul_a
    // double mul_a_vals[3][2] = { { 1, 2 }, { 3, 4 }, { 5, 6 } };

    // for (size_t row = 0; row < 3; ++row)
    // {
    //     for (size_t col = 0; col < 2; ++col)
    //     {
    //         CU_ASSERT_EQUAL(matrix_set(p_mul_a, row, col, mul_a_vals[row][col]),
    //                         MATRIX_SUCCESS);
    //     }
    // }

    // // Initialize p_mul_b
    // double mul_b_vals[2][3] = { { 7, 8, 9 }, { 10, 11, 12 } };

    // for (size_t row = 0; row < 2; ++row)
    // {
    //     for (size_t col = 0; col < 3; ++col)
    //     {
    //         CU_ASSERT_EQUAL(matrix_set(p_mul_b, row, col, mul_b_vals[row][col]),
    //                         MATRIX_SUCCESS);
    //     }
    // }

    // // Multiply p_mul_a * p_mul_b -> p_mul_result
    // CU_ASSERT_EQUAL(matrix_multiply(p_mul_a, p_mul_b, p_mul_result),
    //                 MATRIX_SUCCESS);

    // // Expected multiplication result
    // double expected_mul[3][3]
    //     = { { 27, 30, 33 }, { 61, 68, 75 }, { 95, 106, 117 } };

    // for (size_t row = 0; row < 3; ++row)
    // {
    //     for (size_t col = 0; col < 3; ++col)
    //     {
    //         CU_ASSERT_EQUAL(matrix_get(p_mul_result, row, col, &val_r),
    //                         MATRIX_SUCCESS);
    //         CU_ASSERT_DOUBLE_EQUAL(val_r, expected_mul[row][col], 1e-9);
    //     }
    // }

    // matrix_destroy(p_mul_a);
    // matrix_destroy(p_mul_b);
    // matrix_destroy(p_mul_result);

    // // Test Determinant (3x3)
    // double det = 0.0;
    // CU_ASSERT_EQUAL(matrix_determinant(p_a, &det), MATRIX_SUCCESS);

    // // Manually compute determinant of p_a (which is filled with r+c)
    // // For matrix:
    // // 0 1 2
    // // 1 2 3
    // // 2 3 4
    // // det = 0*(2*4 - 3*3) - 1*(1*4 - 3*2) + 2*(1*3 - 2*2)
    // // det = 0 - 1*(4 - 6) + 2*(3 - 4)
    // // det = 0 - 1*(-2) + 2*(-1) = 2 - 2 = 0
    // CU_ASSERT_DOUBLE_EQUAL(det, 0.0, 1e-9);

    // // Test Inverse (should fail since determinant is 0)
    // matrix_t *p_inv = matrix_create(rows, cols);
    // CU_ASSERT_PTR_NOT_NULL(p_inv);
    // CU_ASSERT_NOT_EQUAL(matrix_inverse(p_a, p_inv), MATRIX_SUCCESS);

    // // Modify p_a to identity matrix for invertible test
    // matrix_fill(p_a, 0.0);

    // for (size_t idx = 0; idx < rows; ++idx)
    // {
    //     CU_ASSERT_EQUAL(matrix_set(p_a, idx, idx, 1.0), MATRIX_SUCCESS);
    // }

    // CU_ASSERT_EQUAL(matrix_inverse(p_a, p_inv), MATRIX_SUCCESS);
    // // Inverse of identity is identity, verify
    // for (size_t row = 0; row < rows; ++row)
    // {
    //     for (size_t col = 0; col < cols; ++col)
    //     {
    //         CU_ASSERT_EQUAL(matrix_get(p_inv, row, col, &val_r),
    //                         MATRIX_SUCCESS);
    //         if (row == col)
    //         {
    //             CU_ASSERT_DOUBLE_EQUAL(val_r, 1.0, 1e-9);
    //         }
    //         else
    //         {
    //             CU_ASSERT_DOUBLE_EQUAL(val_r, 0.0, 1e-9);
    //         }
    //     }
    // }

    // matrix_destroy(p_inv);
    // matrix_destroy(p_a);
    // matrix_destroy(p_b);
    // matrix_destroy(p_result);
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
