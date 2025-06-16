/**
 * @file matrix.c
 * @brief Implementation of a matrix data structure.
 *
 * @author  heapbadger
 */

#include <stdlib.h>
#include "matrix.h"

/**
 * @brief Convert an array module return code to a matrix module error code.
 *
 * @param ret Return code from array function.
 *
 * @return Corresponding matrix_error_code_t.
 */
static matrix_error_code_t matrix_error_from_array(ssize_t ret);

/**
 * @brief Delete a value using the matrix's deletion function pointer.
 *
 * @param p_matrix Pointer to the matrix.
 * @param p_value Pointer to the value to delete.
 */
static void matrix_delete_value(matrix_t *p_matrix, void *p_value);

/**
 * @brief Allocate an empty matrix structure with uninitialized rows.
 *
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @param del_f Deletion function pointer.
 * @param cmp_f Comparison function pointer.
 * @param print_f Print function pointer.
 * @param cpy_f Copy function pointer.
 *
 * @return Pointer to allocated matrix_t or NULL on failure.
 */
static matrix_t *matrix_alloc_empty(size_t           rows,
                                    size_t           cols,
                                    const del_func   del_f,
                                    const cmp_func   cmp_f,
                                    const print_func print_f,
                                    const copy_func  cpy_f);

/**
 * @brief Create a deep copy of a single array (matrix row).
 *
 * @param p_src Pointer to the source array.
 *
 * @return Pointer to the copied array, or NULL on failure.
 */
static array_t *array_copy_row(const array_t *p_src);

matrix_t *
matrix_create (size_t           rows,
               size_t           cols,
               const del_func   del_f,
               const cmp_func   cmp_f,
               const print_func print_f,
               const copy_func  cpy_f)
{
    matrix_t *p_matrix
        = matrix_alloc_empty(rows, cols, del_f, cmp_f, print_f, cpy_f);

    if (NULL == p_matrix)
    {
        return NULL;
    }

    for (size_t idx = 0U; idx < rows; ++idx)
    {
        p_matrix->pp_matrix[idx]
            = array_create(cols, del_f, cmp_f, print_f, cpy_f);

        if (NULL == p_matrix->pp_matrix[idx])
        {
            matrix_destroy(p_matrix);
            p_matrix = NULL;
            break;
        }
    }

    return p_matrix;
}

void
matrix_destroy (matrix_t *p_matrix)
{
    if (NULL != p_matrix)
    {
        matrix_clear(p_matrix);
        free(p_matrix);
    }
}

void
matrix_clear (matrix_t *p_matrix)
{
    if (NULL != p_matrix)
    {
        if (NULL != p_matrix->pp_matrix)
        {
            for (size_t row = 0U; row < p_matrix->rows; ++row)
            {
                array_destroy(p_matrix->pp_matrix[row]);
            }

            free(p_matrix->pp_matrix);
            p_matrix->pp_matrix = NULL;
        }

        p_matrix->rows = 0U;
        p_matrix->cols = 0U;
    }
}

ssize_t
matrix_row_size (const matrix_t *p_matrix)
{
    ssize_t ret = MATRIX_INVALID_ARGUMENT;

    if (NULL != p_matrix)
    {
        ret = (ssize_t)p_matrix->rows;
    }

    return ret;
}

ssize_t
matrix_column_size (const matrix_t *p_matrix)
{
    ssize_t ret = MATRIX_INVALID_ARGUMENT;

    if (NULL != p_matrix)
    {
        ret = (ssize_t)p_matrix->cols;
    }

    return ret;
}

ssize_t
matrix_fill (matrix_t *p_matrix, void *p_value)
{
    ssize_t ret = MATRIX_INVALID_ARGUMENT;

    if ((NULL == p_matrix) || (NULL == p_value))
    {
        return ret;
    }

    ret = MATRIX_SUCCESS;

    for (size_t row = 0U; row < p_matrix->rows; ++row)
    {
        for (size_t col = 0U; col < p_matrix->cols; ++col)
        {
            void *p_copy = p_matrix->cpy_f(p_value);

            if (NULL == p_copy)
            {
                ret = MATRIX_ALLOCATION_FAILURE;
                break;
            }

            ret = matrix_error_from_array(
                array_set(p_matrix->pp_matrix[row], col, p_copy));

            if (MATRIX_SUCCESS != ret)
            {
                matrix_delete_value(p_matrix, p_copy);
                break;
            }
        }

        if (MATRIX_SUCCESS != ret)
        {
            break;
        }
    }

    return ret;
}

ssize_t
matrix_insert (matrix_t *p_matrix, size_t row, size_t col, void *p_value)
{
    ssize_t ret = MATRIX_INVALID_ARGUMENT;

    if ((NULL != p_matrix) && (NULL != p_value))
    {
        if ((row < p_matrix->rows) && (col < p_matrix->cols))
        {
            ret = matrix_error_from_array(
                array_insert(p_matrix->pp_matrix[row], col, p_value));
        }
        else
        {
            matrix_delete_value(p_matrix, p_value);
            ret = MATRIX_OUT_OF_BOUNDS;
        }
    }

    return ret;
}

ssize_t
matrix_remove (matrix_t *p_matrix, size_t row, size_t col)
{
    if (NULL == p_matrix)
    {
        return MATRIX_INVALID_ARGUMENT;
    }

    if ((row >= p_matrix->rows) || (col >= p_matrix->cols))
    {
        return MATRIX_OUT_OF_BOUNDS;
    }

    return matrix_error_from_array(array_remove(p_matrix->pp_matrix[row], col));
}

