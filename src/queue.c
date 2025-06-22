/**
 * @file queue.c
 * @brief Implementation of a queue (FIFO) data structure using a singly linked
 * list.
 *
 * This queue is backed by a singly linked list, enabling efficient O(1) enqueue
 * and dequeue operations without the need for shifting elements, as required in
 * array-based queues. The structure dynamically grows to accommodate new items,
 * with memory managed node-by-node.
 *
 * Linked list queues are ideal for unbounded or unpredictable workloads, as
 * they avoid reallocations and maintain consistent performance. They are
 * well-suited for systems where memory usage patterns are dynamic or unknown in
 * advance.
 *
 * @note The queue only takes ownership of an element upon successful insertion.
 *       If insertion fails, the caller must manage (and eventually free) the
 *       memory.
 * 
 * @author  heapbadger
 */

#include <stdlib.h>
#include "auxiliary.h"
#include "queue.h"

/**
 * @brief Convert a ll module return code to a queue module error code.
 *
 * @param ret Linked list error code.
 *
 * @return Corresponding queue_error_code_t.
 */
static queue_error_code_t queue_error_from_ll(ll_error_code_t ret);

queue_t *
queue_create (const del_func   del_f,
              const cmp_func   cmp_f,
              const print_func print_f,
              const copy_func  cpy_f)
{
    queue_t *p_queue = (queue_t *)calloc(1U, sizeof(queue_t));

    if (NULL != p_queue)
    {
        p_queue->p_queue = ll_create(del_f, cmp_f, print_f, cpy_f);

        if (NULL == p_queue->p_queue)
        {
            queue_destroy(p_queue);
            p_queue = NULL;
        }
    }

    return p_queue;
}

void
queue_destroy (queue_t *p_queue)
{
    if (NULL != p_queue)
    {
        ll_destroy(p_queue->p_queue);
        free(p_queue);
    }
}

void 
queue_del_ele(queue_t *p_queue, void *p_data)
{
    if ((NULL != p_queue) && (NULL != p_data))
    {
        ll_del_ele(p_queue->p_queue, p_data);
    }
}

queue_error_code_t
queue_enqueue (queue_t *p_queue, void *p_data)
{
    if ((NULL != p_queue) && (NULL != p_data))
    {
        return queue_error_from_ll(ll_append(p_queue->p_queue, p_data));
    }

    return QUEUE_INVALID_ARGUMENT;
}

queue_error_code_t
queue_dequeue (queue_t *p_queue, void **p_data)
{
    if ((NULL == p_queue) || (NULL == p_data))
    {
        return QUEUE_INVALID_ARGUMENT;
    }

    queue_error_code_t err
        = queue_error_from_ll(ll_clone_at(p_queue->p_queue, 0, p_data));

    if (QUEUE_SUCCESS != err)
    {
        return err;
    }

    return queue_error_from_ll(ll_del_at(p_queue->p_queue, 0U));
}

queue_error_code_t
queue_peek (const queue_t *p_queue, void **p_top)
{
    if ((NULL != p_queue) && (p_queue->p_queue) && (NULL != p_top))
    {
        ll_node_t *p_node = ll_head(p_queue->p_queue);

        if (NULL == p_node)
        {
            return QUEUE_NOT_FOUND;
        }

        *p_top = p_node->p_data;
        return QUEUE_SUCCESS;
    }

    return QUEUE_INVALID_ARGUMENT;
}

bool
queue_is_empty (const queue_t *p_queue)
{
    if (NULL != p_queue)
    {
        return ll_is_empty(p_queue->p_queue);
    }

    return false;
}

void
queue_print (const queue_t *p_queue)
{
    if (NULL != p_queue)
    {
        ll_print(p_queue->p_queue);
    }
}

queue_error_code_t
queue_size (const queue_t *p_queue, size_t *p_size)
{
    if ((NULL != p_queue) && (NULL != p_size))
    {
        return queue_error_from_ll(ll_size(p_queue->p_queue, p_size));
    }

    return QUEUE_INVALID_ARGUMENT;
}

queue_t *
queue_clone (const queue_t *p_ori)
{
    queue_t *p_new = NULL;

    if (NULL != p_ori)
    {
        p_new = (queue_t *)calloc(1U, sizeof(queue_t));

        if (NULL == p_new)
        {
            return p_new;
        }

        p_new->p_queue = ll_clone(p_ori->p_queue);
    }

    return p_new;
}

static queue_error_code_t
queue_error_from_ll (ll_error_code_t ret)
{
    switch (ret)
    {
        case LL_SUCCESS:
            return QUEUE_SUCCESS;
        case LL_NOT_FOUND:
            return QUEUE_NOT_FOUND;
        case LL_OUT_OF_BOUNDS:
            return QUEUE_OUT_OF_BOUNDS;
        case LL_INVALID_ARGUMENT:
            return QUEUE_INVALID_ARGUMENT;
        case LL_ALLOCATION_FAILURE:
            return QUEUE_ALLOCATION_FAILURE;
        case LL_EMPTY:
            return QUEUE_EMPTY;
        default:
            return QUEUE_FAILURE;
    }
}

/*** end of file ***/
