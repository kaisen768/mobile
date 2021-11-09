#ifndef _CMDHANDLE_H_
#define _CMDHANDLE_H_

#include "atchannel.h"

static inline char *certain_command_response(const char *dat, 
            const char *cmp1, const char *cmp2, const char *cmp3)
{
    char *v;

    if (cmp1) {
        if ((v = strstr(dat, cmp1)))
            goto Result;
    }

    if (cmp2) {
        if ((v = strstr(dat, cmp2)))
            goto Result;
    }

    if (cmp3) {
        if ((v = strstr(dat, cmp3)))
            goto Result;
    }

Result:
    return v;
}

static inline void command_handle(char *dat, struct atchannel_s *atchannel)
{
    char *payload;
    shm_4Gmodule_t *me909s = atchannel->me909s;

    if (!dat)
        return;

    if ((payload = certain_command_response(dat, ME909S_CMD_HCSQ_RESPONSE, NULL, NULL))) {
        me909s_hcsq_decode(payload, &me909s->signal);
        me909s_hcsq_debug(&me909s->signal);
    }

    else if ((payload = certain_command_response(dat, ME909S_CMD_INDENTIFICATION_RESPONSE, NULL, NULL))) {
        me909s_indentification_decode(payload, &me909s->indentification);
        me909s_indentification_debug(&me909s->indentification);
    }

    else if ((payload = certain_command_response(dat, ME909S_CMD_SYSEXINFO_RESPONSE, NULL, NULL))) {
        me909s_sysinfoex_decode(payload, &me909s->sysinfoex);
        me909s_sysinfoex_debug(&me909s->sysinfoex);
        me909s_sim_state(me909s) = me909s->sysinfoex.sim_state;
        me909s_srv_state(me909s) = me909s->sysinfoex.srv_status;
    }

    else if ((payload = certain_command_response(dat, ME909S_CMD_LEDCTRL_RESPONSE, NULL, NULL))) {
        me909s_ledctrl_decode(payload, &me909s->ledctrl);
        me909s_ledctrl_debug(&me909s->ledctrl);
    }

    else if ((payload = certain_command_response(dat, ME909S_CMD_SYSCFGEX_RESPONSE, NULL, NULL))) {
        me909s_syscfgex_decode(payload, &me909s->syscfgex);
        me909s_syscfgex_debug(&me909s->syscfgex);
    }

    else if ((payload = certain_command_response(dat, ME909S_CMD_ECLCFG_RESPONSE, NULL, NULL))) {
        me909s_eCall_decode(payload, &me909s->eCall);
        me909s_eCall_debug(&me909s->eCall);
    }

    else if ((payload = certain_command_response(dat, ME909S_CMD_CREG_RESPONSE, NULL, NULL))) {
        me909s_creg_decode(payload, &me909s->network);
        me909s_creg_debug(&me909s->network);
    }

    else if ((payload = certain_command_response(dat, ME909S_CMD_MONSC_RESPONSE, NULL, NULL))) {
        me909s_monsc_decode(payload, &me909s->monsc);
        me909s_monsc_debug(&me909s->monsc);
    }

    else if ((payload = certain_command_response(dat, ME909S_CMD_IPCFL_RESPONSE, NULL, NULL))) {
        me909s_ipcfl_decode(payload, &me909s->ipcfl);
        me909s_ipcfl_debug(&me909s->ipcfl);
    }

    else if ((payload = certain_command_response(dat, ME909S_CMD_SIMSWITCH_RESPONSE, NULL, NULL))) {
        me909s_simswitch_decode(payload, &me909s->simswitch);
        me909s_simswitch_debug(&me909s->simswitch);
    }

    else if ((payload = certain_command_response(dat, ME909S_CMD_CPIN_RESPONSE, NULL, NULL))) {
        me909s_cpin_decode(payload, &me909s->cpin);
        me909s_cpin_debug(&me909s->cpin);
    }

    /* General CME Error List */
    else if ((payload = certain_command_response(dat, ME909S_CMD_CME_ERROR_RESPONSE, NULL, NULL)))
    {
        me909s_cme_error_decode(payload, &me909s->cme);
        me909s_cme_error_debug(&me909s->cme);

        /* SIM CME Error */
        if (me909s->cme == CME_SIM_NOTINSERTED)
            me909s->sim = SIM_STATE_NOINSERTED;
        else if (me909s->cme == CME_SIM_FAILURE
                || me909s->cme == CME_SIM_WRONG)
            me909s->sim = SIM_STATE_NOINSERTED;
    }

    /* URC: SIM Card State Change Indication */
    else if ((payload = certain_command_response(dat, ME909S_CMD_URC_SIMST_RESPONSE, NULL, NULL)))
    {
        me909s_urc_simst_decode(payload, &me909s->sim);
        me909s_urc_simst_debug(&me909s->sim);
    }

    /* URC: Service State Change Indication */
    else if ((payload = certain_command_response(dat, ME909S_CMD_URC_SRVST_RESPONSE, NULL, NULL)))
    {
        me909s_urc_srvst_decode(payload, &me909s->srv);
        me909s_urc_srvst_debug(&me909s->srv);
    }

    /* Reset the Module (special process) */
    else if ((payload = certain_command_response(dat, ME909S_CMD_RESET_RESPONSE, NULL, NULL)))
    {
        me909s_reset_debug();
        atchannel->reset = ATRESET_REQUEST;
        atchannel->status = ATCHANNEL_UNOPENED;
    }
}

#endif