#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include "commu_queue.h"

/** 
 * queue node structure
 */
struct _node_t
{
    const void *item;
    struct _node_t *next;
};

/** 
 * list block queue
 * 
 * blocking queue based on linked list
 */
struct lb_queue_t
{

    /** 
     * Returns the number of elements in this collection.
     *
     * @param thiz this
     * @return the number of elements in this collection
     */
    uint32_t (*size)(struct lb_queue_t *const thiz);

    /**
     * Removes all of the elements from this collection.
     *
     *  @param thiz this
     */
    void (*clear)(struct lb_queue_t *const thiz);

    /**
     * Free collection
     *
     * @param thiz this
     */
    void (*free)(struct lb_queue_t *const thiz);

    /** 
     * Inserts the specified element into this queue if it is possible to do
     * so immediately without violating capacity restrictions.
     *
     * @param thiz this
     * @param element element
     * @return true if the element was added to this queue, else false
     */
    bool (*offer)(struct lb_queue_t *const thiz, const void *const element);

    /** 
     * Retrieves and removes the head of this queue,
     * or returns NULL if this queue is empty.
     *
     * @param thiz this
     * @return the head of this queue, or NULL if this queue is empty 
     */
    void *(*poll)(struct lb_queue_t *const thiz);

    /** 
     * Retrieves, but does not remove, the head of this queue,
     * or returns NULL if this queue is empty.
     *
     * @param thiz this
     * @return the head of this queue, or NULL if this queue is empty
     */
    void *(*peek)(struct lb_queue_t *const thiz);

    /**
     * Inserts the specified element into this queue, waiting if necessary for space to become available.
     *
     * @param thiz this
     * @param element the element to add
     */
    bool (*put)(struct lb_queue_t *const thiz, const void *const element);

    /** 
     * Inserts the specified element into this queue, waiting up to the
     * specified wait time if necessary for space to become available.
     *
     * @param thiz this
     * @param element the element to add
     * @param timeout how long to wait before giving up, in units of unit
     * @param unit a time_unit_t determining how to interpret the timeout parameter
     * @return true if successful, or false if the specified waiting time elapses before space is available
     */
    bool (*offer_await)(struct lb_queue_t *const thiz, const void *const element, const uint64_t timeout, const struct time_unit_t *const unit);

    /** 
     * Retrieves and removes the head of this queue, waiting if necessary until an element becomes available.
     *
     * @param thiz this
     * @return the head of this queue
     */
    void *(*take)(struct lb_queue_t *const thiz);

    /** 
     * Retrieves and removes the head of this queue, waiting up to the
     * specified wait time if necessary for an element to become available.
     *
     * @param thiz this
     * @param timeout how long to wait before giving up, in units of unit
     * @param unit a time_unit_t determining how to interpret the timeout parameter
     * @return the head of this queue, or NULL if the specified waiting time elapses before an element is available
     */
    void *(*poll_await)(struct lb_queue_t *const thiz, const uint64_t timeout, const struct time_unit_t *const unit);

    /* queue capacity */
    uint32_t _capacity;

    /* Number of queue elements */
    __sig_atomic_t volatile _count;

    /* queue head node pointer */
    struct _node_t *_head;

    /* queue tail node pointer */
    struct _node_t *_last;

    /* mutex lock : get element */
    pthread_mutex_t _take_lock;

    /* conditional lock : queue non-empty */
    pthread_cond_t _not_empty;

    /* mutex lock : add element */
    pthread_mutex_t _put_lock;

    /* conditional lock : queue non-full */
    pthread_cond_t _not_full;
};

#define QUEUE_MAX_CAPACITY 0xFFFFFFFFU

static inline void _enqueue(struct lb_queue_t *const thiz, struct _node_t *node)
{
    thiz->_last->next = node;
    thiz->_last = node;
}

static inline void *_dequeue(struct lb_queue_t *const thiz)
{
    struct _node_t *h = thiz->_head;
    struct _node_t *first = h->next;
    h->next = NULL;
    thiz->_head = first;
    void *x = (void *)first->item;
    first->item = NULL;

    free(h);
    return x;
}

/**
 * Locks to prevent both puts and takes.
 */
static inline void _fully_lock(struct lb_queue_t *const thiz)
{
    pthread_mutex_lock(&thiz->_take_lock);
    pthread_mutex_lock(&thiz->_put_lock);
}

/**
 * Unlocks to allow both puts and takes.
 */
static inline void _fully_unlock(struct lb_queue_t *const thiz)
{
    pthread_mutex_unlock(&thiz->_put_lock);
    pthread_mutex_unlock(&thiz->_take_lock);
}

static inline void _signal_not_empty(struct lb_queue_t *const thiz)
{
    pthread_mutex_lock(&thiz->_take_lock);
    pthread_cond_signal(&thiz->_not_empty);
    pthread_mutex_unlock(&thiz->_take_lock);
}

/**
 * Signals a waiting put. Called only from take/poll.
 */
static inline void _signal_not_full(struct lb_queue_t *const thiz)
{
    pthread_mutex_lock(&thiz->_put_lock);
    pthread_cond_signal(&thiz->_not_full);
    pthread_mutex_unlock(&thiz->_put_lock);
}

