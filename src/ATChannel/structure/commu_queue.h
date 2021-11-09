#ifndef _BLOCKING_QUEUE_H_
#define _BLOCKING_QUEUE_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * Communication Queue
 */
struct commu_queue_t {
    
    /** 
     * Returns the number of elements in this collection.
     *
     * @param thiz this
     * @return the number of elements in this collection
     */
    uint32_t (*size)(struct commu_queue_t * const thiz);

    /**
     * Removes all of the elements from this collection.
     *
     *  @param thiz this
     */
    void (*clear)(struct commu_queue_t * const thiz);

    /**
     * Free collection
     *
     * @param thiz this
     */
    void (*free)(struct commu_queue_t * const thiz);

    /** 
     * Inserts the specified element into this queue if it is possible to do
     * so immediately without violating capacity restrictions.
     *
     * @param thiz this
     * @param element element
     * @return true if the element was added to this queue, else false
     */
    bool (*offer)(struct commu_queue_t * const thiz, const void *element);

    /** 
     * Retrieves and removes the head of this queue,
     * or returns NULL if this queue is empty.
     *
     * @param thiz this
     * @return the head of this queue, or NULL if this queue is empty 
     */
    void *(*poll)(struct commu_queue_t * const thiz);

    /** 
     * Retrieves, but does not remove, the head of this queue,
     * or returns NULL if this queue is empty.
     *
     * @param thiz this
     * @return the head of this queue, or NULL if this queue is empty
     */
    void *(*peek)(struct commu_queue_t * const thiz);

    /**
     * Inserts the specified element into this queue, waiting if necessary for space to become available.
     *
     * @param thiz this
     * @param element the element to add
     * @return true if the element was added to this queue, else false
     */
    bool (*put)(struct commu_queue_t * const thiz, const void *element);
    
    /** 
     * Retrieves and removes the head of this queue, waiting if necessary until an element becomes available.
     *
     * @param thiz this
     * @return the head of this queue
     */
    void *(*take)(struct commu_queue_t * const thiz);
};

struct commu_queue_t *commu_queue(const uint32_t capacity);

#ifdef __cplusplus
}
#endif

#endif