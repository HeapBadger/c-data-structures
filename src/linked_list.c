/**
 * @file linked_list.c
 * @brief Implementation of the singly linked list data structure.
 *
 * This implementation provides a generic singly linked list with dynamic memory
 * management. Each node stores a pointer to user data, allowing the list to
 * manage elements of any type. Core features include insertion, deletion,
 * searching, traversal, and cloning. Function pointers are used for
 * user-defined data operations such as copy, delete, print, and compare.
 *
 * The list grows dynamically and supports efficient O(1) insertion at the head
 * and O(n) access for arbitrary positions. The design prioritizes flexibility
 * and modularity through encapsulation and callback-based customization.
 *
 * @note Elements are not owned by the list until they are successfully
 * inserted. If insertion fails (e.g., due to a memory allocation error), the
 * caller remains responsible for freeing the associated memory.
 *
 * @author heapbadger
 */

#include <stdio.h>
#include <stdlib.h>
#include "auxiliary.h"
#include "linked_list.h"

/**
 * @brief Creates a new node.
 *
 * @param p_data Pointer to the data for the new node.
 *
 * @return Pointer to a new linked list node, or NULL on failure.
 */
static ll_node_t *ll_create_node(void *p_data);

/**
 * @brief Deletes a node.
 *
 * @param p_list Pointer to the list.
 * @param p_node Pointer to the node to be deleted.
 */
static void ll_del_node(ll_t *p_list, ll_node_t *p_node);

/**
 * @brief   Search for a key in the list, returning its index and/or node.
 *
 * @param   p_list     Pointer to the list.
 * @param   p_key      Pointer to the key to search for.
 * @param   p_out_idx  Output pointer to store the found index (or NULL).
 * @param   p_out_node Output pointer to store the found node (or NULL).
 *
 * @return LL_SUCCESS on success, appropriate error code otherwise.
 */
static ll_error_code_t ll_find_internal(const ll_t *p_list,
                                        void       *p_key,
                                        size_t     *p_out_idx,
                                        ll_node_t **p_out_node);

ll_t *
ll_create (const del_func   del_f,
           const cmp_func   cmp_f,
           const print_func print_f,
           const copy_func  cpy_f)
{
    ll_t *p_list = NULL;

    if ((NULL == del_f) || (NULL == cmp_f) || (NULL == print_f)
        || (NULL == cpy_f))
    {
        return p_list;
    }

    p_list = (ll_t *)calloc(1U, sizeof(ll_t));

    if (NULL == p_list)
    {
        return p_list;
    }

    p_list->p_head  = NULL;
    p_list->del_f   = del_f;
    p_list->cmp_f   = cmp_f;
    p_list->print_f = print_f;
    p_list->cpy_f   = cpy_f;
    return p_list;
}

void
ll_destroy (ll_t *p_list)
{
    if (NULL != p_list)
    {
        ll_clear(p_list);
        p_list->p_head  = NULL;
        p_list->del_f   = NULL;
        p_list->cmp_f   = NULL;
        p_list->print_f = NULL;
        p_list->cpy_f   = NULL;
        free(p_list);
    }
}

void
ll_clear (ll_t *p_list)
{
    if ((NULL != p_list) && (NULL != p_list->del_f))
    {
        ll_node_t *p_curr = p_list->p_head;

        while (NULL != p_curr)
        {
            ll_node_t *p_next = p_curr->p_next;
            ll_del_node(p_list, p_curr);
            p_curr = p_next;
        }

        p_list->p_head = NULL;
    }
}

void
ll_del_ele (ll_t *p_list, void *p_value)
{
    if ((NULL != p_list) && (NULL != p_value))
    {
        p_list->del_f(p_value);
    }
}

ll_error_code_t
ll_append (ll_t *p_list, void *p_data)
{
    if ((NULL == p_list) || (NULL == p_data))
    {
        return LL_INVALID_ARGUMENT;
    }

    ll_node_t *p_new = ll_create_node(p_data);

    if (NULL == p_new)
    {
        return LL_ALLOCATION_FAILURE;
    }

    // Append to empty list
    if (NULL == p_list->p_head)
    {
        p_list->p_head = p_new;
        return LL_SUCCESS;
    }

    // Append at tail
    ll_node_t *p_tail = ll_tail(p_list);
    p_tail->p_next    = p_new;
    return LL_SUCCESS;
}

