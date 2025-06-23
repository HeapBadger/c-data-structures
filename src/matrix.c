/**
 * @file matrix.c
 * @brief Implementation of a matrix data structure using a dynamic array.
 *
 * This matrix is represented as a contiguous 1D dynamic array internally,
 * enabling efficient memory access and compact storage. Elements are laid out
 * in row-major order, which improves performance due to better cache locality
 * compared to pointer-based 2D grids. Matrix dimensions are flexible and
 * support resizing as needed.
 *
 * This matrix implementation stores elements as `double`, supporting
 * basic matrix operations such as create, destroy, set, get, find, and clone.
 *
 * Using `double` ensures safe handling of both `int` and `float` values
 * via implicit conversion, offering higher precision and a traditional
 * numerical matrix structure.
 *
 * @author heapbadger
 */

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

#define ROW_MAJOR_IDX(row, col, num_cols) ((row) * (num_cols) + (col))

/**
 * @brief Convert an array module return code to a matrix module error code.
 *
 * @param ret Return code from array function.
 *
 * @return Corresponding matrix_error_code_t.
 */
static matrix_error_code_t matrix_error_from_array(array_error_code_t ret);

/**
 * @brief Custom delete function for handling double data types.
 *
 * @param p_data Pointer to the double type data.
 */
static void double_del(void *p_data);

/**
 * @brief Custom compare function for handling double data types.
 *
 * @param p_double_a Pointer to first double type value.
 * @param p_double_b Pointer to second double type value.
 */
static int double_cmp(const void *p_double_a, const void *p_double_b);

/**
 * @brief Custom print function for handling double data types.
 *
 * @param p_data Pointer to the double type data.
 * @param index Data index.
 */
static void double_print(void *p_data, size_t index);

/**
 * @brief Custom copy function for handling double data types.
 *
 * @param p_data Pointer to the double type data.
 */
static void *double_cpy(const void *p_src);

static bool matrix_is_same_row_size (const matrix_t *p_matrix_a, const matrix_t *p_matrix_b);
static bool matrix_is_same_col_size (const matrix_t *p_matrix_a, const matrix_t *p_matrix_b);

matrix_t *
matrix_create (size_t rows, size_t cols)
{
    matrix_t *p_matrix = NULL;

    if ((0 == rows) || (0 == cols))
    {
        return NULL;
    }

    p_matrix = (matrix_t *)calloc(1U, sizeof(matrix_t));

    if (NULL == p_matrix)
    {
        return NULL;
    }

    size_t cap       = rows * cols;
    p_matrix->p_flat = array_create(cap,
                                    (del_func)double_del,
                                    (cmp_func)double_cmp,
                                    (print_func)double_print,
                                    (copy_func)double_cpy);
    p_matrix->cols   = cols;
    p_matrix->rows   = rows;
    matrix_fill(p_matrix, 0.0);
    return p_matrix;
}

void
matrix_destroy (matrix_t *p_matrix)
{
    if ((NULL != p_matrix) && (NULL != p_matrix->p_flat))
    {
        array_destroy(p_matrix->p_flat);
        p_matrix->p_flat = NULL;
        p_matrix->cols   = 0U;
        p_matrix->rows   = 0U;
        free(p_matrix);
        p_matrix = NULL;
    }
}

void
matrix_del_ele (matrix_t *p_matrix, void *p_data)
{
    if ((NULL != p_matrix) && (NULL != p_data))
    {
        array_del_ele(p_matrix->p_flat, p_data);
    }
}

matrix_error_code_t
matrix_fill (matrix_t *p_matrix, double value)
{
    if (NULL == p_matrix)
    {
        return MATRIX_INVALID_ARGUMENT;
    }

    return matrix_error_from_array(
        array_fill(p_matrix->p_flat, (void *)&value));
}

