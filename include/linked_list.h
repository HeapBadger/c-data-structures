/**
 * @file    linked_list.h
 * @brief   Header for `linked_list.c`.
 *
 * @author  heapbadger
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include "auxiliary.h"

typedef enum
{
    LL_SUCCESS            = 0,  /**< Operation succeeded. */
    LL_NOT_FOUND          = -1, /**< Element not found. */
    LL_OUT_OF_BOUNDS      = -2, /**< Index out of range. */
    LL_INVALID_ARGUMENT   = -3, /**< Invalid argument provided. */
    LL_ALLOCATION_FAILURE = -4, /**< Memory allocation failed. */
    LL_EMPTY              = -5, /**< Empty list. */
} ll_error_code_t;

typedef struct ll_node
{
    void           *p_data;
    struct ll_node *p_next;
} ll_node_t;

typedef struct
{
    ll_node_t *p_head;
    del_func   del_f;
    cmp_func   cmp_f;
    print_func print_f;
    copy_func  cpy_f;
} ll_t;

/**
 * @brief Creates a new linked list with custom handlers.
 *
 * @param del_f   Custom delete function.
 * @param cmp_f   Custom comparison function.
 * @param print_f Custom print function.
 * @param cpy_f   Custom deep copy function.
 *
 * @return Pointer to new list, or NULL on failure.
 */
ll_t *ll_create(const del_func   del_f,
                const cmp_func   cmp_f,
                const print_func print_f,
                const copy_func  cpy_f);

/**
 * @brief Free all memory and destroy the linked list.
 *
 * @param p_list Pointer to the list to destroy.
 */
void ll_destroy(ll_t *p_list);

/**
 * @brief Remove all elements from the list.
 *
 * @param p_list Pointer to the list.
 */
void ll_clear(ll_t *p_list);

/**
 * @brief Deletes a single element using the registered delete function.
 *
 * @param p_list Pointer to the list.
 * @param p_value Pointer to the element to delete.
 */
void ll_del_ele(ll_t *p_list, void *p_value);

/**
 * @brief Insert an element at the end of the list.
 *
 * @param p_list Pointer to the list.
 * @param p_value Pointer to the value to insert.
 *
 * @return LL_SUCCESS on success, appropriate error code otherwise.
 */
ll_error_code_t ll_append(ll_t *p_list, void *p_data);

/**
 * @brief Insert an element at the specified index.
 *
 * @param p_list Pointer to the list.
 * @param p_value Pointer to the value to insert.
 * @param index Index to insert at.
 *
 * @return LL_SUCCESS on success, appropriate error code otherwise.
 */
ll_error_code_t ll_insert(ll_t *p_list, void *p_data, size_t index);

/**
 * @brief Remove the node at the specified index.
 *
 * @param p_list Pointer to the list.
 * @param index Index to remove from.
 *
 * @return LL_SUCCESS on success, appropriate error code otherwise.
 */
ll_error_code_t ll_del_at(ll_t *p_list, size_t index);

/**
 * @brief Retrieves the node at the specified index.
 *
 * @param p_list Pointer to the list.
 * @param index Position to retrieve.
 *
 * @return LL_SUCCESS on success, appropriate error code otherwise.
 */
ll_node_t *ll_at(const ll_t *p_list, size_t index);

/**
 * @brief Retrieves the first node in the list.
 *
 * @param p_list Pointer to the linked list.
 *
 * @return Pointer to a new list on success, or NULL on failure.
 */
ll_node_t *ll_head(const ll_t *p_list);

/**
 * @brief Retrieves the last node in the list.
 *
 * @param p_list Pointer to the linked list.
 * @return Pointer to the tail node, or NULL if empty or invalid.
 */
ll_node_t *ll_tail(const ll_t *p_list);

/**
 * @brief Find the index of the given key using the registered comparison
 * function.
 *
 * @param p_list Pointer to the list.
 * @param p_key Pointer to the key to find.
 * @param p_idx Output parameter for the found index.
 *
 * @return LL_SUCCESS on success, appropriate error code otherwise.
 */
ll_error_code_t ll_find(const ll_t *p_list, void *p_key, size_t *p_idx);

/**
 * @brief Check whether the list is empty.
 *
 * @param p_list Pointer to the list.
 *
 * @return true if empty, false otherwise.
 */
bool ll_is_empty(const ll_t *p_list);

/**
 * @brief Prints all elements in the list using the registered print function.
 *
 * @param p_list Point to the list to print.
 */
void ll_print(const ll_t *p_list);

/**
 * @brief Get the number of elements in the list.
 *
 * @param p_list Pointer to the list.
 * @param p_size Output parameter to store the list's current size.
 *
 * @return LL_SUCCESS on success, appropriate error code otherwise.
 */
ll_error_code_t ll_size(const ll_t *p_list, size_t *p_size);

/**
 * @brief Creates a deep copy of an element using the registered copy function.
 *
 * @param p_list Pointer to the list.
 * @param index Index to copy.
 * @param p_size Output parameter to store a copy of the data.
 *
 * @return LL_SUCCESS on success, appropriate error code otherwise.
 */
ll_error_code_t ll_clone_at(const ll_t *p_list, size_t index, void **p_copy);

/**
 * @brief Create a deep copy of the list structure.
 *
 * @param p_ori Pointer to the source list.
 *
 * @return Pointer to a new list on success, or NULL on failure.
 */
ll_t *ll_clone(const ll_t *p_ori);

/**
 * @brief Reverses the order of nodes in the list.
 *
 * @param p_list Pointer to the list.
 *
 * @return LL_SUCCESS on success, appropriate error code otherwise.
 */
ll_error_code_t ll_reverse(ll_t *p_list);

/**
 * @brief Apply a function to each element in the list.
 *
 * @param p_list Pointer to the list.
 * @param func Function to apply to each element.
 *
 * @return LL_SUCCESS on success, appropriate error code otherwise.
 */
ll_error_code_t ll_foreach(ll_t *p_list, foreach_func func);

#endif // LINKED_LIST_H

/*** end of file ***/
