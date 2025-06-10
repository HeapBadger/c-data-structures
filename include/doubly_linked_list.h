/**
 * @file    doubly_linked_list.h
 * @brief   Header for doubly_linked_list.c.
 *
 * @author  heapbadger
 * @date    August 26, 2024
 */

#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include "auxiliary.h"

typedef enum
{
    DOUBLY_LINKED_LIST_SUCCESS = 0,          /**< Operation succeeded. */
    DOUBLY_LINKED_LIST_NOT_FOUND = -1,       /**< Element not found. */
    DOUBLY_LINKED_LIST_OUT_OF_BOUNDS = -2,   /**< Index out of range. */
    DOUBLY_LINKED_LIST_INVALID_ARGUMENT = -3,/**< Invalid argument. */
    DOUBLY_LINKED_LIST_ALLOCATION_FAILURE = -4, /**< Memory allocation failed. */
} doubly_linked_list_error_code_t;

/**
 * @brief Node in the doubly linked list.
 *
 * Contains data pointer and links to next and previous nodes.
 */
typedef struct doubly_linked_list_node
{
    void                           *p_data;
    struct doubly_linked_list_node *p_next;
    struct doubly_linked_list_node *p_prev;
} doubly_linked_list_node_t;

/**
 * @brief Doubly linked list structure.
 *
 * Contains pointer to head node and function pointers for data management.
 */
typedef struct
{
    doubly_linked_list_node_t *p_head;
    del_func                   del_f;
    cmp_func                   cmp_f;
    print_func                 print_f;
} doubly_linked_list_t;

/**
 * @brief Creates and initializes a new doubly linked list.
 *
 * @param del_f   Function to delete node data.
 * @param cmp_f   Function to compare node data.
 * @param print_f Function to print node data.
 *
 * @return Pointer to the new list, or NULL on failure or invalid args.
 */
doubly_linked_list_t *doubly_linked_list_create(const del_func   del_f,
                                                const cmp_func   cmp_f,
                                                const print_func print_f);

/**
 * @brief Destroys the list and frees all nodes and their data.
 *
 * @param p_list Pointer to the list to destroy.
 *
 * @note After calling, the list pointer is invalid.
 */
void doubly_linked_list_destroy(doubly_linked_list_t *p_list);

/**
 * @brief Prepends data as a new node at the start of the list.
 *
 * @param p_list Pointer to the list.
 * @param p_data Data to store in the new node.
 *
 * @return 0 on success, error code otherwise.
 */
int doubly_linked_list_preappend(doubly_linked_list_t *p_list, void *p_data);

/**
 * @brief Inserts data at a given index in the list.
 *
 * Inserts at head if index is 0, appends if index exceeds list size.
 *
 * @param p_list Pointer to the list.
 * @param p_data Data to store in the new node.
 * @param index  Position to insert at (0-based).
 *
 * @return 0 on success, error code otherwise.
 */
int doubly_linked_list_insert(doubly_linked_list_t *p_list,
                              void                 *p_data,
                              int                   index);

/**
 * @brief Deletes the node at a specified index.
 *
 * @param p_list Pointer to the list.
 * @param index  Zero-based index of the node to delete.
 *
 * @return 0 on success, error code otherwise.
 */
int doubly_linked_list_del_at(doubly_linked_list_t *p_list, int index);

/**
 * @brief Returns the node at a specified index.
 *
 * @param p_list Pointer to the list.
 * @param index  Zero-based index of the node.
 *
 * @return Pointer to the node, or NULL if out of bounds or list is NULL.
 */
doubly_linked_list_node_t *doubly_linked_list_at(const doubly_linked_list_t *p_list,
                                                 int                   index);

/**
 * @brief Finds the index of the node containing specified data.
 *
 * Uses the list’s compare function.
 *
 * @param p_list Pointer to the list.
 * @param p_data Data to find.
 *
 * @return Zero-based index if found, -1 if not found or error.
 */
int doubly_linked_list_find(const doubly_linked_list_t *p_list, void *p_data);

/**
 * @brief Returns the number of nodes in the list.
 *
 * @param p_list Pointer to the list.
 *
 * @return Node count on success, error code otherwise
 */
int doubly_linked_list_size(const doubly_linked_list_t *p_list);

/**
 * @brief Reverses the order of nodes in the list.
 *
 * @param p_list Pointer to the list.
 *
 * @return 0 on success, error code otherwise.
 */
int doubly_linked_list_reverse(doubly_linked_list_t *p_list);

/**
 * @brief Prints all node data using the print function.
 *
 * @param p_list Pointer to the list.
 */
void doubly_linked_list_print(const doubly_linked_list_t *p_list);

/**
 * @brief Swaps data between two nodes at given indices.
 *
 * @param p_list Pointer to the list.
 * @param index_1 Index of first node.
 * @param index_2 Index of second node.
 *
 * @return 0 on success, error code otherwise.
 */
int doubly_linked_list_swap(doubly_linked_list_t *p_list,
                            int                   index_1,
                            int                   index_2);

/**
 * @brief Updates the data of a node at a given index.
 *
 * @param p_list Pointer to the list.
 * @param index  Index of the node to update.
 * @param p_data New data to store.
 *
 * @return 0 on success, error code otherwise.
 *
 * @note Caller manages memory of p_data.
 */
int doubly_linked_list_update(doubly_linked_list_t *p_list,
                              int                   index,
                              void                 *p_data);

#endif // DOUBLY_LINKED_LIST_H