matrix_error_code_t
matrix_get (const matrix_t *p_matrix, size_t row, size_t col, double *p_out)
{
    if ((NULL == p_matrix) || (NULL == p_out))
    {
        return MATRIX_INVALID_ARGUMENT;
    }

    if ((row >= p_matrix->rows) || (col >= p_matrix->cols))
    {
        return MATRIX_OUT_OF_BOUNDS;
    }

    matrix_error_code_t ret;
    void               *p_val = NULL;
    size_t              idx;

    idx = ROW_MAJOR_IDX(row, col, p_matrix->cols);
    ret = matrix_error_from_array(array_get(p_matrix->p_flat, idx, &p_val));

    if ((MATRIX_SUCCESS == ret) && (NULL != p_val))
    {
        *p_out = *(double *)p_val;
    }

    return ret;
}

matrix_error_code_t
matrix_set (matrix_t *p_matrix, size_t row, size_t col, double value)
{
    if (NULL == p_matrix)
    {
        return MATRIX_ALLOCATION_FAILURE;
    }

    if ((row >= p_matrix->rows) || (col >= p_matrix->cols))
    {
        return MATRIX_OUT_OF_BOUNDS;
    }

    double *p_val = calloc(1, sizeof(double));

    if (NULL == p_val)
    {
        return MATRIX_ALLOCATION_FAILURE;
    }

    matrix_error_code_t ret;
    size_t              idx = ROW_MAJOR_IDX(row, col, p_matrix->cols);
    *p_val                  = value;
    ret = matrix_error_from_array(array_set(p_matrix->p_flat, idx, p_val));

    if (MATRIX_SUCCESS != ret)
    {
        free(p_val);
    }

    return ret;
}

bool
matrix_is_equal (const matrix_t *p_matrix_a, const matrix_t *p_matrix_b)
{
    if ((NULL == p_matrix_a) && (NULL == p_matrix_b))
    {
        return true;
    }

    if ((NULL == p_matrix_a) || (NULL == p_matrix_b))
    {
        return false;
    }

    if ((p_matrix_a->rows != p_matrix_b->rows)
        || (p_matrix_a->cols != p_matrix_b->cols))
    {
        return false;
    }

    return array_is_equal(p_matrix_a->p_flat, p_matrix_b->p_flat);
}

void
matrix_print (const matrix_t *p_matrix)
{
    if (NULL != p_matrix)
    {
        array_print(p_matrix->p_flat);
    }
}

matrix_error_code_t
matrix_row_size (const matrix_t *p_matrix, size_t *p_size)
{
    if ((NULL != p_matrix) && (NULL != p_size))
    {
        *p_size = p_matrix->rows;
        return MATRIX_SUCCESS;
    }

    return MATRIX_INVALID_ARGUMENT;
}

matrix_error_code_t
matrix_column_size (const matrix_t *p_matrix, size_t *p_size)
{
    if ((NULL != p_matrix) && (NULL != p_size))
    {
        *p_size = p_matrix->cols;
        return MATRIX_SUCCESS;
    }

    return MATRIX_INVALID_ARGUMENT;
}

matrix_error_code_t
matrix_find (const matrix_t *p_matrix, double key, size_t *p_row, size_t *p_col)
{
    if ((NULL == p_matrix) || (NULL == p_row) || (NULL == p_col))
    {
        return MATRIX_INVALID_ARGUMENT;
    }

    size_t flat_idx = 0;

    if (ARRAY_SUCCESS == array_find(p_matrix->p_flat, (void **)&key, &flat_idx))
    {
        *p_row = flat_idx / p_matrix->cols;
        *p_col = flat_idx % p_matrix->cols;
        return MATRIX_SUCCESS;
    }

    return MATRIX_NOT_FOUND;
}

matrix_t *
matrix_clone (const matrix_t *p_ori)
{
    matrix_t *p_new = NULL;

    if (NULL != p_ori)
    {
        p_new = (matrix_t *)calloc(1U, sizeof(matrix_t));

        if (NULL != p_new)
        {
            p_new->cols   = p_ori->cols;
            p_new->rows   = p_ori->rows;
            p_new->p_flat = array_clone(p_ori->p_flat);
        }
    }

    return p_new;
}

