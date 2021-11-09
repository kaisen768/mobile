#ifndef _MOBILE_H_
#define _MOBILE_H_

#include "uv.h"
#include "ATChannel/atchannel.h"
#include "apnconf.h"
#include "Dial/dial.h"

typedef struct mobile_s {
    /* Globle loop */
    uv_loop_t *loop;

    /* private */
    uv_timer_t _timer;
    void (*_hardware_reset_cb)(void *extra);

    /* ATChannel */
    atchannel_t atchannel;

    /* APN value configure */
    apnconf_t apnconf;

    /* Dial */
    dial_t dial;

    /** 
     * Mobile module start work
     * 
     * @thiz: this
     * @return none
     */
    void (*start)(struct mobile_s *thiz);

    /** 
     * Mobile module stop work
     * 
     * @thiz: this
     * @return none
     */
    void (*stop)(struct mobile_s *thiz);
} mobile_t;

int mobile_init(mobile_t *mobile, uv_loop_t *loop, void (*hardware_reset_cb)(void *));

#endif
