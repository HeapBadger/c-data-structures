/**
 * @file    matrix.h
 * @brief   Header file for `matrix.c`.
 *
 * @author  heapbadger
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>
#include "array.h"

typedef enum
{
    MATRIX_SUCCESS            = 0,  /**< Operation succeeded. */
    MATRIX_NOT_FOUND          = -1, /**< Element not found. */
    MATRIX_OUT_OF_BOUNDS      = -2, /**< Index out of range. */
    MATRIX_INVALID_ARGUMENT   = -3, /**< Invalid argument provided. */
    MATRIX_ALLOCATION_FAILURE = -4, /**< Memory allocation failed. */
    MATRIX_FAILURE            = -5, /**< Generic failure. */
} matrix_error_code_t;

typedef struct
{
    size_t   rows;
    size_t   cols;
    array_t *p_flat;
} matrix_t;

/**
 * @brief Create a new matrix with given dimensions.
 *
 * Allocates memory for a rows x cols matrix initialized to 0.0.
 *
 * @param rows Number of rows (> 0).
 * @param cols Number of columns (> 0).
 * @return Pointer to newly allocated matrix_t, or NULL on failure.
 */
matrix_t *matrix_create(size_t rows, size_t cols);

/**
 * @brief Destroy a matrix and free all associated memory.
 *
 * @param p_matrix Pointer to matrix to destroy (NULL safe).
 */
void matrix_destroy(matrix_t *p_matrix);

/**
 * @brief Deletes a single element using the registered delete function.
 *
 * @param p_matrix Pointer to the matrix.
 * @param p_value Pointer to the element to delete.
 */
void matrix_del_ele(matrix_t *p_matrix, void *p_data);

/**
 * @brief Fill the entire matrix with a given value. Overwrites any
 * previous data.
 *
 * @param p_matrix Pointer to matrix.
 * @param value Value to fill the matrix with.
 * @return MATRIX_SUCCESS on success, error code on failure.
 */
matrix_error_code_t matrix_fill(matrix_t *p_matrix, double value);

/**
 * @brief Retrieve the value at a specific row and column.
 *
 * @param p_matrix Pointer to matrix.
 * @param row Row index (0-based).
 * @param col Column index (0-based).
 * @param p_out Output parameter to hold the retrieved element.
 * @return MATRIX_SUCCESS on success, error code on failure.
 */
matrix_error_code_t matrix_get(const matrix_t *p_matrix,
                               size_t          row,
                               size_t          col,
                               double         *p_out);

/**
 * @brief Set the value at a specific row and column.
 *
 * @param p_matrix Pointer to matrix.
 * @param row Row index (0-based).
 * @param col Column index (0-based).
 * @param value Value to set.
 * @return MATRIX_SUCCESS on success, error code on failure.
 */
matrix_error_code_t matrix_set(matrix_t *p_matrix,
                               size_t    row,
                               size_t    col,
                               double    value);

/**
 * @brief Check if two matrices are equal in size and contents.
 *
 * @param p_matrix_a Pointer to first matrix.
 * @param p_matrix_b Pointer to second matrix.
 * @return true if equal, false otherwise.
 */
bool matrix_is_equal(const matrix_t *p_matrix_a, const matrix_t *p_matrix_b);

/**
 * @brief Print the matrix to stdout in a readable format.
 *
 * @param p_matrix Pointer to matrix.
 */
void matrix_print(const matrix_t *p_matrix);

/**
 * @brief Get the number of rows in the matrix.
 *
 * @param p_matrix Pointer to matrix.
 * @param p_size Output parameter to store number of rows.
 * @return MATRIX_SUCCESS on success, error code on failure.
 */
matrix_error_code_t matrix_row_size(const matrix_t *p_matrix, size_t *p_size);

/**
 * @brief Get the number of columns in the matrix.
 *
 * @param p_matrix Pointer to matrix.
 * @param p_size Output parameter to store number of columns.
 * @return MATRIX_SUCCESS on success, error code on failure.
 */
matrix_error_code_t matrix_column_size(const matrix_t *p_matrix,
                                       size_t         *p_size);

/**
 * @brief Find the first occurrence of a value in the matrix.
 *
 * @param p_matrix Pointer to matrix.
 * @param key Value to find.
 * @param p_row Output parameter to hold the found row index.
 * @param p_col Output parameter to hold the found column index.
 *
 * @return MATRIX_SUCCESS if found, error code on failure.
 */
