/**
 * @file doubly_linked_list.c
 * @brief Implementation of the Doubly doubly linked list data structure.
 *
 * @section operations Operations
 * - **Create**: Creates an empty doubly linked list.
 * - **Destroy**: Deletes all elements in the doubly linked list and destroys
 * the list.
 * - **Insert**: Adds a new element to the beginning, end, or any position in
 * the doubly linked list.
 * - **Delete**: Removes an element from the doubly linked list by position.
 * - **Search By Key**: Finds an element position in the doubly linked list by
 * key.
 * - **Search By Index**: Finds an element in the doubly linked list by
 * position.
 * - **Size**: Finds size of doubly linked list.
 * - **Swap**: Swaps data between two nodes given position.
 * - **Update**: Updates the data in a node at a given position.
 *
 * @section complexity Complexity
 * - **Time Complexity**:   O(1) for insertion and deletion at the p_head,
 *                          O(N) for insertion, deletion, and search, where N is
 * the number of nodes.
 * - **Space Complexity**: O(N) for storing elements in the doubly linked list,
 * where N is the number of nodes.
 *
 * @author  Anna DeVries
 * @date    August 26, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include "doubly_linked_list.h"

// Function Declarations
static void doubly_linked_list_del_node(doubly_linked_list_node_t *p_node,
                                        del_func                   del_f);
static doubly_linked_list_node_t *doubly_linked_list_create_node(void *p_data);

// Creates doubly linked list
doubly_linked_list_t *
doubly_linked_list_create (del_func del_f, cmp_func cmp_f, print_func print_f)
{
    doubly_linked_list_t *p_list = NULL;

    if ((NULL != del_f) && (NULL != cmp_f) && (NULL != print_f))
    {
        p_list = calloc(1, sizeof(doubly_linked_list_t));

        if (NULL != p_list)
        {
            p_list->p_head  = NULL;
            p_list->cmp_f   = cmp_f;
            p_list->del_f   = del_f;
            p_list->print_f = print_f;
        }
    }

    return p_list;
}

// Destroys doubly linked list
void
doubly_linked_list_destroy (doubly_linked_list_t *p_list)
{
    if (NULL != p_list)
    {
        doubly_linked_list_node_t *p_head = p_list->p_head;

        while (NULL != p_head)
        {
            doubly_linked_list_node_t *p_current = p_head;
            p_head                               = p_head->p_next;
            doubly_linked_list_del_node(p_current, p_list->del_f);
        }

        p_list->cmp_f   = NULL;
        p_list->del_f   = NULL;
        p_list->print_f = NULL;

        free(p_list);
        p_list = NULL;
    }

    return;
}

// Pre-appends data into doubly linked list
doubly_linked_list_t *
doubly_linked_list_preappend (doubly_linked_list_t *p_list, void *p_data)
{
    return doubly_linked_list_insert(p_list, p_data, 0);
}

// Inserts data into doubly linked list at index
doubly_linked_list_t *
doubly_linked_list_insert (doubly_linked_list_t *p_list,
                           void                 *p_data,
                           int                   index)
{
    if ((NULL == p_list) || (NULL == p_list->del_f) || (NULL == p_data))
    {
        goto EXIT;
    }

    if (0 > index)
    {
        p_list->del_f(p_data);
        goto EXIT;
    }

    // create new node
    doubly_linked_list_node_t *p_new_node = NULL;
    p_new_node = doubly_linked_list_create_node(p_data);

    if (NULL == p_new_node)
    {
        goto EXIT;
    }

    // handle preappend operations
    if (0 == index)
    {
        p_new_node->p_next = p_list->p_head;

        if (p_list->p_head != NULL)
        {
            p_list->p_head->p_prev
                = p_new_node; // Update the previous pointer of the old head
        }

        p_list->p_head = p_new_node;
    }
    else
    {
        int                        count  = 0;
        doubly_linked_list_node_t *p_curr = p_list->p_head;
        doubly_linked_list_node_t *p_prev = NULL;

        while ((NULL != p_curr) && (count < index))
        {
            p_prev = p_curr;
            p_curr = p_curr->p_next;
            count++;
        }

        if (count == index) // handle insertion operation at specific index
        {
            p_new_node->p_next = p_curr;
            p_new_node->p_prev = p_prev;

            if (p_curr != NULL)
            {
                p_curr->p_prev = p_new_node; // Update the previous pointer of
                                             // the current node
            }

            p_prev->p_next = p_new_node;
        }
        else // count exceeds index; remove new node
        {
            doubly_linked_list_del_node(p_new_node, p_list->del_f);
        }
    }

EXIT:
    return p_list;
}

// Deletes data in doubly linked list at index
doubly_linked_list_t *
doubly_linked_list_del_at (doubly_linked_list_t *p_list, int index)
{
    if ((NULL != p_list) && (0 <= index) && (NULL != p_list->del_f))
    {
        int                        count     = 0;
        doubly_linked_list_node_t *p_current = p_list->p_head;

        while (NULL != p_current)
        {
            if (index == count)
            {
                // Handle deletion of the head node
                if (0 == index)
                {
                    p_list->p_head = p_current->p_next;
                    if (p_list->p_head)
                    {
                        p_list->p_head->p_prev
                            = NULL; // Update the new head's p_prev
                    }
                }
                else
                {
                    // Update the previous node's next pointer
                    if (p_current->p_prev)
                    {
                        p_current->p_prev->p_next = p_current->p_next;
                    }

                    // Update the next node's previous pointer
                    if (p_current->p_next)
                    {
                        p_current->p_next->p_prev = p_current->p_prev;
                    }
                }

                // Delete the current node
                doubly_linked_list_del_node(p_current, p_list->del_f);
                break;
            }

            count++;
            p_current = p_current->p_next;
        }
    }

    return p_list;
}

// Finds node data given index
doubly_linked_list_node_t *
doubly_linked_list_at (doubly_linked_list_t *p_list, int index)
{
    doubly_linked_list_node_t *p_current = NULL;
    doubly_linked_list_node_t *p_node    = NULL;

    if ((NULL != p_list) && (0 <= index) && (NULL != p_list->p_head))
    {
        int count = 0;
        p_current = p_list->p_head;

        while (NULL != p_current)
        {
            if (index == count)
            {
                p_node = p_current;
                break;
            }

            count++;
            p_current = p_current->p_next;
        }
    }

    return p_node;
}

// Finds node index given data
int
doubly_linked_list_find (doubly_linked_list_t *p_list, void *p_data)
{
    int index = DOUBLY_LINKED_LIST_NOT_FOUND;
    int count = 0;

    if ((NULL != p_list) && (NULL != p_data) && (NULL != p_list->p_head))
    {
        doubly_linked_list_node_t *p_current = p_list->p_head;

        while (NULL != p_current)
        {
            if (0 == p_list->cmp_f(p_data, p_current->p_data))
            {
                index = count;
                break;
            }

            count++;
            p_current = p_current->p_next;
        }
    }
    else
    {
        index = DOUBLY_LINKED_LIST_INVALID_ARGUMENT;
    }

    return index;
}

// Finds size of doubly linked list
int
doubly_linked_list_size (doubly_linked_list_t *p_list)
{
    int size = 0;

    if ((NULL != p_list) && (NULL != p_list->p_head))
    {
        doubly_linked_list_node_t *p_current = p_list->p_head;

        while (NULL != p_current)
        {
            size++;
            p_current = p_current->p_next;
        }
    }

    return size;
}

// Reverse a doubly linked list
doubly_linked_list_t *
doubly_linked_list_reverse (doubly_linked_list_t *p_list)
{
    if ((NULL != p_list) && (NULL != p_list->p_head))
    {
        doubly_linked_list_node_t *p_current = p_list->p_head;
        doubly_linked_list_node_t *p_next    = p_list->p_head->p_next;
        doubly_linked_list_node_t *p_prev    = NULL;

        while (NULL != p_current)
        {
            p_next            = p_current->p_next;
            p_current->p_next = p_prev;
            p_current->p_prev = p_next;
            p_prev            = p_current;
            p_current         = p_next;
        }

        p_list->p_head
            = p_prev; // Update the head of the list to the new first element
    }

    return p_list;
}

// Prints data from each node in list
void
doubly_linked_list_print (doubly_linked_list_t *p_list)
{
    if ((NULL != p_list) && (NULL != p_list->print_f)
        && (NULL != p_list->p_head))
    {
        printf("\nLinked List: ");
        doubly_linked_list_node_t *p_current = p_list->p_head;

        while (NULL != p_current)
        {
            p_list->print_f(p_current->p_data);
            p_current = p_current->p_next;
        }

        printf("\n\n");
    }

    return;
}

// Swaps the data of two nodes
int
doubly_linked_list_swap (doubly_linked_list_t *p_list, int index_1, int index_2)
{
    int result = DOUBLY_LINKED_LIST_OUT_OF_BOUNDS;

    if (NULL == p_list)
    {
        result = DOUBLY_LINKED_LIST_INVALID_ARGUMENT;
        goto EXIT;
    }

    if ((0 > index_1) || (0 > index_2))
    {
        goto EXIT;
    }

    doubly_linked_list_node_t *p_node_1
        = doubly_linked_list_at(p_list, index_1);
    doubly_linked_list_node_t *p_node_2
        = doubly_linked_list_at(p_list, index_2);

    if ((NULL != p_node_1) && (NULL != p_node_2))
    {
        void *p_data_1   = p_node_1->p_data;
        void *p_data_2   = p_node_2->p_data;
        p_node_1->p_data = p_data_2;
        p_node_2->p_data = p_data_1;
        result           = DOUBLY_LINKED_LIST_SUCCESS;
    }

EXIT:
    return result;
}

// Updates the data of a node
int
doubly_linked_list_update (doubly_linked_list_t *p_list,
                           int                   index,
                           void                 *p_data)
{
    int result = DOUBLY_LINKED_LIST_OUT_OF_BOUNDS;

    if ((NULL == p_list) || (NULL == p_data))
    {
        result = DOUBLY_LINKED_LIST_INVALID_ARGUMENT;
        goto EXIT;
    }

    if (0 > index)
    {
        goto EXIT;
    }

    doubly_linked_list_node_t *p_node = doubly_linked_list_at(p_list, index);

    if (NULL != p_node)
    {
        p_list->del_f(p_node->p_data);
        p_node->p_data = p_data;
        result         = DOUBLY_LINKED_LIST_SUCCESS;
    }

EXIT:
    return result;
}

/**
 * @brief Deletes a node from the doubly linked list and frees its memory.
 *
 * This function deallocates the memory used by a single node in the linked
 * list. It calls the provided delete function to handle the node's data and
 * then frees the memory allocated for the node itself. After freeing the
 * memory, it sets pointers in the node to `NULL` to avoid dangling pointers.
 *
 * @param p_node Pointer to the node to be deleted.
 * @param del_f  Function pointer to the function used to delete the node's
 * data.
 */
