/**
 * @file    linked_list.h
 * @brief   Header for `linked_list.c`.
 *
 * @author  heapbadger
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include <sys/types.h>
#include "auxiliary.h"

typedef enum
{
    LL_SUCCESS            = 0,  /**< Operation succeeded. */
    LL_NOT_FOUND          = -1, /**< Element not found. */
    LL_OUT_OF_BOUNDS      = -2, /**< Index out of range. */
    LL_INVALID_ARGUMENT   = -3, /**< Invalid argument provided. */
    LL_ALLOCATION_FAILURE = -4, /**< Memory allocation failed. */
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
} ll_t;

/**
 * @brief Creates a new linked list with custom handlers.
 *
 * @param del_f   Custom delete function.
 * @param cmp_f   Custom comparison function.
 * @param print_f Custom print function.
 *
 * @return Pointer to new list, or NULL on failure.
 */
ll_t *ll_create(const del_func   del_f,
                const cmp_func   cmp_f,
                const print_func print_f);

/**
 * @brief Frees the entire linked list and its data.
 *
 * @param p_list List to destroy.
 */
void ll_destroy(ll_t *p_list);

/**
 * @brief Clears all elements from the linked list.
 *
 * @param p_list Pointer to the linked list.
 */
ssize_t ll_clear(ll_t *p_list);

/**
 * @brief Inserts a new node at the start of the list.
 *
 * @param p_list Target list.
 * @param p_data Data to store.
 *
 * @return 0 on success, error code otherwise.
 */
ssize_t ll_preappend(ll_t *p_list, void *p_data);

/**
 * @brief Inserts a new node at a given index (or appends if out of bounds).
 *
 * @param p_list Target list.
 * @param p_data Data to store.
 * @param index  Position to insert.
 *
 * @return 0 on success, error code otherwise.
 */
ssize_t ll_insert(ll_t *p_list, void *p_data, size_t index);

/**
 * @brief Deletes the node at the specified index.
 *
 * @param p_list Target list.
 * @param index  Position to delete.
 *
 * @return 0 on success, error code otherwise.
 */
ssize_t ll_del_at(ll_t *p_list, size_t index);

/**
 * @brief Checks whether the linked list is empty.
 *
 * @param p_list Pointer to the linked list.
 *
 * @return true if the list is empty or NULL, false otherwise.
 */
bool ll_is_empty(const ll_t *p_list);

/**
 * @brief Retrieves the node at the specified index.
 *
 * @param p_list Target list.
 * @param index  Position to retrieve.
 *
 * @return Pointer to node or NULL if invalid.
 */
ll_node_t *ll_at(const ll_t *p_list, size_t index);

/**
 * @brief Finds the index of the node containing matching data.
 *
 * @param p_list Target list.
 * @param p_data Data to find.
 *
 * @return Index on success, error code otherwise.
 */
ssize_t ll_find(const ll_t *p_list, void *p_data);

/**
 * @brief Returns the number of nodes in the list.
 *
 * @param p_list Target list.
 *
 * @return Number of nodes on success, error code.
 */
ssize_t ll_size(const ll_t *p_list);

/**
 * @brief Reverses the order of nodes in the list.
 *
 * @param p_list Target list.
 *
 * @return 0 on success, error code otherwise.
 */
ssize_t ll_reverse(ll_t *p_list);

/**
 * @brief Prints the data of all nodes using the print function.
 *
 * @param p_list Target list.
 */
void ll_print(const ll_t *p_list);

/**
 * @brief Swaps data between two nodes at given indices.
 *
 * @param p_list  Target list.
 * @param index_1 First node index.
 * @param index_2 Second node index.
 *
 * @return 0 on success, error code otherwise.
 */
ssize_t ll_swap(ll_t *p_list, size_t index_1, size_t index_2);

/**
 * @brief Updates the data at a specified node index.
 *
 * @param p_list Target list.
 * @param index  Node index.
 * @param p_data New data pointer.
 *
 * @return 0 on success, error code otherwise.
 */
ssize_t ll_update(ll_t *p_list, size_t index, void *p_data);

#endif // LINKED_LIST_H

/*** end of file ***/