static uint32_t lb_queue_size(struct lb_queue_t *const thiz)
{
    return thiz->_count;
}

static void lb_queue_clear(struct lb_queue_t *const thiz)
{
    if (!thiz)
    {
        errno = ENOMEM;
        return;
    }

    int c;

    _fully_lock(thiz);

    for (struct _node_t *next, *current = thiz->_head->next; current != NULL; current = next)
    {
        next = current->next;

        if (current->item)
            free((void *)current->item);

        free(current);
    }

    thiz->_head->next = NULL;
    thiz->_last = thiz->_head;

    c = atomic_fetch_and(&thiz->_count, 0x0);
    if (c == thiz->_capacity)
        pthread_cond_signal(&thiz->_not_full);

    _fully_unlock(thiz);

    return;
}

static void lb_queue_free(struct lb_queue_t *const thiz)
{
    if (!thiz)
    {
        errno = ENOMEM;
        return;
    }

    thiz->clear(thiz);

    pthread_cond_destroy(&thiz->_not_empty);
    pthread_cond_destroy(&thiz->_not_full);
    pthread_mutex_destroy(&thiz->_take_lock);
    pthread_mutex_destroy(&thiz->_put_lock);

    free(thiz->_head);
    free(thiz);
}

static bool lb_queue_offer(struct lb_queue_t *const thiz, const void *const element)
{
    if (!thiz || !element)
    {
        errno = EINVAL;
        return false;
    }

    if (thiz->_count == thiz->_capacity)
        return false;

    uint32_t c;

    struct _node_t *new_node = (struct _node_t *)malloc(sizeof(struct _node_t));
    if (!new_node)
    {
        errno = ENOMEM;
        return false;
    }

    new_node->item = element;
    new_node->next = NULL;

    /* element enqueue */
    pthread_mutex_lock(&thiz->_put_lock);

    if (thiz->_count == thiz->_capacity)
        goto insert_full;

    _enqueue(thiz, new_node);

    c = atomic_fetch_add(&thiz->_count, 1);
    if ((c + 1) < thiz->_capacity)
        pthread_cond_signal(&thiz->_not_full);

    pthread_mutex_unlock(&thiz->_put_lock);

    if (c == 0)
        _signal_not_empty(thiz);

    return true;

insert_full:
    pthread_mutex_unlock(&thiz->_put_lock);
    free(new_node);

    return false;
}

static void *lb_queue_poll(struct lb_queue_t *const thiz)
{
    if (!thiz)
    {
        errno = ENOMEM;
        return NULL;
    }

    void *item = NULL;
    uint32_t c;

    pthread_mutex_lock(&thiz->_take_lock);

    if (thiz->_count == 0)
        goto take_empty;

    item = _dequeue(thiz);
    c = atomic_fetch_sub(&thiz->_count, 1);
    if (c > 1)
        pthread_cond_signal(&thiz->_not_empty);

    pthread_mutex_unlock(&thiz->_take_lock);
    if (c == thiz->_capacity)
        _signal_not_full(thiz);

    return item;

take_empty:
    pthread_mutex_unlock(&thiz->_take_lock);

    return NULL;
}

static void *lb_queue_peek(struct lb_queue_t *const thiz)
{
    if (!thiz)
    {
        errno = ENOMEM;
        return NULL;
    }

    if (thiz->_count == 0)
        return NULL;

    void *item = NULL;
    pthread_mutex_lock(&thiz->_take_lock);
    item = thiz->_count > 0 ? (void *)thiz->_head->next->item : NULL;
    pthread_mutex_unlock(&thiz->_take_lock);

    return item;
}

static bool lb_queue_put(struct lb_queue_t *const thiz, const void *const element)
{
    if (!thiz || !element)
    {
        errno = EINVAL;
        return false;
    }

    int c;

    struct _node_t *new_node = (struct _node_t *)malloc(sizeof(struct _node_t));
    if (!new_node)
    {
        errno = ENOMEM;
        return false;
    }

    new_node->item = element;
    new_node->next = NULL;

    pthread_mutex_lock(&thiz->_put_lock);

    while (thiz->_count == thiz->_capacity)
    {
        pthread_cond_wait(&thiz->_not_full, &thiz->_put_lock);
    }

    _enqueue(thiz, new_node);

    c = atomic_fetch_add(&thiz->_count, 1);
    if (c + 1 < thiz->_capacity)
        pthread_cond_signal(&thiz->_not_full);

    pthread_mutex_unlock(&thiz->_put_lock);

    if (c == 0)
        _signal_not_empty(thiz);

    return true;
}

static void *lb_queue_take(struct lb_queue_t *const thiz)
{
    if (!thiz)
    {
        errno = ENOMEM;
        return NULL;
    }

    int c;
    void *item = NULL;

    pthread_mutex_lock(&thiz->_take_lock);

    while (thiz->_count == 0)
    {
        pthread_cond_wait(&thiz->_not_empty, &thiz->_take_lock);
    }

    item = _dequeue(thiz);

    c = atomic_fetch_sub(&thiz->_count, 1);
    if (c > 1)
        pthread_cond_signal(&thiz->_not_empty);

    pthread_mutex_unlock(&thiz->_take_lock);

    if (c == thiz->_capacity)
        _signal_not_full(thiz);

    return item;
}