static void
doubly_linked_list_del_node (doubly_linked_list_node_t *p_node, del_func del_f)
{
    if (NULL != p_node)
    {
        if (NULL != del_f)
        {
            del_f(p_node->p_data);
            p_node->p_data = NULL;
            p_node->p_next = NULL;
            p_node->p_prev = NULL;
        }

        free(p_node);
        p_node = NULL;
    }

    return;
}

/**
 * @brief Creates and initializes a new node for the doubly linked list.
 *
 * This function allocates memory for a new node in the doubly linked list and
 * initializes its members. The node's data pointer is set to the provided data,
 * and the next pointer is set to NULL. The function returns a pointer to the
 * newly created node, or NULL if memory allocation fails.
 *
 * @param p_data Pointer to the data to be stored in the new node.
 *
 * @return doubly_linked_list_node_t* Pointer to the newly created node, or NULL
 * if memory allocation fails.
 *
 * @note The provided data pointer `p_data` is directly assigned to the node
 * without copying or validation. Ensure that the data pointer is valid and
 * properly managed by the caller.
 */
static doubly_linked_list_node_t *
doubly_linked_list_create_node (void *p_data)
{
    doubly_linked_list_node_t *p_node = NULL;
    p_node = calloc(1, sizeof(doubly_linked_list_node_t));

    if (NULL != p_node)
    {
        p_node->p_data = p_data;
        p_node->p_next = NULL;
        p_node->p_prev = NULL;
    }

    return p_node;
}
