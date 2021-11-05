#ifndef ME909S_COMMAND_URC_SIMST_H_
#define ME909S_COMMAND_URC_SIMST_H_

#include "me909s_basic.h"
#include "me909s_common_types.h"

/** 
 * List of URC Commands
 * 
 * @note URC commands listed in the following table are provided only for your reference. Some URC
 * commands may be not supported by the module.
 * 
 * @#usually urc
 * 
 *  |-------------------------------------------------------------------------------------------|
 *  | URC           | Verbose mode                                                              |
 *  |---------------|---------------------------------------------------------------------------|
 *  | +CEREG        | Notify the current LTE registration status                                |
 *  | ^HCSQ         | Report system mode and Signal Strength                                    |
 *  | ^IPSRVST      | Unsolicitedly Report the Socket Service State                             |
 *  | ^IPSTATE      | Indicate TCP/UDP data link state                                          |
 *  | ^SIMST        | SIM Card State Change Indication                                          |
 *  | ^SRVST        | Service State Change Indication                                           |
 *  | ^ECCLIST      | (Only for the HUAWEI specified client)                                    |
 *  | ^MODE         | RSSI or System Mode Change Indication (be replaced by the URC "^HCSQ")    |
 *  |-------------------------------------------------------------------------------------------|
 * 
 * 
 * ^SIMST
 * 
 * SIM Card State Change Indication
 * 
 */

#define ME909S_CMD_URC_SIMST_RESPONSE   "^SIMST: "

static inline void me909s_urc_simst_decode(const char *dat, me909s_sim_state_t *sim)
{
    if (!dat)
        return;

    if (!sim)
        return;

    int state = 0;

    sscanf(dat, "^SIMST: %d", &state);

    *sim = (me909s_sim_state_t)state;

    return;
}

static inline void me909s_urc_simst_debug(me909s_sim_state_t *sim)
{
    if (!sim)
        return;

    ME909S_DEBUG_PRINTF(("\n\n%s\n", "SIMST:"));
    ME909S_DEBUG_PRINTF(("**********************************************************\n"));
    ME909S_DEBUG_PRINTF(("state:\t%d\n", *sim));
    ME909S_DEBUG_PRINTF(("**********************************************************\n\n"));
}

#endif
