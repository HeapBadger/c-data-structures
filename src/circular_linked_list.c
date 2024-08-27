/**
 * @file circular_linked_list.c
 * @brief Implementation of the circular circular linked list data structure.
 *
 * @section operations Operations
 * - **Create**: Creates an empty circular linked list.
 * - **Destroy**: Deletes all elements in the circular linked list and destroys
 * the list.
 * - **Insert**: Adds a new element to the beginning, end, or any position in
 * the circular linked list.
 * - **Delete**: Removes an element from the circular linked list by position.
 * - **Search By Key**: Finds an element position in the circular linked list by
 * key.
 * - **Search By Index**: Finds an element in the circular linked list by
 * position.
 * - **Size**: Finds size of circular linked list.
 * - **Swap**: Swaps data between two nodes given position.
 * - **Update**: Updates the data in a node at a given position.
 *
 * @section complexity Complexity
 * - **Time Complexity**:   O(1) for insertion and deletion at the p_head,
 *                          O(N) for insertion, deletion, and search, where N is
 * the number of nodes.
 * - **Space Complexity**: O(N) for storing elements in the circular linked list,
 * where N is the number of nodes.
 *
 * @author  Anna DeVries
 * @date    August 27, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include "circular_linked_list.h"

// Function Declarations
static void circular_linked_list_del_node(circular_linked_list_node_t *p_node,
                                        del_func                   del_f);
static circular_linked_list_node_t *circular_linked_list_create_node(void *p_data);

// Creates circular linked list
circular_linked_list_t *
circular_linked_list_create (del_func del_f, cmp_func cmp_f, print_func print_f)
{
    circular_linked_list_t *p_list = NULL;

    if ((NULL != del_f) && (NULL != cmp_f) && (NULL != print_f))
    {
        p_list = calloc(1, sizeof(circular_linked_list_t));

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

// Destroys circular linked list
void
circular_linked_list_destroy (circular_linked_list_t *p_list)
{
    if (NULL != p_list)
    {
        circular_linked_list_node_t *p_head = p_list->p_head;

        if (NULL != p_head)
        {
            circular_linked_list_node_t *p_current = p_head->p_next;

            while (p_current != p_head)
            {
                circular_linked_list_node_t *next = p_current->p_next;
                circular_linked_list_del_node(p_current, p_list->del_f);
                p_current = next;
            }

            circular_linked_list_del_node(p_head, p_list->del_f);
        }
    
        p_list->cmp_f   = NULL;
        p_list->del_f   = NULL;
        p_list->print_f = NULL;

        free(p_list);
        p_list = NULL;
    }

    return;
}

// Pre-appends data into circular linked list
circular_linked_list_t *
circular_linked_list_preappend (circular_linked_list_t *p_list, void *p_data)
{
    return circular_linked_list_insert(p_list, p_data, 0);
}

// Inserts data into circular linked list at index
circular_linked_list_t *
circular_linked_list_insert (circular_linked_list_t *p_list, void *p_data,  int index)
{
    // handle invalid inputs
    if ((NULL == p_list) || (NULL == p_list->del_f) || (NULL == p_data))
    {
        goto EXIT;
    }

    // handle invalid index
    if (0 > index)
    {
        p_list->del_f(p_data);
        goto EXIT;
    }

    // create new node
    circular_linked_list_node_t *p_new_node = NULL;
    p_new_node = circular_linked_list_create_node(p_data);

    if (NULL == p_new_node)
    {
        goto EXIT;
    }

    // Case 1: List is empty
    if (NULL == p_list->p_head)
    {
        p_list->p_head = p_new_node;
        goto EXIT;
    }

    // Traverse the list to find the insertion point or reach the last node
    circular_linked_list_node_t *p_curr = p_list->p_head;
    circular_linked_list_node_t *p_prev = NULL;
    int count = 0;

    do
    {
        p_prev = p_curr;
        p_curr = p_curr->p_next;
        count++;
    } while ((p_curr != p_list->p_head) && (count < index));

    if (index == 0)
    { // Case 2: Insert at the beginning
        p_new_node->p_next = p_list->p_head;
        p_prev->p_next = p_new_node;  
        p_list->p_head = p_new_node;
    }
    else
    { // Case 3: Insert at the middle or end
        p_new_node->p_next = p_curr;
        p_prev->p_next = p_new_node;
    }

EXIT:
    return p_list;
}

// Deletes data in circular linked list at index
circular_linked_list_t *
circular_linked_list_del_at (circular_linked_list_t *p_list, int index)
{
    if ((NULL == p_list) || (index < 0) || (NULL == p_list->p_head))
    {
        goto EXIT;
    }

    circular_linked_list_node_t *p_curr = p_list->p_head;
    circular_linked_list_node_t *p_prev = NULL;

    // Case 1: Deleting the head (index 0)
    if (index == 0)
    {
        // If the list has only one node
        if (p_curr->p_next == p_list->p_head)
        {
            circular_linked_list_del_node(p_curr, p_list->del_f);
            p_list->p_head = NULL;
            goto EXIT;
        }

        // Find the last node to update its p_next after head deletion
        while (p_curr->p_next != p_list->p_head)
        {
            p_curr = p_curr->p_next;
        }

        // Update pointers and delete the old head
        p_prev = p_list->p_head;
        p_curr->p_next = p_list->p_head->p_next;
        p_list->p_head = p_list->p_head->p_next;
        circular_linked_list_del_node(p_prev, p_list->del_f);
        goto EXIT;
    }

    // Traverse the list to find the node at the specified index
    int count = 0;
    do
    {
        p_prev = p_curr;
        p_curr = p_curr->p_next;
        count++;
    } while ((p_curr != p_list->p_head) && (count < index));

    // Case 2: Delete at the middle or end
    if (count == index)
    {
        p_prev->p_next = p_curr->p_next;
        circular_linked_list_del_node(p_curr, p_list->del_f);
    }

EXIT:
    return p_list;
}

// Finds node data given index
circular_linked_list_node_t *
circular_linked_list_at (circular_linked_list_t *p_list, int index)
{
    // // Traverse the list to find the insertion point or reach the last node
    // circular_linked_list_node_t *p_curr = p_list->p_head;
    // // circular_linked_list_node_t *p_prev = NULL;
    // circular_linked_list_node_t *p_node = NULL;
    // int count = 0;

    // do
    // {
    //     // p_prev = p_curr;
    //     p_curr = p_curr->p_next;
    //     count++;
    // } while ((p_list->p_head != p_curr) && (index > count));

    // if (index == count)
    // {
    //     p_node = p_curr;
    // }

    // return p_node;


    circular_linked_list_node_t *p_current = NULL;
    circular_linked_list_node_t *p_node    = NULL;

    if ((NULL != p_list) && (0 <= index))
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
circular_linked_list_find (circular_linked_list_t *p_list, void *p_data)
{
    int index = CIRCULAR_LINKED_LIST_NOT_FOUND;
    int count = 0;

    if ((NULL != p_list) && (NULL != p_data))
    {
        circular_linked_list_node_t *p_current = p_list->p_head;

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

    return index;
}

// Finds size of circular linked list
int
circular_linked_list_size (circular_linked_list_t *p_list)
{
    int size = 0;

    if (NULL != p_list)
    {
        circular_linked_list_node_t *p_current = p_list->p_head;

        while (NULL != p_current)
        {
            size++;
            p_current = p_current->p_next;
        }
    }

    return size;
}

// Reverse a circular linked list
circular_linked_list_t *
circular_linked_list_reverse (circular_linked_list_t *p_list)
{
    if (NULL != p_list)
    {
        circular_linked_list_node_t *p_current = p_list->p_head;
        circular_linked_list_node_t *p_next    = p_list->p_head->p_next;
        circular_linked_list_node_t *p_prev    = NULL;

        while (NULL != p_current)
        {
            p_next            = p_current->p_next;
            p_current->p_next = p_prev;
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
circular_linked_list_print (circular_linked_list_t *p_list)
{
    if ((NULL != p_list) && (NULL != p_list->print_f))
    {
        printf("\nLinked List: ");
        circular_linked_list_node_t *current = p_list->p_head;

        while (NULL != current)
        {
            p_list->print_f(current->p_data);
            current = current->p_next;
        }

        printf("\n\n");
    }

    return;
}

// Swaps the data of two nodes
int
circular_linked_list_swap (circular_linked_list_t *p_list, int index_1, int index_2)
{
    int result = CIRCULAR_LINKED_LIST_OUT_OF_BOUNDS;

    if (NULL == p_list)
    {
        result = CIRCULAR_LINKED_LIST_INVALID_ARGUMENT;
        goto EXIT;
    }

    if ((0 > index_1) || (0 > index_2))
    {
        goto EXIT;
    }

    circular_linked_list_node_t *p_node_1
        = circular_linked_list_at(p_list, index_1);
    circular_linked_list_node_t *p_node_2
        = circular_linked_list_at(p_list, index_2);

    if ((NULL != p_node_1) && (NULL != p_node_2))
    {
        void *p_data_1   = p_node_1->p_data;
        void *p_data_2   = p_node_2->p_data;
        p_node_1->p_data = p_data_2;
        p_node_2->p_data = p_data_1;
        result           = CIRCULAR_LINKED_LIST_SUCCESS;
    }

EXIT:
    return result;
}

// Updates the data of a node
int
circular_linked_list_update (circular_linked_list_t *p_list,
                           int                   index,
                           void                 *p_data)
{
    int result = CIRCULAR_LINKED_LIST_OUT_OF_BOUNDS;

    if ((NULL == p_list) || (NULL == p_data))
    {
        result = CIRCULAR_LINKED_LIST_INVALID_ARGUMENT;
        goto EXIT;
    }

    if (0 > index)
    {
        goto EXIT;
    }

    circular_linked_list_node_t *p_node = circular_linked_list_at(p_list, index);

    if (NULL != p_node)
    {
        p_list->del_f(p_node->p_data);
        p_node->p_data = p_data;
        result         = CIRCULAR_LINKED_LIST_SUCCESS;
    }

EXIT:
    return result;
}

/**
 * @brief Deletes a node from the circular linked list and frees its memory.
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
circular_linked_list_del_node (circular_linked_list_node_t *p_node, del_func del_f)
{
    if (NULL != p_node)
    {
        if (NULL != del_f)
        {
            del_f(p_node->p_data);
            p_node->p_data = NULL;
            p_node->p_next = NULL;
        }

        free(p_node);
        p_node = NULL;
    }

    return;
}

/**
 * @brief Creates and initializes a new node for the circular linked list.
 *
 * This function allocates memory for a new node in the circular linked list and
 * initializes its members. The node's data pointer is set to the provided data,
 * and the next pointer is set to NULL. The function returns a pointer to the
 * newly created node, or NULL if memory allocation fails.
 *
 * @param p_data Pointer to the data to be stored in the new node.
 *
 * @return circular_linked_list_node_t* Pointer to the newly created node, or NULL
 * if memory allocation fails.
 *
 * @note The provided data pointer `p_data` is directly assigned to the node
 * without copying or validation. Ensure that the data pointer is valid and
 * properly managed by the caller.
 */
static circular_linked_list_node_t *
circular_linked_list_create_node (void *p_data)
{
    circular_linked_list_node_t *p_node = NULL;
    p_node = calloc(1, sizeof(circular_linked_list_node_t));

    if (NULL != p_node)
    {
        p_node->p_data = p_data;
        p_node->p_next = p_node; // point to self by default
    }

    return p_node;
}
