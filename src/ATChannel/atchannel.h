#ifndef _ATCHANNEL_H_
#define _ATCHANNEL_H_

#include "uv.h"
#include "structure/commu_queue.h"
#include "communication/serial.h"

#define ME909S_USB_PORT "/dev/ttyUSB2"

typedef enum atchannel_status_e {
    ATCHANNEL_UNOPENED,
    ATCHANNEL_PROCESSING,
    ATCHANNEL_READY
} atchannel_status_t;

typedef enum atreset_status_e {
    ATRESET_NOACTIONS,
    ATRESET_REQUEST,
    ATRESET_RESTTING,
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
     * @return none
     */
    void (*start)(struct atchannel_s *thiz, const char *uart_name);

    /** 
     * 4G module AT-command communication stop
     * 
     * @thiz: this
     * @return none
     */
    void (*stop)(struct atchannel_s *thiz);

} atchannel_t;

int atchannel_init(atchannel_t *atchannel, uv_loop_t *loop);

#endif
