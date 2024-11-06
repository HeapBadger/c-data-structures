/**
 * @file    circular_linked_list.h
 * @brief   Header file for `circular_linked_list.c`.
 *
 * @author  Anna DeVries
 * @date    August 27, 2024
 */

#ifndef CIRCULAR_LINKED_LIST_H
#define CIRCULAR_LINKED_LIST_H

typedef enum
{
    CIRCULAR_LINKED_LIST_SUCCESS = 0, /**< Operation completed successfully. */
    CIRCULAR_LINKED_LIST_NOT_FOUND, /**< The requested element was not found. */
    CIRCULAR_LINKED_LIST_OUT_OF_BOUNDS,    /**< An index was out of the valid
                                              range.*/
    CIRCULAR_LINKED_LIST_INVALID_ARGUMENT, /**< An invalid argument was
                                              provided.*/
} circular_linked_list_error_code_t;

/**
 * @brief Function pointer type for a custom delete function.
 *
 * This function pointer is intended to delete or free the data stored
 * within a circular linked list node. For simple data types (e.g., integers,
 * basic structs), this can be as straightforward as pointing to the
 * `free` function.
 *
 * @param p_data Pointer to the data within the node that needs to be deleted.
 *
 * @note The function should handle any necessary cleanup to avoid memory leaks.
 *
 * @example
 * To delete integers/chars/strings:
 * @code
 * void delete_function(void *p_data) {
 *     free(p_data);
 *     p_data = NULL;
 * }
 * @endcode
 */
typedef void (*del_func)(void *p_data);

/**
 * @brief Function pointer type for a custom print function.
 *
 * This function pointer is used to print the data stored within a
 * circular linked list node. The specific implementation of this function
 * should be tailored to the data type stored in the node.
 *
 * @param p_data Pointer to the data within the node that needs to be printed.
 *
 * @note Ensure the function properly formats the output according to the
 *       data type being printed.
 *
 * @example
 * To print integers:
 * @code
 * void print_int(void *p_data) {
 *     printf("%d\n", *(int *)p_data);
 * }
 * @endcode
 *
 * To print characters:
 * @code
 * void print_char(void *p_data) {
 *     printf("%c\n", *(char *)p_data);
 * }
 * @endcode
 *
 * To print strings:
 * @code
 * void print_string(void *p_data) {
 *     printf("%s\n", (char *)p_data);
 * }
 * @endcode
 */
typedef void (*print_func)(void *p_data);

/**
 * @brief Function pointer type for a custom comparison function.
 *
 * This function pointer is designed to compare two arbitrary data types
 * stored within circular linked list nodes. The comparison function should
 * return an integer value that indicates the relative order of the two data
 * items, similar to the behavior of the standard `strcmp` function.
 *
 * @param p_lhs Pointer to the left-hand side data for comparison.
 * @param p_rhs Pointer to the right-hand side data for comparison.
 *
 * @returns
 * - A negative value if p_lhs is less than p_rhs.
 * - Zero if p_lhs is equal to p_rhs.
 * - A positive value if p_lhs is greater than p_rhs.
 *
 * @note The return values are intended to be compatible with standard
 *       sorting and comparison functions, following the conventions
 *       established by `strcmp`.
 *
 * @example
 * To compare integers:
 * @code
 * int compare_ints(void *p_lhs, void *p_rhs) {
 *     int lhs = *(int *)p_lhs;
 *     int rhs = *(int *)p_rhs;
 *     return (lhs > rhs) - (lhs < rhs);
 * }
 * @endcode
 *
 * To compare characters:
 * @code
 * int compare_chars(void *p_lhs, void *p_rhs) {
 *     char lhs = *(char *)p_lhs;
 *     char rhs = *(char *)p_rhs;
 *     return (lhs > rhs) - (lhs < rhs);
 * }
 * @endcode
 *
 * To compare strings:
 * @code
 * int compare_strings(void *p_lhs, void *p_rhs) {
 *     return strcmp((char *)p_lhs, (char *)p_rhs);
 * }
 * @endcode
 */
typedef int (*cmp_func)(void *p_lhs, void *p_rhs);

/**
 * @brief Structure representing a node in the circular linked list.
 *
 * This structure represents a single node in a circular linked list. Each node
 * contains a pointer to the data it holds and a pointer to the next node in the
 * list. It allows the circular linked list to manage and traverse a sequence of
 * elements.
 */
typedef struct circular_linked_list_node
{
    void                             *p_data;
    struct circular_linked_list_node *p_next;
} circular_linked_list_node_t;

