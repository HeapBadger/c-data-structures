/**
 * @file linked_list.c
 * @brief Implementation of the singly Linked List data structure.
 *
 * @author heapbadger
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "auxiliary.h"
#include "linked_list.h"

static void       ll_del_node(ll_node_t *p_node, const del_func del_f);
static ll_node_t *ll_create_node(void *p_data);

ll_t *
ll_create (const del_func del_f, const cmp_func cmp_f, const print_func print_f)
{
    if ((NULL == del_f) || (NULL == cmp_f) || (NULL == print_f))
    {
        return NULL;
    }

    ll_t *p_list = (ll_t *)calloc(1U, sizeof(ll_t));

    if (NULL == p_list)
    {
        return NULL;
    }

    p_list->p_head  = NULL;
    p_list->del_f   = del_f;
    p_list->cmp_f   = cmp_f;
    p_list->print_f = print_f;

    return p_list;
}

void
ll_destroy (ll_t *p_list)
{
    if (NULL == p_list)
    {
        return;
    }

    ll_clear(p_list);
    free(p_list);
}

ssize_t
ll_clear (ll_t *p_list)
{
    if ((NULL == p_list) || (NULL == p_list->del_f))
    {
        return LL_INVALID_ARGUMENT;
    }

    ll_node_t *p_curr = p_list->p_head;

    while (NULL != p_curr)
    {
        ll_node_t *p_next = p_curr->p_next;
        ll_del_node(p_curr, p_list->del_f);
        p_curr = p_next;
    }

    p_list->p_head = NULL;
    return LL_SUCCESS;
}

ssize_t
ll_prepend (ll_t *p_list, void *p_data)
{
    return ll_insert(p_list, p_data, 0U);
}

ssize_t
ll_append (ll_t *p_list, void *p_data)
{
    return ll_insert(p_list, p_data, ll_size(p_list));
}

ssize_t
ll_insert (ll_t *p_list, void *p_data, size_t index)
{
    ssize_t ret = LL_SUCCESS;

    if ((NULL == p_list) || (NULL == p_list->del_f) || (NULL == p_data))
    {
        ret = LL_INVALID_ARGUMENT;
        goto EXIT;
    }

    ssize_t size = ll_size(p_list);

    if ((0 > size) || (index > (size_t)size))
    {
        p_list->del_f(p_data);
        ret = LL_OUT_OF_BOUNDS;
        goto EXIT;
    }

    ll_node_t *p_new_node = ll_create_node(p_data);

    if (NULL == p_new_node)
    {
        ret = LL_ALLOCATION_FAILURE;
        goto EXIT;
    }

    if (0U == index)
    {
        p_new_node->p_next = p_list->p_head;
        p_list->p_head     = p_new_node;
        goto EXIT;
    }

    ll_node_t *p_curr = p_list->p_head;

    for (size_t i = 0U; i < index - 1U; ++i)
    {
        p_curr = p_curr->p_next;
    }

    p_new_node->p_next = p_curr->p_next;
    p_curr->p_next     = p_new_node;

EXIT:
    return ret;
}

ssize_t
ll_del_at (ll_t *p_list, size_t index)
{
    ssize_t ret = LL_SUCCESS;

    if ((NULL == p_list) || (NULL == p_list->del_f))
    {
        ret = LL_INVALID_ARGUMENT;
        goto EXIT;
    }

    if (ll_is_empty(p_list))
    {
        ret = LL_OUT_OF_BOUNDS;
        goto EXIT;
    }

    if (0U == index)
    {
        ll_node_t *p_to_delete = p_list->p_head;
        p_list->p_head         = p_to_delete->p_next;
        ll_del_node(p_to_delete, p_list->del_f);
        goto EXIT;
    }

    ll_node_t *p_curr = p_list->p_head;

    for (size_t i = 0U; i < (index - 1U); i++)
    {
        if (NULL == p_curr->p_next)
        {
            ret = LL_OUT_OF_BOUNDS;
            goto EXIT;
        }
        p_curr = p_curr->p_next;
    }

    ll_node_t *p_to_delete = p_curr->p_next;

    if (NULL == p_to_delete)
    {
        ret = LL_OUT_OF_BOUNDS;
        goto EXIT;
    }

    p_curr->p_next = p_to_delete->p_next;
    ll_del_node(p_to_delete, p_list->del_f);

EXIT:
    return ret;
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

ll_node_t *
ll_at (const ll_t *p_list, size_t index)
{
    if (NULL == p_list)
    {
        return NULL;
    }

    ll_node_t *p_curr = p_list->p_head;
    size_t     count  = 0U;

    while (NULL != p_curr)
    {
        if (count == index)
        {
            return p_curr;
        }

        p_curr = p_curr->p_next;
        count++;
    }

    return NULL;
}

ssize_t
ll_find (const ll_t *p_list, void *p_data)
{
    if ((NULL == p_list) || (NULL == p_data))
    {
        return LL_INVALID_ARGUMENT;
    }

    ll_node_t *p_curr = p_list->p_head;
    size_t     index  = 0U;

    while (NULL != p_curr)
    {
        if (0 == p_list->cmp_f(p_data, p_curr->p_data))
        {
            return (ssize_t)index;
        }

        p_curr = p_curr->p_next;
        index++;
    }

    return LL_NOT_FOUND;
}

ssize_t
ll_size (const ll_t *p_list)
{
    if (NULL == p_list)
    {
        return LL_INVALID_ARGUMENT;
    }

    size_t     count  = 0U;
    ll_node_t *p_curr = p_list->p_head;

    while (NULL != p_curr)
    {
        count++;
        p_curr = p_curr->p_next;
    }

    return (ssize_t)count;
}

ssize_t
ll_reverse (ll_t *p_list)
{
    if ((NULL == p_list) || (NULL == p_list->p_head))
    {
        return LL_INVALID_ARGUMENT;
    }

    ll_node_t *p_prev = NULL;
    ll_node_t *p_curr = p_list->p_head;
    ll_node_t *p_next = NULL;

    while (NULL != p_curr)
    {
        p_next         = p_curr->p_next;
        p_curr->p_next = p_prev;
        p_prev         = p_curr;
        p_curr         = p_next;
    }

    p_list->p_head = p_prev;
    return LL_SUCCESS;
}

ssize_t
ll_foreach (ll_t *p_list, foreach_func func)
{
    if ((NULL == p_list) || (NULL == func))
    {
        return LL_INVALID_ARGUMENT;
    }

    ll_node_t *p_curr = p_list->p_head;
    size_t     index  = 0;

    while (p_curr != NULL)
    {
        func(p_curr->p_data, index);
        p_curr = p_curr->p_next;
        index++;
    }

    return LL_SUCCESS;
}

ssize_t
ll_swap (ll_t *p_list, size_t index_1, size_t index_2)
{
    if (NULL == p_list)
    {
        return LL_INVALID_ARGUMENT;
    }

    if (index_1 == index_2)
    {
        return LL_SUCCESS;
    }

    ll_node_t *p_node1 = ll_at(p_list, index_1);
    ll_node_t *p_node2 = ll_at(p_list, index_2);

    if ((NULL == p_node1) || (NULL == p_node2))
    {
        return LL_OUT_OF_BOUNDS;
    }

    void *p_tmp     = p_node1->p_data;
    p_node1->p_data = p_node2->p_data;
    p_node2->p_data = p_tmp;

    return LL_SUCCESS;
}

ssize_t
ll_update (ll_t *p_list, size_t index, void *p_data)
{
    if ((NULL == p_list) || (NULL == p_data))
    {
        return LL_INVALID_ARGUMENT;
    }

    ll_node_t *p_node = ll_at(p_list, index);

    if (NULL == p_node)
    {
        return LL_OUT_OF_BOUNDS;
    }

    p_list->del_f(p_node->p_data);
    p_node->p_data = p_data;

    return LL_SUCCESS;
}

ll_t *
ll_clone(const ll_t *p_ori, copy_func cpy_f)
{
    ll_t *p_new = NULL;

    if ((NULL == p_ori) || (NULL == cpy_f))
    {
        goto EXIT;
    }

    p_new = ll_create(p_ori->del_f, p_ori->cmp_f, p_ori->print_f);

    if (NULL == p_new)
    {
        goto EXIT;
    }

    ll_node_t *p_curr = p_ori->p_head;

    while (p_curr != NULL)
    {
        void *p_copied_data = cpy_f(p_curr->p_data);

        if (p_copied_data == NULL)
        {
            ll_destroy(p_new);
            p_new = NULL;
            goto EXIT;
        }

        if (LL_SUCCESS != ll_append(p_new, p_copied_data))
        {
            p_new->del_f(p_copied_data);
            ll_destroy(p_new);
            p_new = NULL;
            goto EXIT;
        }

        p_curr = p_curr->p_next;
    }

EXIT:
    return p_new;
}

static void
ll_del_node (ll_node_t *p_node, const del_func del_f)
{
    if (NULL == p_node)
    {
        return;
    }

    if (NULL != del_f)
    {
        del_f(p_node->p_data);
    }

    free(p_node);
}

static ll_node_t *
ll_create_node (void *p_data)
{
    ll_node_t *p_node = (ll_node_t *)calloc(1U, sizeof(ll_node_t));

    if (NULL == p_node)
    {
        return NULL;
    }

    p_node->p_data = p_data;
    p_node->p_next = NULL;
    return p_node;
}

/*** end of file ***/