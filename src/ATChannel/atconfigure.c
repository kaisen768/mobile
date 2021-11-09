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

static int sim_card = 1;
static bool polling_element_exchange = false;

#define ATCONFIGURE_STEP_JUMPTO(s)  do { atconfigure_step = s; } while (0)

static inline void __delaynms(uv_timer_t *handle, int nms)
{
    if (uv_timer_get_repeat(handle) == nms)
        return;

    uv_timer_set_repeat(handle, nms);
    uv_timer_again(handle);
}

static inline int cme_error_handler(shm_4Gmodule_t *shm, atconfigure_steps_t *step)
{
    if (judge_4Gmodule_para(shm, ME909S_ID_CMEERROR) == false)
        return -1;

    switch (shm->cme) {
    case CME_SIM_WRONG:
    case CME_SIM_FAILURE:
    case CME_SIM_NOTINSERTED:
        *step = ATCONFIGURE_HOTPLUS;
        break;

    case CME_SIM_PIN_REQUIRED:
        /* sim pin required (need sim pin password) */
        break;

    case CME_SIM_PUK_REQUIRED:
        /* sim puk required (need sim puk password) */
        break;

    case CME_SIM_BUSY:
        /* SIM card busy */
        break;

    case CME_MEMORY_FULL:
        /* memory full */
        break;

    default:
        break;
    }

    return 0;
}

void atconfigure_handler(void *handle)
{
    uv_timer_t *timer = handle;
    atchannel_t *atchannel = timer->data;

    switch (atconfigure_step) {
    case ATCONFIGURE_READY:
        goto delay1000ms;

    case ATCONFIGURE_IDENTIFICATION:
        if (judge_4Gmodule_para(atchannel->me909s, ME909S_ID_INDENTIFICATION) == false) {
            atchannel->command.indentification(atchannel);
            goto delay500ms;
        }
        ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_S_LEDCTRL);
        break;

    case ATCONFIGURE_S_LEDCTRL:
        if (judge_4Gmodule_para(atchannel->me909s, ME909S_ID_LEDCTRL) == false) {
            atchannel->command.ledctrl_set(atchannel);
            ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_G_LEDCTRL);
        } else {
            ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_G_CPIN);
        }
        break;

    case ATCONFIGURE_SIM_AUTOCHECK:
        if (judge_4Gmodule_para(atchannel->me909s, ME909S_ID_CPIN) == false) {
            if (cme_error_handler(atchannel->me909s, &atconfigure_step) != 0)
                ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_G_CPIN);
        } else {
            atchannel->command.sysinfoex(atchannel);
            reset_shm_4Gmodule(atchannel->me909s, ME909S_ID_CMEERROR);
            reset_shm_4Gmodule(atchannel->me909s, ME909S_ID_SIMSTATE);
            ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_S_IPLEN);
        }
        break;

    case ATCONFIGURE_HOTPLUS:
        if (judge_4Gmodule_para(atchannel->me909s, ME909S_ID_SIMSTATE) == false) {
            /* sim switch */
            sim_card = (sim_card ? 0 : 1);
            atchannel->command.simswitch_set(atchannel, sim_card);

            /* sim error process ... */
            goto delay3000ms;
        }
        reset_shm_4Gmodule(atchannel->me909s, ME909S_ID_CMEERROR);
        reset_shm_4Gmodule(atchannel->me909s, ME909S_ID_CPIN);
        reset_shm_4Gmodule(atchannel->me909s, ME909S_ID_MONSC);
        ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_SIM_AUTOCHECK);
        break;

    case ATCONFIGURE_S_IPLEN:  // cme
        if (judge_4Gmodule_para(atchannel->me909s, ME909S_ID_IPCFL) == false)
        {
            atchannel->command.ipcfl_set(atchannel);
            if (cme_error_handler(atchannel->me909s, &atconfigure_step) != 0)
                ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_G_IPLEN);
            goto delay500ms;
        }
        ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_S_ECALL);
        break;

    case ATCONFIGURE_S_ECALL:  // cme
        if (judge_4Gmodule_para(atchannel->me909s, ME909S_ID_ECALL) == false) {
            atchannel->command.eCall_set(atchannel);
            ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_G_ECALL);
        }
        ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_NET_UNCONNECT);
        break;

    case ATCONFIGURE_NET_UNCONNECT:
        atchannel->command.unconnect(atchannel);
        ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_NET_WAITREADY);
        break;

    case ATCONFIGURE_NET_WAITREADY:
        if (judge_4Gmodule_para(atchannel->me909s, ME909S_ID_CREG) == false) {
            if (cme_error_handler(atchannel->me909s, &atconfigure_step) != 0) {
                if (polling_element_exchange)
                    atchannel->command.sysinfoex(atchannel);
                else
                    atchannel->command.network_get(atchannel);

                polling_element_exchange = polling_element_exchange ? false : true;
            }
            goto delay500ms;
        }
        atchannel->command.signal(atchannel); /* update signal value */
        ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_S_NETMODE);
        break;

    case ATCONFIGURE_S_NETMODE:
        if (me909s_syscfgex_acqorder_to_mode((char *)me909s_acqorder(atchannel->me909s)) != 
            atchannel->netmode)
        {
            atchannel->command.syscfgex_set(atchannel);
            ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_G_NETMODE);
            goto delay500ms;
        } else {
            atchannel->command.network_get(atchannel);
            ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_READY);
        }
        break;

    case ATCONFIGURE_G_LEDCTRL:
        atchannel->command.ledctrl_get(atchannel);
        ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_S_LEDCTRL);
        goto delay1000ms;

    case ATCONFIGURE_G_CPIN:
        atchannel->command.cpin_get(atchannel);
        ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_SIM_AUTOCHECK);
        goto delay1000ms;

    case ATCONFIGURE_G_IPLEN:
        atchannel->command.ipcfl_get(atchannel);
        ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_S_IPLEN);
        goto delay1000ms;

    case ATCONFIGURE_G_ECALL:
        atchannel->command.eCall_get(atchannel);
        ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_S_ECALL);
        goto delay1000ms;

    case ATCONFIGURE_G_NETMODE:
        atchannel->command.syscfgex_get(atchannel);
        ATCONFIGURE_STEP_JUMPTO(ATCONFIGURE_S_NETMODE);
        goto delay1000ms;

    default:
        break;
    }

delay100ms:
    __delaynms(timer, 100);
    return;

delay500ms:
    __delaynms(timer, 500);
    return;

delay1000ms:
    __delaynms(timer, 1000);
    return;

delay3000ms:
    __delaynms(timer, 3000);
    return;
}

bool determine_simswitch(void)
{
    return (atconfigure_step == ATCONFIGURE_HOTPLUS) ? true : false;
}
