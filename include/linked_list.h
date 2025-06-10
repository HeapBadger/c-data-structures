/**
 * @file    linked_list.h
 * @brief   Header for singly linked list implementation.
 *
 * @author  heapbadger
 * @date    August 21, 2024
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "auxiliary.h"

typedef enum
{
    LINKED_LIST_SUCCESS            = 0,  /**< Operation succeeded. */
    LINKED_LIST_NOT_FOUND          = -1, /**< Element not found. */
    LINKED_LIST_OUT_OF_BOUNDS      = -2, /**< Index out of range. */
    LINKED_LIST_INVALID_ARGUMENT   = -3, /**< Invalid argument provided. */
    LINKED_LIST_ALLOCATION_FAILURE = -4, /**< Memory allocation failed. */
} linked_list_error_code_t;

typedef struct linked_list_node
{
    void                    *p_data;
    struct linked_list_node *p_next;
} linked_list_node_t;

typedef struct
{
    linked_list_node_t *p_head;
    del_func            del_f;
    cmp_func            cmp_f;
    print_func          print_f;
} linked_list_t;

/**
 * @brief Creates a new linked list with custom handlers.
 *
 * @param del_f   Custom delete function.
 * @param cmp_f   Custom comparison function.
 * @param print_f Custom print function.
 *
 * @return Pointer to new list, or NULL on failure.
 */
linked_list_t *linked_list_create(const del_func   del_f,
                                  const cmp_func   cmp_f,
                                  const print_func print_f);

/**
 * @brief Frees the entire linked list and its data.
 *
 * @param p_list List to destroy.
 */
void linked_list_destroy(linked_list_t *p_list);

/**
 * @brief Inserts a new node at the start of the list.
 *
 * @param p_list Target list.
 * @param p_data Data to store.
 *
 * @return 0 on success, error code otherwise.
 */
int linked_list_preappend(linked_list_t *p_list, void *p_data);

/**
 * @brief Inserts a new node at a given index (or appends if out of bounds).
 *
 * @param p_list Target list.
 * @param p_data Data to store.
 * @param index  Position to insert.
 *
 * @return 0 on success, error code otherwise.
 */
int linked_list_insert(linked_list_t *p_list, void *p_data, int index);

/**
 * @brief Deletes the node at the specified index.
 *
 * @param p_list Target list.
 * @param index  Position to delete.
 *
 * @return 0 on success, error code otherwise.
 */
int linked_list_del_at(linked_list_t *p_list, int index);

/**
 * @brief Retrieves the node at the specified index.
 *
 * @param p_list Target list.
 * @param index  Position to retrieve.
 *
 * @return Pointer to node or NULL if invalid.
 */
linked_list_node_t *linked_list_at(const linked_list_t *p_list, int index);

/**
 * @brief Finds the index of the node containing matching data.
 *
 * @param p_list Target list.
 * @param p_data Data to find.
 *
 * @return Index on success, error code otherwise.
 */
int linked_list_find(const linked_list_t *p_list, void *p_data);

/**
 * @brief Returns the number of nodes in the list.
 *
 * @param p_list Target list.
 *
 * @return Number of nodes on success, error code.
 */
int linked_list_size(const linked_list_t *p_list);

/**
 * @brief Reverses the order of nodes in the list.
 *
 * @param p_list Target list.
 *
 * @return 0 on success, error code otherwise.
 */
int linked_list_reverse(linked_list_t *p_list);

/**
 * @brief Prints the data of all nodes using the print function.
 *
 * @param p_list Target list.
 */
void linked_list_print(const linked_list_t *p_list);

/**
 * @brief Swaps data between two nodes at given indices.
 *
 * @param p_list  Target list.
 * @param index_1 First node index.
 * @param index_2 Second node index.
 *
 * @return 0 on success, error code otherwise.
 */
int linked_list_swap(linked_list_t *p_list, int index_1, int index_2);

/**
 * @brief Updates the data at a specified node index.
 *
 * @param p_list Target list.
 * @param index  Node index.
 * @param p_data New data pointer.
 *
 * @return 0 on success, error code otherwise.
 */
int linked_list_update(linked_list_t *p_list, int index, void *p_data);

#endif // LINKED_LIST_H