ssize_t
matrix_get (const matrix_t *p_matrix, size_t row, size_t col, void **p_out)
{
    if ((NULL == p_matrix) || (NULL == p_matrix->pp_matrix) || (NULL == p_out))
    {
        return MATRIX_INVALID_ARGUMENT;
    }

    if ((row >= p_matrix->rows) || (col >= p_matrix->cols))
    {
        return MATRIX_OUT_OF_BOUNDS;
    }

    return matrix_error_from_array(
        array_get(p_matrix->pp_matrix[row], col, p_out));
}

ssize_t
matrix_set (matrix_t *p_matrix, size_t row, size_t col, void *p_value)
{
    ssize_t ret = MATRIX_INVALID_ARGUMENT;

    if ((NULL != p_matrix) && (NULL != p_value))
    {
        if ((row < p_matrix->rows) && (col < p_matrix->cols))
        {
            ret = matrix_error_from_array(
                array_set(p_matrix->pp_matrix[row], col, p_value));
        }
        else
        {
            matrix_delete_value(p_matrix, p_value);
            ret = MATRIX_OUT_OF_BOUNDS;
        }
    }

    return ret;
}

ssize_t
matrix_find (matrix_t *p_matrix, void *p_key, size_t *p_row, size_t *p_col)
{
    if ((NULL == p_matrix) || (NULL == p_key) || (NULL == p_row)
        || (NULL == p_col))
    {
        return MATRIX_INVALID_ARGUMENT;
    }

    for (size_t row = 0U; row < p_matrix->rows; ++row)
    {
        ssize_t col = array_find(p_matrix->pp_matrix[row], p_key);

        if (col >= 0)
        {
            *p_row = row;
            *p_col = (size_t)col;
            return MATRIX_SUCCESS;
        }
    }

    return MATRIX_NOT_FOUND;
}

matrix_t *
matrix_copy (const matrix_t *p_ori)
{
    matrix_t *p_new = NULL;

    if ((NULL == p_ori) || (NULL == p_ori->pp_matrix))
    {
        goto EXIT;
    }

    p_new = matrix_alloc_empty(p_ori->rows,
                               p_ori->cols,
                               p_ori->del_f,
                               p_ori->cmp_f,
                               p_ori->print_f,
                               p_ori->cpy_f);

    if (NULL == p_new)
    {
        goto EXIT;
    }

    for (size_t row = 0U; row < p_ori->rows; ++row)
    {
        array_t *p_dst = array_copy_row(p_ori->pp_matrix[row]);

        if (NULL == p_dst)
        {
            matrix_destroy(p_new);
            p_new = NULL;
            goto EXIT;
        }

        p_new->pp_matrix[row] = p_dst;
    }

EXIT:
    return p_new;
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

    for (size_t row = 0U; row < p_matrix_a->rows; ++row)
    {
        if (!array_is_equal(p_matrix_a->pp_matrix[row],
                            p_matrix_b->pp_matrix[row]))
        {
            return false;
        }
    }

    return true;
}

void
matrix_print (const matrix_t *p_matrix)
{
    if (NULL == p_matrix)
    {
        return;
    }

    for (size_t row = 0; row < p_matrix->rows; row++)
    {
        for (size_t col = 0; col < p_matrix->cols; col++)
        {
            p_matrix->print_f(p_matrix->pp_matrix[row]->pp_array[col], row);
        }
    }
}

static matrix_error_code_t
matrix_error_from_array (ssize_t ret)
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
matrix_delete_value (matrix_t *p_matrix, void *p_value)
{
    if ((NULL != p_matrix) && (NULL != p_matrix->del_f) && (NULL != p_value))
    {
        p_matrix->del_f(p_value);
    }
}

static matrix_t *
matrix_alloc_empty (size_t           rows,
                    size_t           cols,
                    const del_func   del_f,
                    const cmp_func   cmp_f,
                    const print_func print_f,
                    const copy_func  cpy_f)
{
    matrix_t *p_matrix = NULL;

    if ((0 == rows) || (0 == cols) || (NULL == del_f) || (NULL == cmp_f)
        || (NULL == print_f) || (NULL == cpy_f))
    {
        goto EXIT;
    }

    p_matrix = calloc(1, sizeof(matrix_t));

    if (NULL == p_matrix)
    {
        goto EXIT;
    }

    p_matrix->pp_matrix = calloc(rows, sizeof(array_t *));

    if (NULL == p_matrix->pp_matrix)
    {
        free(p_matrix);
        p_matrix = NULL;
        goto EXIT;
    }

    p_matrix->rows    = rows;
    p_matrix->cols    = cols;
    p_matrix->del_f   = del_f;
    p_matrix->cmp_f   = cmp_f;
    p_matrix->print_f = print_f;
    p_matrix->cpy_f   = cpy_f;

EXIT:
    return p_matrix;
}

static array_t *
array_copy_row (const array_t *p_src)
{
    array_t *p_dst = NULL;

    if (NULL == p_src)
    {
        goto EXIT;
    }

    p_dst = array_create(
        p_src->cap, p_src->del_f, p_src->cmp_f, p_src->print_f, p_src->cpy_f);

    if (NULL == p_dst)
    {
        goto EXIT;
    }

    for (size_t col = 0U; col < p_src->len; ++col)
    {
        void *p_copy = p_src->cpy_f(p_src->pp_array[col]);

        if (NULL == p_copy)
        {
            array_destroy(p_dst);
            goto EXIT;
        }

        p_dst->pp_array[col] = p_copy;
        p_dst->len++;
    }

EXIT:
    return p_dst;
}

/*** end of file ***/
