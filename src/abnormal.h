#ifndef _ABNORMAL_H_
#define _ABNORMAL_H_

#include "uv.h"

typedef enum abnormal_id_e abnormal_id_t;

enum abnormal_id_e {
    ABNORMAL_ID_USBDEVICE = 0,
    ABNORMAL_ID_APNCONFIG,
    ABNORMAL_ID_NUM,
};

typedef void (*abnormal_handletask_cb)(void *arg);

void abnormal_handletask_add(uv_loop_t *loop, abnormal_handletask_cb handletask, void *arg, abnormal_id_t abnormalid);

#endif