/**
 * @brief Structure representing a circular linked list.
 *
 * This structure represents the entire circular linked list. It includes a
 * pointer to the head node of the list and function pointers for managing the
 * nodes' data. The functions provided allow for deleting, comparing, and
 * printing node data, facilitating custom behavior for the list.
 */
typedef struct
{
    circular_linked_list_node_t *p_head;
    del_func                     del_f;
    cmp_func                     cmp_f;
    print_func                   print_f;
} circular_linked_list_t;

/**
 * @brief Creates and initializes a new circular linked list.
 *
 * This function allocates memory for a new circular linked list and initializes
 * its members. The circular linked list is configured with custom delete,
 * compare, and print functions provided by the user.
 *
 * @param del_f   Pointer to a function that will be used to delete a node's
 * data.
 * @param cmp_f   Pointer to a function that will be used to compare two nodes'
 * data.
 * @param print_f Pointer to a function that will be used to print a node's
 * data.
 *
 * @return circular_linked_list_t* Pointer to the newly created circular linked
 * list, or NULL if memory allocation fails or if any provided function pointer
 * is NULL.
 */
circular_linked_list_t *circular_linked_list_create(del_func   del_f,
                                                    cmp_func   cmp_f,
                                                    print_func print_f);

/**
 * @brief Destroys the circular linked list and frees its resources.
 *
 * This function iterates through the circular linked list, freeing the memory
 * allocated for each node's data using the delete function pointer. It then
 * frees the memory for each node and finally frees the memory allocated for the
 * linked list structure itself. The function sets function pointers to NULL
 * after use to prevent potential dangling pointer issues.
 *
 * @param p_list Pointer to the circular linked list to be destroyed.
 *
 * @note The circular linked list itself, including all its nodes and data, will
 * be deallocated. After calling this function, the provided pointer to the
 * linked list will be invalid.
 */
void circular_linked_list_destroy(circular_linked_list_t *p_list);

/**
 * @brief Prepends a new node with the given data to the start of the circular
 * linked list.
 *
 * This function is a convenience wrapper around `circular_linked_list_insert`
 * that inserts a new node at the head (index 0) of the circular linked list.
 * The new node becomes the first node in the list.
 *
 * @param p_list Pointer to the circular linked list where the node will be
 * prepended.
 * @param p_data Pointer to the data to be stored in the new node.
 *
 * @return int Error code indicating the result of the operation.
 *
 * @note The function assumes that the `p_data` pointer is valid and properly
 * managed by the caller. It may return an error code if the list pointer or
 * data pointer is invalid, or if memory allocation for the new node fails.
 */
int circular_linked_list_preappend(circular_linked_list_t *p_list,
                                   void                   *p_data);

/**
 * @brief Inserts a new node with the given data at the specified index in the
 * circular linked list.
 *
 * This function inserts a new node at the specified index in the circular
 * linked list. If the index is 0, the new node is inserted at the head. If the
 * index is greater than the number of nodes in the list, the function appends
 * the node at the end.
 *
 * @param p_list Pointer to the circular linked list.
 * @param p_data Pointer to the data to be stored in the new node.
 * @param index  The position at which to insert the new node (0-based index).
 *
 * @return int Error code indicating the result of the operation.
 *
 * @note The function assumes that the `p_data` pointer is valid and properly
 * managed by the caller.
 */
int circular_linked_list_insert(circular_linked_list_t *p_list,
                                void                   *p_data,
                                int                     index);

/**
 * @brief Deletes the node at a specified index in a circular linked list.
 *
 * This function traverses the circular linked list to find the node at the
 * given index and deletes it using the node deletion function specified in the
 * circular linked list. The function handles deletion of both the head node and
 * nodes in the middle or end of the list.
 *
 * @param p_list  A pointer to the circular linked list from which the node will
 * be deleted. This must be a valid circular linked list structure with an
 * initialized deletion function.
 * @param index   The zero-based index of the node to delete. Indexing starts at
 * 0.
 *
 * @return A pointer to the updated circular linked list. The return value is
 * the same as `p_list`. Returns the circular linked list structure after
 * removing the specified node. If the index is out of bounds or if the circular
 * linked list or deletion function is NULL, the function returns the original
 * circular linked list unchanged.
 */
circular_linked_list_t *circular_linked_list_del_at(
    circular_linked_list_t *p_list, int index);