matrix_error_code_t matrix_find(const matrix_t *p_matrix,
                                double          key,
                                size_t         *p_row,
                                size_t         *p_col);

/**
 * @brief Create a deep copy of a matrix.
 *
 * @param p_ori Pointer to original matrix.
 * @return Pointer to cloned matrix, or NULL on failure.
 */
matrix_t *matrix_clone(const matrix_t *p_ori);

/**
 * @brief Add two matrices element-wise.
 *
 * Both matrices must have the same dimensions.
 *
 * @param p_matrix_a Pointer to first matrix.
 * @param p_matrix_b Pointer to second matrix.
 * @param p_result Output pointer to matrix where result will be stored.
 *                 Must be pre-allocated and of the same size as inputs.
 *
 * @return MATRIX_SUCCESS if found, error code on failure.
 */
matrix_error_code_t matrix_add(const matrix_t *p_matrix_a,
                               const matrix_t *p_matrix_b,
                               matrix_t       *p_result);

/**
 * @brief Subtract one matrix from another element-wise.
 *
 * Both matrices must have the same dimensions.
 *
 * @param p_matrix_a Pointer to first matrix.
 * @param p_matrix_b Pointer to second matrix.
 * @param p_result Output pointer to matrix where result will be stored.
 *                 Must be pre-allocated and of the same size as inputs.
 *
 * @return MATRIX_SUCCESS if found, error code on failure.
 */
matrix_error_code_t matrix_subtract(const matrix_t *p_matrix_a,
                                    const matrix_t *p_matrix_b,
                                    matrix_t       *p_result);

/**
 * @brief Multiply two matrices (matrix multiplication).
 *
 * The number of columns of the first matrix must equal the number of rows
 * of the second matrix.
 *
 * @param p_matrix_a Pointer to first matrix.
 * @param p_matrix_b Pointer to second matrix.
 * @param p_result Output pointer to matrix where result will be stored.
 *                 Must be pre-allocated with dimensions p_a->rows x p_b->cols.
 *
 * @return MATRIX_SUCCESS if found, error code on failure.
 */
matrix_error_code_t matrix_multiply(const matrix_t *p_matrix_a,
                                    const matrix_t *p_matrix_b,
                                    matrix_t       *p_result);

/**
 * @brief Multiply all elements of a matrix by a scalar value.
 *
 * The number of columns of the first matrix must equal the number of rows
 * of the second matrix.
 *
 * @param p_matrix Pointer to input matrix.
 * @param scalar Scalar value to multiply.
 * @param p_result Output pointer to matrix where result will be stored.
 *                 Must be pre-allocated and of the same size as input.
 *
 * @return MATRIX_SUCCESS if found, error code on failure.
 */
matrix_error_code_t matrix_scalar_multiply(const matrix_t *p_matrix,
                                           double          scalar,
                                           matrix_t       *p_result);

/**
 * @brief Compute the transpose of a matrix.
 *
 * The transpose swaps rows and columns of the input matrix.
 *
 * @param p_matrix Pointer to input matrix.
 * @param p_result Pointer to matrix where transpose will be stored.
 *                 Must be pre-allocated with dimensions
 *                 p_matrix->cols x p_matrix->rows.
 *
 * @return MATRIX_SUCCESS if found, error code on failure.
 */
matrix_error_code_t matrix_transpose(const matrix_t *p_matrix,
                                     matrix_t       *p_result);

/**
 * @brief Calculate the determinant of a square matrix.
 *
 * Only defined for square matrices.
 *
 * @param p_matrix Pointer to input square matrix.
 * @param p_result Pointer to double where determinant will be stored.
 *
 * @return MATRIX_SUCCESS if found, error code on failure.
 */
matrix_error_code_t matrix_determinant(const matrix_t *p_matrix,
                                       double         *p_result);

/**
 * @brief Compute the inverse of a square, invertible matrix.
 *
 * Only defined for square matrices with non-zero determinant.
 *
 * @param p_matrix Pointer to input square matrix.
 * @param p_result Pointer to matrix where inverse will be stored.
 *                 Must be pre-allocated and of same dimensions as input.
 *
 * @return MATRIX_SUCCESS if found, error code on failure.
 */
matrix_error_code_t matrix_inverse(const matrix_t *p_matrix,
                                   matrix_t       *p_result);

#endif // MATRIX_H

/*** end of file ***/