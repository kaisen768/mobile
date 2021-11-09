#ifndef _ATCHANNEL_H_
#define _ATCHANNEL_H_

#include "uv.h"
#include "structure/commu_queue.h"
#include "communication/serial.h"
#include "4Gmodule_paras.h"

#define ME909S_USB_PORT "/dev/ttyUSB2"

typedef enum atchannel_status_e {
    ATCHANNEL_UNOPENED,
    ATCHANNEL_PROCESSING,
    ATCHANNEL_READY
} atchannel_status_t;

typedef enum atreset_status_e {
    ATRESET_NOACTIONS,
    ATRESET_REQUEST,
    ATRESET_RESETTING,
    ATRESET_SUCCESS
} atreset_status_t;

typedef struct atchannel_s {
    uv_loop_t *loop;

    /* serial port */
    struct serial_s serial;
    char *uart_name;
    unsigned int baudrate;

    /* current status */
    atchannel_status_t status;

    /* AT port reset status */
    atreset_status_t reset;

    /* 4G module parameters */
    shm_4Gmodule_t *me909s;

    /* 4G module network mode */
    me909s_syscfgex_netmode_t netmode;

    /* private */
    uv_pipe_t   _pipe;
    uv_buf_t    _buf;
    uv_async_t  _async;
    uv_timer_t  _timer;

    /* write at command queue */
    struct commu_queue_t *_queue;

    /** 
     * 4G module AT-command communication start
     * 
     * @thiz: this
     * @uart_name: port path name
     * @netmode: 4G module network mode 
     *          -1 , use default value (auto mode)
     *          me909s_syscfgex_netmode_t , select mode
     * @return none
     */
    void (*start)(struct atchannel_s *thiz, const char *uart_name, int netmode);

    /** 
     * 4G module AT-command communication stop
     * 
     * @thiz: this
     * @return none
     */
    void (*stop)(struct atchannel_s *thiz);

    /** 
     * Send serial data to thiz module
     */
    int (*sendto)(struct atchannel_s *thiz, uv_buf_t *wb);

    /** 
     * Determine atstatus is simswitch
     * 
     * @thiz: this
     * @return true if atstatus is simswitch, else false
     */
    bool (*determine_simswitch)(void);

    /** 
     * Command interfaces
     */
    struct {
        int (*indentification)(struct atchannel_s *thiz);
        int (*sysinfoex)(struct atchannel_s *thiz);
        int (*unconnect)(struct atchannel_s *thiz);
        int (*ledctrl_set)(struct atchannel_s *thiz);
        int (*ledctrl_get)(struct atchannel_s *thiz);
        int (*syscfgex_set)(struct atchannel_s *thiz);
        int (*syscfgex_get)(struct atchannel_s *thiz);
        int (*ipcfl_set)(struct atchannel_s *thiz);
        int (*ipcfl_get)(struct atchannel_s *thiz);
        int (*eCall_set)(struct atchannel_s *thiz);
        int (*eCall_get)(struct atchannel_s *thiz);
        int (*cpin_set)(struct atchannel_s *thiz);
        int (*cpin_get)(struct atchannel_s *thiz);
        int (*simswitch_set)(struct atchannel_s *thiz, int card);
        int (*simswitch_get)(struct atchannel_s *thiz);
        int (*network_set)(struct atchannel_s *thiz);
        int (*network_get)(struct atchannel_s *thiz);
        int (*monsc)(struct atchannel_s *thiz);
        int (*signal)(struct atchannel_s *thiz);
    } command;
} atchannel_t;

int atchannel_init(atchannel_t *atchannel, uv_loop_t *loop);

#endif