/**
 * @brief Retrieves the node at a specified index in a circular linked list.
 *
 * This function traverses the circular linked list starting from the head and
 * returns the node at the given index. If the index is out of bounds or the
 * list is NULL, the function returns NULL.
 *
 * @param p_list  A pointer to the circular linked list from which the node is
 * to be retrieved. This must be a valid circular linked list structure.
 * @param index   The zero-based index of the node to retrieve. Indexing starts
 * at 0.
 *
 * @return A pointer to the node at the specified index, or NULL if the index is
 * out of bounds or if the circular linked list is NULL. The returned pointer
 * should be treated as a node in the circular linked list and not directly
 * modified.
 */
circular_linked_list_node_t *circular_linked_list_at(
    circular_linked_list_t *p_list, int index);

/**
 * @brief Finds the index of a node in a circular linked list containing the
 * specified data.
 *
 * This function searches the circular linked list for a node that contains data
 * matching the provided data. The comparison is done using the comparison
 * function pointer provided when the circular linked list was created. If a
 * matching node is found, the function returns the zero-based index of that
 * node. If no matching node is found or if the circular linked list or data is
 * NULL, the function returns -1.
 *
 * @param p_list  A pointer to the circular linked list to search. This must be
 * a valid circular linked list structure with an initialized comparison
 * function.
 * @param p_data  A pointer to the data to find in the circular linked list. The
 * data is compared using the comparison function specified in the circular
 * linked list.
 *
 * @return The zero-based index of the first node containing the matching data,
 * or -1 if no matching node is found or if the circular linked list or data is
 * NULL. The return value of -1 indicates that the data was not found in the
 * list.
 */
int circular_linked_list_find(circular_linked_list_t *p_list, void *p_data);

/**
 * @brief Computes the number of nodes in a circular linked list.
 *
 * This function traverses the circular linked list starting from the head node
 * and counts the total number of nodes present in the list.
 *
 * @param p_list  A pointer to the circular linked list whose size is to be
 * determined. This pointer should be valid and not NULL.
 *
 * @return The number of nodes in the circular linked list. If the list is NULL,
 * the function returns 0.
 */
int circular_linked_list_size(circular_linked_list_t *p_list);

/**
 * @brief Reverses the circular linked list.
 *
 * This function reverses the order of the nodes in a circular linked list. The
 * head of the list is updated to point to the last node, effectively inverting
 * the order of the list.
 *
 * @param p_list A pointer to the circular linked list to be reversed.
 *
 * @return A pointer to the reversed circular linked list.
 */
circular_linked_list_t *circular_linked_list_reverse(
    circular_linked_list_t *p_list);

/**
 * @brief Prints the data in each node of the circular linked list.
 *
 * This function traverses the circular linked list and uses the provided print
 * function pointer to print the data stored in each node. The traversal
 * continues until all nodes have been processed. The function does nothing if
 * the circular linked list or the print function is `NULL`.
 *
 * @param p_list Pointer to the circular linked list to be printed.
 */
void circular_linked_list_print(circular_linked_list_t *p_list);

/**
 * @brief Swaps the data of two nodes in the circular linked list.
 *
 * This function swaps the data stored in the nodes at the specified indices
 * within the circular linked list. If either index is out of bounds or the list
 * is `NULL`, the function returns an error.
 *
 * @param p_list Pointer to the linked list in which the swap will occur.
 * @param index_1 The index of the first node.
 * @param index_2 The index of the second node.
 *
 * @return `0` on success, or a non-zero error code if the operation fails
 *         (e.g., due to invalid indices or a `NULL` pointer).
 */
int circular_linked_list_swap(circular_linked_list_t *p_list,
                              int                     index_1,
                              int                     index_2);

/**
 * @brief Updates the data of a node at a specified index in the circular linked
 * list.
 *
 * This function updates the data stored in the node at the specified index
 * within the linked list with the provided data. If the index is out of bounds
 * or the list is `NULL`, the function returns an error.
 *
 * @param p_list Pointer to the linked list in which the update will occur.
 * @param index The index of the node to be updated.
 * @param p_data Pointer to the new data to be stored in the node.
 *
 * @return `0` on success, or a non-zero error code if the operation fails
 *         (e.g., due to an invalid index, a `NULL` pointer, or a `NULL` data
 * pointer).
 *
 * @note The caller must ensure that `p_data` is valid and properly allocated
 *       before calling this function. If the function fails, the caller is
 *       responsible for freeing the memory allocated for `p_data`.
 */
int circular_linked_list_update(circular_linked_list_t *p_list,
                                int                     index,
                                void                   *p_data);

#endif // CIRCULAR_LINKED_LIST_H
