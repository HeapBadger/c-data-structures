/**
 * @file doubly_linked_list.c
 * @brief Implementation of the doubly linked list data structure.
 *
 * @section operations Operations
 * - **Create**: Creates an empty doubly linked list.
 * - **Destroy**: Deletes all elements in the doubly linked list and destroys
 * the list.
 * - **Insert**: Adds a new element to the beginning, end, or any position in
 * the doubly linked list.
 * - **Delete**: Removes an element from the doubly linked list by position.
 * - **Search By Key**: Finds the position of an element by key.
 * - **Search By Index**: Finds an element in the doubly linked list by
 * position.
 * - **Size**: Returns the number of elements in the list.
 * - **Swap**: Swaps data between two nodes at given positions.
 * - **Update**: Updates the data in a node at a given position.
 *
 * @section complexity Complexity
 * - **Time Complexity**:   O(1) for insertion and deletion at the head,
 *                          O(N) for insertion, deletion, and search, where N is
 *                          the number of nodes.
 * - **Space Complexity**:  O(N) for storing elements in the doubly linked list,
 *                          where N is the number of nodes.
 *
 * @author  heapbadger
 * @date    August 26, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include "doubly_linked_list.h"

static void doubly_linked_list_del_node(doubly_linked_list_node_t *p_node,
                                        const del_func del_f);
static doubly_linked_list_node_t *doubly_linked_list_create_node(void *p_data);

doubly_linked_list_t *
doubly_linked_list_create (const del_func del_f, const cmp_func cmp_f, const print_func print_f)
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

int
doubly_linked_list_preappend (doubly_linked_list_t *p_list, void *p_data)
{
    return doubly_linked_list_insert(p_list, p_data, 0);
}

int
doubly_linked_list_insert (doubly_linked_list_t *p_list,
                           void                 *p_data,
                           int                   index)
{
    int p_return = DOUBLY_LINKED_LIST_SUCCESS;

    if ((NULL == p_list) || (NULL == p_list->del_f) || (NULL == p_data))
    {
        p_return = DOUBLY_LINKED_LIST_INVALID_ARGUMENT;
        goto EXIT;
    }

    if (0 > index)
    {
        p_return = DOUBLY_LINKED_LIST_OUT_OF_BOUNDS;
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
            p_return = DOUBLY_LINKED_LIST_OUT_OF_BOUNDS;
            doubly_linked_list_del_node(p_new_node, p_list->del_f);
        }
    }

EXIT:
    return p_return;
}

int
doubly_linked_list_del_at (doubly_linked_list_t *p_list, int index)
{
    int p_return = DOUBLY_LINKED_LIST_INVALID_ARGUMENT;

    if ((NULL != p_list) && (0 <= index) && (NULL != p_list->del_f))
    {
        int                        count     = 0;
        doubly_linked_list_node_t *p_current = p_list->p_head;
        p_return                             = DOUBLY_LINKED_LIST_OUT_OF_BOUNDS;

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
                p_return = DOUBLY_LINKED_LIST_SUCCESS;
                break;
            }

            count++;
            p_current = p_current->p_next;
        }
    }

    if (0 > index)
    {
        p_return = DOUBLY_LINKED_LIST_OUT_OF_BOUNDS;
    }

    return p_return;
}

doubly_linked_list_node_t *
doubly_linked_list_at (const doubly_linked_list_t *p_list, int index)
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

int
doubly_linked_list_find (const doubly_linked_list_t *p_list, void *p_data)
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

int
doubly_linked_list_size (const doubly_linked_list_t *p_list)
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

int
doubly_linked_list_reverse (doubly_linked_list_t *p_list)
{
    int p_return = DOUBLY_LINKED_LIST_INVALID_ARGUMENT;

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

        p_return = DOUBLY_LINKED_LIST_SUCCESS;
    }

    return p_return;
}

void
doubly_linked_list_print (const doubly_linked_list_t *p_list)
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
 * @param p_node Pointer to the node to be deleted.
 * @param del_f  Function to delete node data.
 * data.
 */
static void
doubly_linked_list_del_node (doubly_linked_list_node_t *p_node, const del_func del_f)
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
 * @param p_data Pointer to the data to be stored in the new node.
 *
 * @return doubly_linked_list_node_t* Pointer to the newly created node, or NULL
 * if memory allocation fails.
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