matrix_error_code_t
matrix_add (const matrix_t *p_matrix_a,
            const matrix_t *p_matrix_b,
            matrix_t       *p_result)
{
    if ((NULL == p_matrix_a) || (NULL == p_matrix_b) || (NULL == p_result) || (false == matrix_is_same_col_size(p_matrix_a, p_matrix_b)) || (false == matrix_is_same_row_size(p_matrix_a, p_matrix_b)))
    {
        return ARRAY_INVALID_ARGUMENT;
    }

    return ARRAY_SUCCESS;
}

matrix_error_code_t
matrix_subtract (const matrix_t *p_matrix_a,
                 const matrix_t *p_matrix_b,
                 matrix_t       *p_result)
{
    return ARRAY_SUCCESS;
}

matrix_error_code_t
matrix_multiply (const matrix_t *p_matrix_a,
                 const matrix_t *p_matrix_b,
                 matrix_t       *p_result)
{
    return ARRAY_SUCCESS;
}

matrix_error_code_t
matrix_scalar_multiply (const matrix_t *p_matrix,
                        double          scalar,
                        matrix_t       *p_result)
{
    return ARRAY_SUCCESS;
}

matrix_error_code_t
matrix_transpose (const matrix_t *p_matrix, matrix_t *p_result)
{
    return ARRAY_SUCCESS;
}

matrix_error_code_t
matrix_determinant (const matrix_t *p_matrix, double *p_result)
{
    return ARRAY_SUCCESS;
}

matrix_error_code_t
matrix_inverse (const matrix_t *p_matrix, matrix_t *p_result)
{
    return ARRAY_SUCCESS;
}

static matrix_error_code_t
matrix_error_from_array (array_error_code_t ret)
{
    switch (ret)
    {
        case ARRAY_SUCCESS:
            return MATRIX_SUCCESS;
        case ARRAY_NOT_FOUND:
            return MATRIX_NOT_FOUND;
        case ARRAY_OUT_OF_BOUNDS:
            return MATRIX_OUT_OF_BOUNDS;
        case ARRAY_INVALID_ARGUMENT:
            return MATRIX_INVALID_ARGUMENT;
        case ARRAY_ALLOCATION_FAILURE:
            return MATRIX_ALLOCATION_FAILURE;
        default:
            return MATRIX_FAILURE;
    }
}

static void
double_del (void *p_data)
{
    free(p_data);
}

static int
double_cmp (const void *p_double_a, const void *p_double_b)
{
    double double_a = *(const double *)p_double_a;
    double double_b = *(const double *)p_double_b;

    if (double_a < double_b)
    {
        return -1;
    }

    if (double_a > double_b)
    {
        return 1;
    }

    return 0;
}

static void
double_print (void *p_data, size_t index)
{
    if (p_data != NULL)
    {
        printf("(%zu: %f)", index, *(const double *)p_data);
    }

    return;
}

static void *
double_cpy (const void *p_src)
{
    if (NULL == p_src)
    {
        return NULL;
    }

    double *p_copy = calloc(1, sizeof(double));

    if (NULL != p_copy)
    {
        *p_copy = *(const double *)p_src;
    }

    return p_copy;
}

static bool
matrix_is_same_row_size (const matrix_t *p_matrix_a, const matrix_t *p_matrix_b)
{
    if ((NULL == p_matrix_a) || (NULL == p_matrix_b))
    {
        return false;
    }

    size_t row_a = 0;
    size_t row_b = 0;

    if ((MATRIX_SUCCESS == matrix_row_size(p_matrix_a, &row_a))
        && (MATRIX_SUCCESS == matrix_row_size(p_matrix_b, &row_b)))
    {
        return (row_a == row_b);
    }

    return false;
}

static bool
matrix_is_same_col_size (const matrix_t *p_matrix_a, const matrix_t *p_matrix_b)
{
    if ((NULL == p_matrix_a) || (NULL == p_matrix_b))
    {
        return false;
    }

    size_t col_a = 0;
    size_t col_b = 0;

    if ((MATRIX_SUCCESS == matrix_column_size(p_matrix_a, &col_a))
        && (MATRIX_SUCCESS == matrix_column_size(p_matrix_b, &col_b)))
    {
        return (col_a == col_b);
    }

    return false;
}

/*** end of file ***/
