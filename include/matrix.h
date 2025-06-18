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
    array_t **pp_matrix;
    size_t    rows;
    size_t    cols;
} matrix_t;

/**
 * @brief Create a new matrix with given dimensions and function pointers.
 *
 * @param rows Number of rows in the matrix (must be > 0).
 * @param cols Number of columns in the matrix (must be > 0).
 * @param del_f Function pointer to delete a matrix element.
 * @param cmp_f Function pointer to compare matrix elements.
 * @param print_f Function pointer to print a matrix element.
 * @param cpy_f Function pointer to copy a matrix element.
 *
 * @return Pointer to a newly allocated matrix_t, or NULL on failure.
 */
matrix_t *matrix_create(size_t           rows,
                        size_t           cols,
                        const del_func   del_f,
                        const cmp_func   cmp_f,
                        const print_func print_f,
                        const copy_func  cpy_f);

/**
 * @brief Destroy a matrix and free all associated memory.
 *
 * @param p_matrix Pointer to the matrix to destroy (NULL safe).
 */
void matrix_destroy(matrix_t *p_matrix);

/**
 * @brief Clear the contents of a matrix, freeing internal arrays but not the
 * matrix itself.
 *
 * @param p_matrix Pointer to the matrix to clear (NULL safe).
 */
void matrix_clear(matrix_t *p_matrix);

/**
 * @brief Get the number of rows in the matrix.
 *
 * @param p_matrix Pointer to the matrix.
 * @param p_size Output parameter for the found size.
 *
 * @return MATRIX_SUCCESS on success, appropriate error code otherwise.
 */
matrix_error_code_t matrix_row_size(const matrix_t *p_matrix, size_t *p_size);

/**
 * @brief Get the number of columns in the matrix.
 *
 * @param p_matrix Pointer to the matrix.
 * @param p_size Output parameter for the found size.
 *
 * @return MATRIX_SUCCESS on success, appropriate error code otherwise.
 */
matrix_error_code_t matrix_column_size(const matrix_t *p_matrix,
                                       size_t         *p_size);

/**
 * @brief Fill the entire matrix with copies of the given value.
 *
 * @param p_matrix Pointer to the matrix to fill.
 * @param p_value Pointer to the value to copy into each element.
 *
 * @note Caller is responsible for freeing p_value.
 *
 * @return MATRIX_SUCCESS on success, error code on failure.
 */
matrix_error_code_t matrix_fill(matrix_t *p_matrix, void *p_value);

/**
 * @brief Insert a value at a specific row and column in the matrix.
 *
 * @param p_matrix Pointer to the matrix.
 * @param row Row index.
 * @param col Column index.
 * @param p_value Pointer to the value to insert.
 *
 * @return MATRIX_SUCCESS on success, error code on failure.
 */
matrix_error_code_t matrix_insert(matrix_t *p_matrix,
                                  size_t    row,
                                  size_t    col,
                                  void     *p_value);

/**
 * @brief Remove the value at a specific position in the matrix.
 *
 * @param p_matrix Pointer to the matrix.
 * @param row Row index.
 * @param col Column index.
 *
 * @return MATRIX_SUCCESS on success, error code on failure.
 */
matrix_error_code_t matrix_remove(matrix_t *p_matrix, size_t row, size_t col);

/**
 * @brief Retrieve the value at a specific row and column.
 *
 * @param p_matrix Pointer to the matrix.
 * @param row Row index.
 * @param col Column index.
 * @param[out] p_out Pointer to store the retrieved value.
 *
 * @return MATRIX_SUCCESS on success, error code on failure.
 */
matrix_error_code_t matrix_get(const matrix_t *p_matrix,
                               size_t          row,
                               size_t          col,
                               void          **p_out);

/**
 * @brief Set the value at a specific row and column.
 *
 * @param p_matrix Pointer to the matrix.
 * @param row Row index.
 * @param col Column index.
 * @param p_value Pointer to the value to set.
 *
 * @return MATRIX_SUCCESS on success, error code on failure.
 */
matrix_error_code_t matrix_set(matrix_t *p_matrix,
                               size_t    row,
                               size_t    col,
                               void     *p_value);

/**
 * @brief Find the first occurrence of a key in the matrix.
 *
 * @param p_matrix Pointer to the matrix.
 * @param p_key Pointer to the key to find.
 * @param[out] p_row Pointer to store the row index of found element.
 * @param[out] p_col Pointer to store the column index of found element.
 *
 * @return MATRIX_SUCCESS on success, error code on failure.
 */
matrix_error_code_t matrix_find(matrix_t *p_matrix,
                                void     *p_key,
                                size_t   *p_row,
                                size_t   *p_col);

/**
 * @brief Create a deep copy of a matrix.
 *
 * @param p_ori Pointer to the original matrix.
 *
 * @return Pointer to the copied matrix, or NULL on failure.
 */
matrix_t *matrix_clone(const matrix_t *p_ori);

/**
 * @brief Check if two matrices are equal in size and content.
 *
 * @param p_matrix_a Pointer to the first matrix.
 * @param p_matrix_b Pointer to the second matrix.
 *
 * @return true if equal, false otherwise.
 */
bool matrix_is_equal(const matrix_t *p_matrix_a, const matrix_t *p_matrix_b);

/**
 * @brief Print the matrix to standard output.
 *
 * @param p_matrix Pointer to the matrix to print.
 */
void matrix_print(const matrix_t *p_matrix);

#endif // MATRIX_H

/*** end of file ***/