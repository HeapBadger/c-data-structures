/**
 * @file    doubly_linked_list.h
 * @brief   Header for `doubly_linked_list.c`.
 *
 * @author  heapbadger
 */

#ifndef DL_H
#define DL_H

#include <stdbool.h>
#include <sys/types.h>
#include "auxiliary.h"

typedef enum
{
    DL_SUCCESS            = 0,  /**< Operation succeeded. */
    DL_NOT_FOUND          = -1, /**< Element not found. */
    DL_OUT_OF_BOUNDS      = -2, /**< Index out of range. */
    DL_INVALID_ARGUMENT   = -3, /**< Invalid argument. */
    DL_ALLOCATION_FAILURE = -4, /**< Memory allocation failed. */
} dl_error_code_t;

typedef struct dl_node
{
    void           *p_data;
    struct dl_node *p_next;
    struct dl_node *p_prev;
} dl_node_t;

typedef struct
{
    dl_node_t *p_head;
    del_func   del_f;
    cmp_func   cmp_f;
    print_func print_f;
} dl_t;

/**
 * @brief Creates and initializes a new doubly linked list.
 *
 * @param del_f   Function to delete node data.
 * @param cmp_f   Function to compare node data.
 * @param print_f Function to print node data.
 *
 * @return Pointer to the new list, or NULL on failure or invalid args.
 */
dl_t *dl_create(const del_func   del_f,
                const cmp_func   cmp_f,
                const print_func print_f);

/**
 * @brief Destroys the doubly linked list and frees all nodes and their data.
 *
 * @param p_list Pointer to the doubly linked list to destroy.
 *
 * @note After calling, the doubly linked list pointer is invalid.
 */
void dl_destroy(dl_t *p_list);

/**
 * @brief Clears all elements from the doubly linked list.
 *
 * @param p_list Pointer to the doubly linked list.
 */
ssize_t dl_clear(dl_t *p_list);

/**
 * @brief Prepends data as a new node at the start of the doubly linked list.
 *
 * @param p_list Pointer to the doubly linked list.
 * @param p_data Data to store in the new node.
 *
 * @return 0 on success, error code otherwise.
 */
ssize_t dl_preappend(dl_t *p_list, void *p_data);

/**
 * @brief Inserts data at a given index in the doubly linked list.
 *
 * Inserts at head if index is 0, appends if index exceeds list size.
 *
 * @param p_list Pointer to the doubly linked list.
 * @param p_data Data to store in the new node.
 * @param index  Position to insert at (0-based).
 *
 * @return 0 on success, error code otherwise.
 */
ssize_t dl_insert(dl_t *p_list, void *p_data, size_t index);

/**
 * @brief Deletes the node at a specified index.
 *
 * @param p_list Pointer to the doubly linked list.
 * @param index  Zero-based index of the node to delete.
 *
 * @return 0 on success, error code otherwise.
 */
ssize_t dl_del_at(dl_t *p_list, size_t index);

/**
 * @brief Checks whether the doubly linked list is empty.
 *
 * @param p_list Pointer to the doubly linked list.
 *
 * @return true if the doubly linked list is empty or NULL, false otherwise.
 */
bool dl_is_empty(const dl_t *p_list);

/**
 * @brief Returns the node at a specified index.
 *
 * @param p_list Pointer to the doubly linked list.
 * @param index  Zero-based index of the node.
 *
 * @return Pointer to the node, or NULL if out of bounds or list is NULL.
 */
dl_node_t *dl_at(const dl_t *p_list, size_t index);

/**
 * @brief Finds the index of the node containing specified data.
 *
 * Uses the doubly linked list’s compare function.
 *
 * @param p_list Pointer to the doubly linked list.
 * @param p_data Data to find.
 *
 * @return Zero-based index if found, -1 if not found or error.
 */
ssize_t dl_find(const dl_t *p_list, void *p_data);

/**
 * @brief Returns the number of nodes in the doubly linked list.
 *
 * @param p_list Pointer to the doubly linked list.
 *
 * @return Node count on success, error code otherwise
 */
ssize_t dl_size(const dl_t *p_list);

/**
 * @brief Reverses the order of nodes in the doubly linked list.
 *
 * @param p_list Pointer to the doubly linked list.
 *
 * @return 0 on success, error code otherwise.
 */
ssize_t dl_reverse(dl_t *p_list);

/**
 * @brief Prints all node data using the print function.
 *
 * @param p_list Pointer to the doubly linked list.
 */
void dl_print(const dl_t *p_list);

/**
 * @brief Swaps data between two nodes at given indices.
 *
 * @param p_list Pointer to the doubly linked list.
 * @param index_1 Index of first node.
 * @param index_2 Index of second node.
 *
 * @return 0 on success, error code otherwise.
 */
ssize_t dl_swap(dl_t *p_list, size_t index_1, size_t index_2);

/**
 * @brief Updates the data of a node at a given index.
 *
 * @param p_list Pointer to the doubly linked list.
 * @param index  Index of the node to update.
 * @param p_data New data to store.
 *
 * @return 0 on success, error code otherwise.
 *
 * @note Caller manages memory of p_data.
 */
ssize_t dl_update(dl_t *p_list, size_t index, void *p_data);

#endif // DL_H

/*** end of file ***/