/*
static bool lb_queue_offer_wait(struct lb_queue_t *const thiz, const void *const element,
                                const uint64_t timeout, const struct time_unit_t *const unit)
{
    if (!thiz || !element || !unit)
    {
        errno = EINVAL;
        return false;
    }

    int c;

    pthread_mutex_lock(&thiz->_put_lock);
    struct timespec timeo;
    uint64_t nano_timeout;
    uint64_t nanos;
    int64_t nanotime;

    calc_timeout(&timeo, timeout, unit);
    nano_timeout = timespec_to_nano(&timeo);
    nanos = nano_timeout;

    while (thiz->_count == thiz->_capacity)
    {
        if (nanos <= 0)
            goto result_r;

        if (pthread_cond_timedwait(&thiz->_not_full, &thiz->_put_lock, &timeo) == ETIMEDOUT)
            goto result_r;

        if ((nanotime = nano_time()) < 0)
            goto result_r;

        nanos = nano_timeout - nanotime;
    }

    struct _node_t *new_node = (struct _node_t *)malloc(sizeof(struct _node_t));
    if (!new_node)
    {
        errno = ENOMEM;
        goto result_r;
    }
    new_node->item = element;
    new_node->next = NULL;

    _enqueue(thiz, new_node);
    c = atomic_fetch_add(&thiz->_count, 1);
    if (c + 1 < thiz->_capacity)
        pthread_cond_signal(&thiz->_not_full);

    pthread_mutex_unlock(&thiz->_put_lock);

    if (c == 0)
        _signal_not_empty(thiz);

    return true;

result_r:
    pthread_mutex_unlock(&thiz->_put_lock);

    return false;
}
*/

/*
static void *lb_queue_poll_wait(struct lb_queue_t *const thiz, const uint64_t timeout, const struct time_unit_t *const unit)
{
    if (!thiz || !unit)
    {
        errno = ENOMEM;
        return NULL;
    }

    void *item = NULL;
    int c;

    pthread_mutex_lock(&thiz->_take_lock);

    struct timespec timeo;
    uint64_t nano_timeout;
    uint64_t nanos;
    int64_t nanotime;

    calc_timeout(&timeo, timeout, unit);
    nano_timeout = timespec_to_nano(&timeo);
    nanos = nano_timeout;

    while (thiz->_count == 0)
    {
        if (nanos <= 0)
            goto result_r;

        if (pthread_cond_timedwait(&thiz->_not_empty, &thiz->_take_lock, &timeo) == ETIMEDOUT)
            goto result_r;

        if ((nanotime = nano_time()) < 0)
            goto result_r;

        nanos = nano_timeout - nanotime;
    }

    item = _dequeue(thiz);
    c = atomic_fetch_sub(&thiz->_count, 1);
    if (c > 1)
        pthread_cond_signal(&thiz->_not_empty);

    pthread_mutex_unlock(&thiz->_take_lock);

    if (c == thiz->_capacity)
        pthread_cond_signal(&thiz->_not_full);

    return item;

result_r:
    pthread_mutex_unlock(&thiz->_take_lock);

    return item;
}
*/

struct commu_queue_t *lb_queue(const uint32_t capacity)
{
    pthread_condattr_t cond_attr;

    struct lb_queue_t *const thiz = (struct lb_queue_t *)malloc(sizeof(struct lb_queue_t));
    if (thiz == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }

    memset((void *)thiz, 0, sizeof(struct lb_queue_t));

    thiz->_capacity = (capacity == 0) ? QUEUE_MAX_CAPACITY : capacity;

    /* thread cond timeout block's way CLOCK_REALTIME --> CLOCK_MONOTONIC */
    pthread_condattr_init(&cond_attr);
    pthread_condattr_setclock(&cond_attr, CLOCK_MONOTONIC);

    pthread_mutex_init(&thiz->_take_lock, NULL);
    pthread_cond_init(&thiz->_not_empty, &cond_attr);
    pthread_mutex_init(&thiz->_put_lock, NULL);
    pthread_cond_init(&thiz->_not_full, &cond_attr);

    pthread_condattr_destroy(&cond_attr);

    thiz->_head = (struct _node_t *)malloc(sizeof(struct _node_t));
    thiz->_head->next = NULL;
    thiz->_head->item = NULL;
    thiz->_last = thiz->_head;

    atomic_fetch_and(&thiz->_count, 0x0);

    /* methods */
    thiz->size = lb_queue_size;
    thiz->clear = lb_queue_clear;
    thiz->free = lb_queue_free;
    thiz->offer = lb_queue_offer;
    thiz->poll = lb_queue_poll;
    thiz->peek = lb_queue_peek;
    thiz->put = lb_queue_put;
    thiz->take = lb_queue_take;

    return (struct commu_queue_t *)thiz;
}
