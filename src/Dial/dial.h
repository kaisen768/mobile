#ifndef _DIAL_H_
#define _DIAL_H_

#include <stdbool.h>
#include "uv.h"
#include "PPPDmodule_paras.h"

typedef enum dial_status_e {
    DIAL_UNOPENED = 0,
    DIAL_PROCESSING,
    DIAL_REMAIN,
    DIAL_HOLDON,
} dial_status_t;

typedef struct dial_s {
    uv_loop_t *loop;

    /* dial status */
    dial_status_t status;

    /* PPPD processor parameters */
    PPPDmodule_t *pppd;

    /* private - read only */
    uv_process_t _proc;
    uv_process_options_t _opts;
    uv_pipe_t _pipe_read;
    uv_buf_t _buf;

    /** 
     * 4G module pppd dial start
     * 
     * @thiz: this
     * @proc: process path name
     * @return none
     */
    void (*start)(struct dial_s *dial, char *interface, char *proc, char *chat_file);

    /** 
     * 4G module pppd dial stop
     * 
     * @thiz: this
     * @return none
     */
    void (*stop)(struct dial_s *thiz);

    /** 
     * APN Adaptation
     * 
     * @thiz: this
     * @return 
     */
    int (*args)(void *thiz, char *apn);

    /** 
     * Check pppd is online
     * 
     * @thiz: this
     * @return true is returned if module online, else false
     */
    bool (*online)(struct dial_s *thiz);

    /** 
     * PPP Parameter setup
     * 
     * @thiz: tihs
     * 
     * @return none
     */
    void (*ppp_setup)(struct dial_s *thiz, const char *interface, const char *name, const char *chat_file);
} dial_t;

int dial_init(dial_t *dial, uv_loop_t *loop);

#endif
