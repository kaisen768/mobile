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
    atchannel_t *atchannel = &mobile->atchannel;
    apnconf_t *apnconf = &mobile->apnconf;
    dial_t *dial = &mobile->dial;
    pppd_module_t *pppd = &dial->pppd;

    static unsigned int usbdevice_lost_count = 0;
    static abnormal_state_t abnormal_state = ABNORMAL_STATE_NONE;

    char *apn = NULL;
    static int apn_idx = -1;
    static int apn_err_times = 0;

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
    case MOBILE_STATE_ATCHANNEL_CHECK:
        if (atchannel->status == ATCHANNEL_UNOPENED) {
            if (check_usbport_status(USBDEVICE_4G_USB4) != true) {
                MOBILE_STATE_JUMPTO(MOBILE_STATE_USBDEVICE_CHECK);
            } else {
                /*
                 * Prevent the ATChannel start interface from being called again 
                 * when the AT reset command is requested 
                 */
                if (mobile->atchannel.reset == ATRESET_RESETTING)
                    mobile->atchannel.reset = ATRESET_SUCCESS;

                if (mobile->atchannel.reset == ATRESET_NOACTIONS 
                    || mobile->atchannel.reset == ATRESET_SUCCESS) {
                    mobile->atchannel.reset = ATRESET_NOACTIONS;
                    mobile->atchannel.start(&mobile->atchannel, USBDEVICE_4G_USB2, -1);
                }
            }
        } else if (atchannel->status == ATCHANNEL_PROCESSING) {
            /* 
             * Determine if the SIM card is pulled out 
             */
            if (atchannel->determine_simswitch() == true)
                apnconf->reacquire(apnconf);
        } else if (atchannel->status == ATCHANNEL_READY) {
            MOBILE_STATE_JUMPTO(MOBILE_STATE_APNCONFIG_CHECK);
        }
        goto delay1000ms;

    /** 
     * Configure APN value
     * 
     * @note If it is detected that the dialing process is not started, enter 
     *      the APN configuration.
     *      Populate the dial script 
     *              (mcc, mnc --> apn)
     ------------------------------------------------------------------------------- */
    case MOBILE_STATE_APNCONFIG_CHECK:
        if (dial->status == DIAL_UNOPENED) {
            apn_idx = apnconf->configure(apnconf, (char *)me909s_mcc(atchannel->me909s), 
                                (char *)me909s_mnc(atchannel->me909s), &apn);
            if (apn_idx == APNCONF_MNCC_INVAIL) {
                atchannel->command.monsc(atchannel);
                goto delay1000ms;
            } else if (apn_idx == APNCONF_MATCH_FAILED) {
                abnormal_state = ABNORMAL_STATE_APN_ERROR;
                goto delay1000ms;
            }

            fprintf(stderr, "Mobile Current APN : %s\n", apn);

            /* 
             * all apn value of apn.conf is invail(for dial-up)
             * change the config method to mxl
             */
            if (++apn_err_times >= apnconf->count)
                apnconf->changeto_xml(apnconf);

            /*
             *  the user can configure the ppp parameters by calling the 
             *   < dial->pppd.arg_set >
             * interface, otherwise the default parameters will be used
             *      interface : "ppp0"
             *      proc_name : "pppd"
             *      chat_file : "/tmp/chat-default"
             */
            // pppd->arg_set(pppd, interface, PPPD_ARG_interface);
            // pppd->arg_set(pppd, exec, PPPD_ARG_EXEC);
            // pppd->arg_set(pppd, chat_file, PPPD_ARG_CHAT_FILE);
            pppd->arg_set(pppd, apn, PPPD_ARG_APN);

            MOBILE_STATE_JUMPTO(MOBILE_STATE_DIALPROCS_CHECK);
        } else if (dial->status == DIAL_REMAIN) {
            MOBILE_STATE_JUMPTO(MOBILE_STATE_DIALPROCS_CHECK);
        }
        break;

    /** 
     * Check the status of the dialing process
     * 
     * @note If it is detected that the dialing process is not started, 
     *      the dialing process is started.
     ------------------------------------------------------------------------------- */
    case MOBILE_STATE_DIALPROCS_CHECK:
        if (atchannel->status != ATCHANNEL_READY) {
            MOBILE_STATE_JUMPTO(MOBILE_STATE_ATCHANNEL_CHECK);
        } else {
            if (dial->status == DIAL_UNOPENED) {
                dial->start(dial, NULL, NULL, NULL);
                goto delay1000ms;
            } else if (dial->status == DIAL_PROCESSING) {
                if (dial->online(dial) != true)
                    goto delay1000ms;

                dial->status = DIAL_HOLDON;

                /* 
                * hold on available APN (use this for next dial-up)
                */
                apn_err_times = 0;
                apnconf->hold(apnconf, apn_idx);

                MOBILE_STATE_JUMPTO(MOBILE_STATE_DIALHOLDS_CHECK);
            } else if (dial->status == DIAL_REMAIN) {
                if (pthread_kill(pppd->pid, 0) != ESRCH)
                    uv_kill(pppd->pid, SIGTERM);
                else
                    dial->status = DIAL_UNOPENED;
                goto delay3000ms;
            }
        }
        break;

    /** 
     * Status polling for successful dialing
     * 
     * @mobile_success: The dialing process started successfully
     * @1 Hardware status part status detection
     * @2 Software processing part status detection
     * @3 Log
     *      1) Record online time *
     *      2) Record the dialing exit time and save the first segment of dialing information *
     ------------------------------------------------------------------------------- */
    case MOBILE_STATE_DIALHOLDS_CHECK:
        if (check_usbport_status(USBDEVICE_4G_USB4) != true) {
            MOBILE_STATE_JUMPTO(MOBILE_STATE_USBDEVICE_CHECK);
            break;
        }
        if (atchannel->status != ATCHANNEL_READY ||
            dial->status != DIAL_HOLDON) 
        {
            MOBILE_STATE_JUMPTO(MOBILE_STATE_ATCHANNEL_CHECK);
            break;
        }
        /*
         * Log : dial processor information
         *      @1 online time
         *      ...
         */
        goto delay1000ms;

    default:
        break;
    }

delay10ms:
    __delaynms(handle, 10);
    return;

delay1000ms:
    __delaynms(handle, 1000);
    return;

delay3000ms:
    __delaynms(handle, 3000);
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
    atchannel_t *atchannel = &mobile->atchannel;
    dial_t *dial = &mobile->dial;

    if (!mobile)
        return;

    uv_timer_stop(&mobile->_timer);
    uv_close((uv_handle_t*)&mobile->_timer, NULL);

    /* release atchannel */
    if (atchannel->status != ATCHANNEL_UNOPENED)
        atchannel->stop(atchannel);

    /* kill dial processor */
    if (dial->status != DIAL_UNOPENED)
        dial->stop(dial);
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

    /* ATChannel context init */
    atchannel_init(&mobile->atchannel, loop);

    /* APN configure init */
    apnconf_init(&mobile->apnconf);

    /* Dial context init */
    dial_init(&mobile->dial, loop);

    return 0;
}
