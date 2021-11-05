#ifndef ME909S_COMMAND_SYSINFOEX_H_
#define ME909S_COMMAND_SYSINFOEX_H_

#include "me909s_basic.h"

/*
 * AT^SYSINFOEX-Query Extended System Information
 * 
 * Command Syntax:  
 *      @ AT^SYSINFOEX
 *      @ <CR><LF>^SYSINFOEX:
 *        <srv_status>,<srv_domain>,<roam_status>,<sim_state>,<lock_state>,<sysmode>,
 *        <sysmode_name>,<submode>,<submode_name><CR><LF><CR><LF>OK<CR><LF>
 *      
 * Interface Description:
 *      @desc: This command queries the current system information, such as the system service status, 
 *             domain, roaming status, system mode, and SIM card state.
 * 
 * Parameter Description:
 *      <srv_status>: indicates the system service status.
 *          0 No services
 *          1 Restricted services
 *          2 Valid services
 *          3 Restricted regional services
 *          4 Power saving or hibernate state
 *      <srv_domain>: indicates the system service domain.
 *          0 No services
 *          1 CS service only
 *          2 PS service only
 *          3 PS+CS services
 *          4 Not registered to CS or PS; searching now
 *          255 CDMA (not supported currently)
 *      <roam_status>: indicates the roaming status.
 *          0 Not roaming
 *          1 Roaming 
 *      <sim_state>: indicates the state of the SIM card.
 *          0 Invalid SIM card
 *          1 Valid SIM card
 *          2 Invalid SIM card in CS
 *          3 Invalid SIM card in PS
 *          4 Invalid SIM card in PS and CS
 *          240 ROMSIM version
 *          255 No SIM card is found
 *      <lock_state>: indicates whether the SIM card is locked by the CardLock feature. (notsupported currently)
 *          0 SIM card is not locked by the CardLock feature.
 *          1 SIM card is locked by the CardLock feature.
 *      <sysmode>: indicates the system mode.
 *          0 NO SERVICE
 *          1 GSM
 *          2 CDMA (not supported currently)
 *          3 WCDMA
 *          4 TD-SCDMA
 *          5 WiMAX (not supported currently)
 *          6 LTE
 *      <sysmode_name>: a string type value that indicates the system mode name corresponding to
 *                      <sysmode>. For example, if <sysmode>=3, <sysmode_name>="WCDMA".
 *      <submode>: indicates the system sub-mode.
 *          0 NO SERVICE
 *          1 GSM
 *          2 GPRS
 *          3 EDGE
 *          4-20 (No defined)
 *          21 IS95A
 *          22 IS95B
 *          23 CDMA2000 1X
 *          24 EVDO Rel0
 *          25 EVDO RelA
 *          26 EVDO RelB
 *          27 Hybrid (CDMA2000 1X)
 *          28 Hybrid (EVDO Rel0)
 *          29 Hybrid (EVDO RelA)
 *          30 Hybrid (EVDO RelB)
 *          31 eHRPD Rel0
 *          32 eHRPD RelA
 *          33 eHRPD RelB
 *          34 Hybrid (eHRPD Rel0)
 *          35 Hybrid (eHRPD RelA)
 *          36 Hybrid (eHRPD RelB)
 *          37-39 Reserved
 *          41 WCDMA
 *          42 HSDPA
 *          43 HSUPA
 *          44 HSPA
 *          45 HSPA+
 *          46 DC-HSPA+
 *          47-60 Reserved
 *          61 TD-SCDMA
 *          62 HSDPA    
 *          63 HSUPA
 *          64 HSPA
 *          65 HSPA+
 *          66-80 Reserved
 *          81 802.16e
 *          82-100 Reserved
 *          101 LTE
 *          Other value Reserved
 *      <submode_name>: indicates system sub-mode. (Its value can be extended.)
 * 
 * Example:
 *      Run:        AT^SYSINFOEX
 *      Response:   ^SYSINFOEX:
 *                  2,3,0,1,,3,"WCDMA",41,"WCDMA"
 *                  OK
 */

#define ME909S_CMD_SYSEXINFO            "AT^SYSINFOEX"
#define ME909S_CMD_SYSEXINFO_RESPONSE   "^SYSINFOEX:"

typedef struct me909s_sysinfoex_s me909s_sysinfoex_t;

struct me909s_sysinfoex_s 
{
    uint8_t srv_status;
    uint8_t srv_domain;
    uint8_t roam_status;
    uint8_t sim_state;
    uint8_t lock_state;
    uint8_t sysmode;
    uint8_t sysmode_name[32];
    uint8_t submode;
    uint8_t submode_name[32];
};

static inline void me909s_sysinfoex_decode(const char *dat, me909s_sysinfoex_t *sysinfoex)
{
    if (!dat)
        return;

    if (!sysinfoex)
        return;

    int i;
    char *element = NULL;
    char *vail = NULL;

    if ((vail = (char*)strstr(dat, ME909S_CMD_SYSEXINFO_RESPONSE)))
    {
        element = strtok(vail, ":\r\n,");
    
        for (i = 1; element; i++)
        {
            element = strtok(NULL, ":\r\n,");

            if (!element)
                return;

            switch (i)
            {
            case 1:
                sysinfoex->srv_status = atoi(element);
                break;
            
            case 2:
                sysinfoex->srv_domain = atoi(element);
                break;

            case 3:
                sysinfoex->roam_status = atoi(element);
                break;

            case 4:
                sysinfoex->sim_state = atoi(element);
                break;

            /*   no support
            case 5:
                sysinfoex->lock = atoi(element);
                break;
            */

            case 5:
                sysinfoex->sysmode = atoi(element);
                break;

            case 6:
                me909s_strcopy((char*)sysinfoex->sysmode_name, element, strlen(element));
                break;

            case 7:
                sysinfoex->submode = atoi(element);
                break;

            case 8:
                me909s_strcopy((char*)sysinfoex->submode_name, element, strlen(element));
                return;

            default:
                break;
            }    
        }
    
    }

}

static inline int me909s_sysinfoex_code(char *cmd, me909s_cmd_motion_t motion)
{
    if (!cmd)
        return -1;

    return sprintf(cmd, "%s\r\n", ME909S_CMD_SYSEXINFO);
}

static inline void me909s_sysinfoex_debug(me909s_sysinfoex_t *sysinfoex)
{
    if (!sysinfoex)
        return;

    ME909S_DEBUG_PRINTF(("\n\n%s\n", ME909S_CMD_SYSEXINFO));
    ME909S_DEBUG_PRINTF(("**********************************************************\n"));
    ME909S_DEBUG_PRINTF(("srv_status:\t%d\n", sysinfoex->srv_status));
    ME909S_DEBUG_PRINTF(("srv_domain:\t%d\n", sysinfoex->srv_domain));
    ME909S_DEBUG_PRINTF(("roam_status:\t%d\n", sysinfoex->roam_status));
    ME909S_DEBUG_PRINTF(("sim_state:\t%d\n", sysinfoex->sim_state));
    ME909S_DEBUG_PRINTF(("lock_state:\t%d\n", sysinfoex->lock_state));
    ME909S_DEBUG_PRINTF(("sysmode:\t%d\n", sysinfoex->sysmode));
    ME909S_DEBUG_PRINTF(("sysmode_name:\t%s\n", sysinfoex->sysmode_name));
    ME909S_DEBUG_PRINTF(("submode:\t%d\n", sysinfoex->submode));
    ME909S_DEBUG_PRINTF(("submode_name:\t%s\n", sysinfoex->submode_name));
    ME909S_DEBUG_PRINTF(("**********************************************************\n\n"));
}

#endif