ll_error_code_t
ll_insert (ll_t *p_list, void *p_data, size_t index)
{
    if ((NULL == p_list) || (NULL == p_data))
    {
        return LL_INVALID_ARGUMENT;
    }

    ll_node_t *p_new = ll_create_node(p_data);

    if (NULL == p_new)
    {
        return LL_ALLOCATION_FAILURE;
    }

    // Insert at head
    if (index == 0)
    {
        p_new->p_next  = p_list->p_head;
        p_list->p_head = p_new;
    }
    // Insert at index
    else
    {
        ll_node_t *p_prev = ll_at(p_list, index - 1U);

        if (NULL == p_prev)
        {
            ll_del_node(p_list, p_new);
            return LL_OUT_OF_BOUNDS;
        }

        p_new->p_next  = p_prev->p_next;
        p_prev->p_next = p_new;
    }

    return LL_SUCCESS;
}

ll_error_code_t
ll_del_at (ll_t *p_list, size_t index)
{
    ll_error_code_t ret = LL_SUCCESS;

    if ((NULL == p_list) || (NULL == p_list->del_f))
    {
        ret = LL_INVALID_ARGUMENT;
        goto EXIT;
    }

    if (ll_is_empty(p_list))
    {
        ret = LL_EMPTY;
        goto EXIT;
    }

    if (0U == index)
    {
        ll_node_t *p_tmp = p_list->p_head;
        p_list->p_head   = p_tmp->p_next;
        ll_del_node(p_list, p_tmp);
        goto EXIT;
    }

    ll_node_t *p_prev = ll_at(p_list, index - 1U);

    if ((NULL == p_prev) || (NULL == p_prev->p_next))
    {
        ret = LL_OUT_OF_BOUNDS;
        goto EXIT;
    }

    ll_node_t *p_tmp = p_prev->p_next;
    p_prev->p_next   = p_tmp->p_next;
    ll_del_node(p_list, p_tmp);

EXIT:
    return ret;
}

ll_node_t *
ll_at (const ll_t *p_list, size_t index)
{
    if (NULL == p_list)
    {
        return NULL;
    }

    ll_node_t *p_curr = p_list->p_head;

    for (size_t i = 0U; (NULL != p_curr) && (i < index); ++i)
    {
        p_curr = p_curr->p_next;
    }

    return p_curr;
}

ll_node_t *
ll_head (const ll_t *p_list)
{
    return (p_list != NULL) ? p_list->p_head : NULL;
}

ll_node_t *
ll_tail (const ll_t *p_list)
{
    if (NULL == p_list)
    {
        return NULL;
    }

    ll_node_t *p_curr = p_list->p_head;

    if (NULL == p_curr)
    {
        return NULL;
    }

    while (p_curr->p_next != NULL)
    {
        p_curr = p_curr->p_next;
    }

    return p_curr;
}

ll_error_code_t
ll_find (const ll_t *p_list, void *p_key, size_t *p_idx)
{
    return ll_find_internal(p_list, p_key, p_idx, NULL);
}

bool
ll_is_empty (const ll_t *p_list)
{
    if (NULL == p_list)
    {
        return true;
    }

    return (NULL == p_list->p_head);
}

void
ll_print (const ll_t *p_list)
{
    if ((NULL != p_list) && (NULL != p_list->print_f))
    {
        size_t idx = 0U;
        printf("[");

        for (ll_node_t *p_curr = p_list->p_head; NULL != p_curr;
             p_curr            = p_curr->p_next)
        {
            if (idx > 0U)
            {
                printf(" -> ");
            }

            p_list->print_f(p_curr->p_data, idx);
            ++idx;
        }

        printf("]\n");
    }
}

ll_error_code_t
ll_size (const ll_t *p_list, size_t *p_size)
{
    if ((NULL == p_list) || (NULL == p_size))
    {
        return LL_INVALID_ARGUMENT;
    }

    *p_size           = 0U;
    ll_node_t *p_curr = p_list->p_head;

    while (NULL != p_curr)
    {
        (*p_size)++;
        p_curr = p_curr->p_next;
    }

    return LL_SUCCESS;
}

ll_error_code_t
ll_clone_at (const ll_t *p_list, size_t index, void **p_copy)
{
    if ((NULL == p_list) || (NULL == p_copy) || (NULL == p_list->cpy_f))
    {
        return LL_INVALID_ARGUMENT;
    }

    ll_node_t *p_node = ll_at(p_list, index);

    if (NULL == p_node)
    {
        return LL_EMPTY;
    }

    *p_copy = p_list->cpy_f(p_node->p_data);

    if (NULL == *p_copy)
    {
        return LL_ALLOCATION_FAILURE;
    }

    return LL_SUCCESS;
}

