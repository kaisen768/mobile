#include "mobile.h"
#include "abnormal.h"
#include "usbdevice.h"

typedef enum mobile_state_e {
    MOBILE_STATE_USBDEVICE_CHECK,
    MOBILE_STATE_ATCHANNEL_CHECK,
    MOBILE_STATE_APNCONFIG_CHECK,
    MOBILE_STATE_DIALPROCS_CHECK,
    MOBILE_STATE_DIALHOLDS_CHECK,
} mobile_state_t;

typedef enum abnormal_state_e {
    ABNORMAL_STATE_NONE,
    ABNORMAL_STATE_LOST_USBDEVICE,
    ABNORMAL_STATE_APN_ERROR,
} abnormal_state_t;

static mobile_state_t mobile_state = MOBILE_STATE_USBDEVICE_CHECK;
#define MOBILE_STATE_JUMPTO(s)      do { mobile_state = s; } while(0)
#define MOBILE_STATE_JUMPTO_NEXT    do { mobile_state++; } while(0)

static inline void __delaynms(uv_timer_t *handle, int delaynms)
{
    if (uv_timer_get_repeat(handle) == delaynms) 
        return;

    uv_timer_set_repeat(handle, delaynms);
    uv_timer_again(handle);
}

static void mobile_handler(uv_timer_t *handle)
{
    struct mobile_s *mobile = handle->data;

    static unsigned int usbdevice_lost_count = 0;
    static abnormal_state_t abnormal_state = ABNORMAL_STATE_NONE;

    /** --------------------------------------------------- 
     * Abnormal state handling
     * 
     * @note Exception handling interface tasks are 
     *       submitted to the multi-threaded task queue
     ------------------------------------------------------ */
    switch (abnormal_state) {
    case ABNORMAL_STATE_LOST_USBDEVICE:
        fprintf(stderr, "usbdevice abnormal !!!\n");
        if (mobile->_hardware_reset_cb)
            abnormal_handletask_add(mobile->loop, mobile->_hardware_reset_cb, NULL, ABNORMAL_ID_USBDEVICE);
        abnormal_state = ABNORMAL_STATE_NONE;
        break;

    case ABNORMAL_STATE_APN_ERROR:
        fprintf(stderr, "apn configure abnormal !!!\n");
        abnormal_state = ABNORMAL_STATE_NONE;
        break;

    default:
        break;
    }

    switch (mobile_state) {
    /**
     * NIC USB device and port detection
     * 
     * @note Detect whether the USB device of the network card and the 
     *      serial communication port exist, if not, jump to exception handling.
     ------------------------------------------------------------------------------- */
    case MOBILE_STATE_USBDEVICE_CHECK:
        if (check_usbport_status(USBDEVICE_4G_USB4) != true) {
            if (usbdevice_lost_count++ >= 10) {
                usbdevice_lost_count = 0;
                abnormal_state = ABNORMAL_STATE_LOST_USBDEVICE;
            }
            goto delay1000ms;
        }
        usbdevice_lost_count = 0;
        MOBILE_STATE_JUMPTO(MOBILE_STATE_ATCHANNEL_CHECK);
        break;

    /** 
     * NIC ATChannel configuration status detection
     * 
     * @note Check the current state of ATChannel, if the ATChannel module 
     *      is not started, start the module.
     ------------------------------------------------------------------------------- */
    

    }

delay1000ms:
    __delaynms(handle, 1000);
    return;
}

static void start(struct mobile_s *mobile)
{
    if (!mobile)
        return;

    if (!mobile->loop)
        return;

    uv_timer_init(mobile->loop, &mobile->_timer);
    uv_timer_start(&mobile->_timer, mobile_handler, 1000, 1000);
}

static void stop(struct mobile_s *mobile)
{
    if (!mobile)
        return;

    uv_timer_stop(&mobile->_timer);
    uv_close((uv_handle_t*)&mobile->_timer, NULL);
}

int mobile_init(mobile_t *mobile, uv_loop_t *loop, void (*hardware_reset_cb)(void *))
{
    if (!mobile || !loop)
        return -1;

    mobile->loop = loop;
    mobile->_hardware_reset_cb = hardware_reset_cb;
    mobile->_timer.data = mobile;

    mobile->start = start;
    mobile->stop = stop;

    return 0;
}
