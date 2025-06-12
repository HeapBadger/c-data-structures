/**
 * @file doubly_linked_list.c
 * @brief Implementation of the doubly linked list data structure.
 *
 * @author heapbadger
 */

#include <stdio.h>
#include <stdlib.h>
#include "doubly_linked_list.h"

static void       dl_del_node(dl_node_t *p_node, const del_func del_f);
static dl_node_t *dl_create_node(void *p_data);

dl_t *
dl_create (const del_func del_f, const cmp_func cmp_f, const print_func print_f)
{
    dl_t *p_list = NULL;

    if ((NULL != del_f) && (NULL != cmp_f) && (NULL != print_f))
    {
        p_list = calloc(1, sizeof(dl_t));

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
dl_destroy (dl_t *p_list)
{
    if (NULL != p_list)
    {
        dl_clear(p_list);
        p_list->cmp_f   = NULL;
        p_list->del_f   = NULL;
        p_list->print_f = NULL;
        free(p_list);
    }

    return;
}

ssize_t
dl_clear (dl_t *p_list)
{
    if ((NULL == p_list) || (NULL == p_list->del_f))
    {
        return DL_INVALID_ARGUMENT;
    }

    dl_node_t *p_curr = p_list->p_head;

    while (NULL != p_curr)
    {
        dl_node_t *p_next = p_curr->p_next;
        dl_del_node(p_curr, p_list->del_f);
        p_curr = p_next;
    }

    p_list->p_head = NULL;
    return DL_SUCCESS;
}

ssize_t
dl_preappend (dl_t *p_list, void *p_data)
{
    return dl_insert(p_list, p_data, 0);
}

ssize_t
dl_insert (dl_t *p_list, void *p_data, size_t index)
{
    ssize_t result = DL_SUCCESS;

    if ((NULL == p_list) || (NULL == p_list->del_f) || (NULL == p_data))
    {
        result = DL_INVALID_ARGUMENT;
        goto EXIT;
    }

    // create new node
    dl_node_t *p_new_node = NULL;
    p_new_node            = dl_create_node(p_data);

    if (NULL == p_new_node)
    {
        result = DL_ALLOCATION_FAILURE;
        goto EXIT;
    }

    // handle preappend operations
    if (0 == index)
    {
        p_new_node->p_next = p_list->p_head;

        if (p_list->p_head != NULL)
        {
            p_list->p_head->p_prev = p_new_node;
        }

        p_list->p_head = p_new_node;
    }
    else
    {
        size_t     count  = 0;
        dl_node_t *p_curr = p_list->p_head;
        dl_node_t *p_prev = NULL;

        while ((NULL != p_curr) && (count < index))
        {
            p_prev = p_curr;
            p_curr = p_curr->p_next;
            count++;
        }

        if (count == index)
        {
            p_new_node->p_next = p_curr;
            p_new_node->p_prev = p_prev;

            if (p_curr != NULL)
            {
                p_curr->p_prev = p_new_node;
            }

            p_prev->p_next = p_new_node;
        }
        else
        {
            result = DL_OUT_OF_BOUNDS;
            dl_del_node(p_new_node, p_list->del_f);
        }
    }

EXIT:
    return result;
}

ssize_t
dl_del_at (dl_t *p_list, size_t index)
{
    if ((NULL == p_list) || (NULL == p_list->del_f))
    {
        return DL_INVALID_ARGUMENT;
    }

    if (dl_is_empty(p_list))
    {
        return DL_OUT_OF_BOUNDS;
    }

    dl_node_t *p_to_delete = NULL;

    // Deleting the head node
    if (0 == index)
    {
        p_to_delete    = p_list->p_head;
        p_list->p_head = p_to_delete->p_next;

        if (p_list->p_head != NULL)
        {
            p_list->p_head->p_prev = NULL;
        }

        dl_del_node(p_to_delete, p_list->del_f);
        return DL_SUCCESS;
    }

    // Traverse to the node at index
    dl_node_t *p_curr = p_list->p_head;

    for (size_t i = 0U; (i < index) && (p_curr != NULL); ++i)
    {
        p_curr = p_curr->p_next;
    }

    if (NULL == p_curr)
    {
        return DL_OUT_OF_BOUNDS;
    }

    // Remove p_curr from the list
    if (p_curr->p_prev != NULL)
    {
        p_curr->p_prev->p_next = p_curr->p_next;
    }

    if (p_curr->p_next != NULL)
    {
        p_curr->p_next->p_prev = p_curr->p_prev;
    }

    dl_del_node(p_curr, p_list->del_f);
    return DL_SUCCESS;
}

bool
dl_is_empty (const dl_t *p_list)
{
    if (NULL == p_list)
    {
        return true;
    }

    return (NULL == p_list->p_head);
}

dl_node_t *
dl_at (const dl_t *p_list, size_t index)
{
    dl_node_t *p_current = NULL;
    dl_node_t *p_node    = NULL;

    if ((NULL != p_list) && (NULL != p_list->p_head))
    {
        size_t count = 0;
        p_current    = p_list->p_head;

        while (NULL != p_current)
        {
            if (count == index)
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

ssize_t
dl_find (const dl_t *p_list, void *p_data)
{
    ssize_t index = DL_NOT_FOUND;
    size_t  count = 0;

    if ((NULL != p_list) && (NULL != p_data) && (NULL != p_list->p_head))
    {
        dl_node_t *p_current = p_list->p_head;

        while (NULL != p_current)
        {
            if (0 == p_list->cmp_f(p_data, p_current->p_data))
            {
                index = (ssize_t)count;
                break;
            }

            count++;
            p_current = p_current->p_next;
        }
    }
    else
    {
        index = DL_INVALID_ARGUMENT;
    }

    return index;
}

ssize_t
dl_size (const dl_t *p_list)
{
    ssize_t size = 0;

    if (NULL != p_list)
    {
        dl_node_t *p_current = p_list->p_head;

        while (NULL != p_current)
        {
            size++;
            p_current = p_current->p_next;
        }
    }
    else
    {
        size = DL_INVALID_ARGUMENT;
    }

    return size;
}

ssize_t
dl_reverse (dl_t *p_list)
{
    ssize_t result = DL_INVALID_ARGUMENT;

    if ((NULL != p_list) && (NULL != p_list->p_head))
    {
        dl_node_t *p_current = p_list->p_head;
        dl_node_t *p_next    = NULL;
        dl_node_t *p_prev    = NULL;

        while (NULL != p_current)
        {
            p_next            = p_current->p_next;
            p_current->p_next = p_prev;
            p_current->p_prev = p_next;
            p_prev            = p_current;
            p_current         = p_next;
        }

        p_list->p_head = p_prev;
        result         = DL_SUCCESS;
    }

    return result;
}

void
dl_print (const dl_t *p_list)
{
    if ((NULL != p_list) && (NULL != p_list->print_f)
        && (NULL != p_list->p_head))
    {
        printf("\nLinked List: ");
        dl_node_t *p_current = p_list->p_head;

        while (NULL != p_current)
        {
            p_list->print_f(p_current->p_data);
            p_current = p_current->p_next;
        }

        printf("\n\n");
    }

    return;
}

ssize_t
dl_swap (dl_t *p_list, size_t index_1, size_t index_2)
{
    ssize_t result = DL_OUT_OF_BOUNDS;

    if (NULL == p_list)
    {
        result = DL_INVALID_ARGUMENT;
        goto EXIT;
    }

    dl_node_t *p_node_1 = dl_at(p_list, index_1);
    dl_node_t *p_node_2 = dl_at(p_list, index_2);

    if ((NULL != p_node_1) && (NULL != p_node_2))
    {
        void *p_data_1   = p_node_1->p_data;
        void *p_data_2   = p_node_2->p_data;
        p_node_1->p_data = p_data_2;
        p_node_2->p_data = p_data_1;
        result           = DL_SUCCESS;
    }

EXIT:
    return result;
}

ssize_t
dl_update (dl_t *p_list, size_t index, void *p_data)
{
    ssize_t result = DL_OUT_OF_BOUNDS;

    if ((NULL == p_list) || (NULL == p_data))
    {
        result = DL_INVALID_ARGUMENT;
        goto EXIT;
    }

    dl_node_t *p_node = dl_at(p_list, index);

    if (NULL != p_node)
    {
        p_list->del_f(p_node->p_data);
        p_node->p_data = p_data;
        result         = DL_SUCCESS;
    }

EXIT:
    return result;
}

static void
dl_del_node (dl_node_t *p_node, const del_func del_f)
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

static dl_node_t *
dl_create_node (void *p_data)
{
    dl_node_t *p_node = NULL;
    p_node            = calloc(1, sizeof(dl_node_t));

    if (NULL != p_node)
    {
        p_node->p_data = p_data;
        p_node->p_next = NULL;
        p_node->p_prev = NULL;
    }

    return p_node;
}

/*** end of file ***/