ll_t *
ll_clone (const ll_t *p_ori)
{
    ll_node_t *p_prev = NULL;
    ll_t      *p_new  = NULL;

    if ((NULL == p_ori) || (NULL == p_ori->cpy_f))
    {
        goto EXIT;
    }

    p_new = (ll_t *)calloc(1U, sizeof(ll_t));

    if (NULL == p_new)
    {
        goto EXIT;
    }

    p_new->p_head  = NULL;
    p_new->del_f   = p_ori->del_f;
    p_new->cmp_f   = p_ori->cmp_f;
    p_new->print_f = p_ori->print_f;
    p_new->cpy_f   = p_ori->cpy_f;

    for (ll_node_t *p_curr = p_ori->p_head; NULL != p_curr;
         p_curr            = p_curr->p_next)
    {
        void *p_copy = p_ori->cpy_f(p_curr->p_data);

        if (NULL == p_copy)
        {
            goto CLEANUP;
        }

        ll_node_t *p_node = ll_create_node(p_copy);

        if (NULL == p_node)
        {
            p_new->del_f(p_copy);
            goto CLEANUP;
        }

        if (NULL == p_prev)
        {
            p_new->p_head = p_node;
        }
        else
        {
            p_prev->p_next = p_node;
        }

        p_prev = p_node;
    }

    goto EXIT;

CLEANUP:
    ll_destroy(p_new);
    p_new = NULL;

EXIT:
    return p_new;
}

ll_error_code_t
ll_reverse (ll_t *p_list)
{
    if (NULL == p_list)
    {
        return LL_INVALID_ARGUMENT;
    }

    if (ll_is_empty(p_list))
    {
        return LL_EMPTY;
    }

    ll_node_t *p_prev = NULL;
    ll_node_t *p_curr = p_list->p_head;

    while (NULL != p_curr)
    {
        ll_node_t *p_next = p_curr->p_next;
        p_curr->p_next    = p_prev;
        p_prev            = p_curr;
        p_curr            = p_next;
    }

    p_list->p_head = p_prev;
    return LL_SUCCESS;
}

ll_error_code_t
ll_foreach (ll_t *p_list, foreach_func func)
{
    if ((NULL == p_list) || (NULL == func))
    {
        return LL_INVALID_ARGUMENT;
    }

    if (ll_is_empty(p_list))
    {
        return LL_EMPTY;
    }

    ll_node_t *p_curr = p_list->p_head;
    size_t     idx    = 0U;

    while (NULL != p_curr)
    {
        func(p_curr->p_data, idx);
        p_curr = p_curr->p_next;
        ++idx;
    }

    return LL_SUCCESS;
}

static ll_node_t *
ll_create_node (void *p_data)
{
    ll_node_t *p_node = (ll_node_t *)calloc(1U, sizeof(ll_node_t));

    if (NULL != p_node)
    {
        p_node->p_data = p_data;
        p_node->p_next = NULL;
    }

    return p_node;
}

static void
ll_del_node (ll_t *p_list, ll_node_t *p_node)
{
    if ((NULL != p_list) && (NULL != p_node))
    {
        ll_del_ele(p_list, p_node->p_data);
        free(p_node);
    }
}

static ll_error_code_t
ll_find_internal (const ll_t *p_list,
                  void       *p_key,
                  size_t     *p_out_idx,
                  ll_node_t **p_out_node)
{
    if ((NULL == p_list) || (NULL == p_key) || (NULL == p_list->cmp_f))
    {
        return LL_INVALID_ARGUMENT;
    }

    if (ll_is_empty(p_list))
    {
        return LL_EMPTY;
    }

    size_t     idx    = 0U;
    ll_node_t *p_curr = p_list->p_head;

    while (NULL != p_curr)
    {
        if (0 == p_list->cmp_f(p_key, p_curr->p_data))
        {
            if (NULL != p_out_idx)
            {
                *p_out_idx = idx;
            }

            if (NULL != p_out_node)
            {
                *p_out_node = p_curr;
            }

            return LL_SUCCESS;
        }

        p_curr = p_curr->p_next;
        idx++;
    }

    return LL_NOT_FOUND;
}

/*** end of file ***/
