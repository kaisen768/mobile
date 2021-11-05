#include "atconfigure.h"
#include "atchannel.h"

typedef enum atconfigure_steps_e {
    ATCONFIGURE_IDENTIFICATION,
    ATCONFIGURE_S_LEDCTRL,
    ATCONFIGURE_G_LEDCTRL,
    ATCONFIGURE_S_IPLEN,
    ATCONFIGURE_G_IPLEN,
    ATCONFIGURE_S_ECALL,
    ATCONFIGURE_G_ECALL,
    ATCONFIGURE_SIM_AUTOCHECK,
    ATCONFIGURE_G_CPIN,
    ATCONFIGURE_HOTPLUS,
    ATCONFIGURE_NET_UNCONNECT,
    ATCONFIGURE_NET_WAITREADY,
    ATCONFIGURE_S_NETMODE,
    ATCONFIGURE_G_NETMODE,
    ATCONFIGURE_READY,
} atconfigure_steps_t;

static atconfigure_steps_t atconfigure_step = ATCONFIGURE_IDENTIFICATION;

#define ATCONFIGURE_STEP_JUMPTO(s)  do { atconfigure_step = s; } while (0)

static inline void __delaynms(uv_timer_t *handle, int nms)
{
    if (uv_timer_get_repeat(handle) == nms)
        return;

    uv_timer_set_repeat(handle, nms);
    uv_timer_again(handle);
}

void atconfigure_handler(void *handle)
{
    uv_timer_t *timer = handle;
    atchannel_t *atchannel = timer->data;

    switch (atconfigure_step) {
    case ATCONFIGURE_READY:
        goto delay1000ms;

    case ATCONFIGURE_IDENTIFICATION:
        if ()
        break;
    }



delay1000ms:
    __delaynms(timer, 1000);
    return;
}
