#ifndef ME909S_COMMAND_URC_SYSSTART_H_
#define ME909S_COMMAND_URC_SYSSTART_H_

#include "me909s_basic.h"

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
 *  | ^SYSSTART     | Unsolicitedly Report Module Startup                                       |
 *  |-------------------------------------------------------------------------------------------|
 * 
 * 
 * ^SYSSTART
 * 
 * Unsolicitedly Report Module Startup
 * 
 */

#define ME909S_CMD_URC_SYSSTART_RESPONSE   "^SYSSTART"

static inline void me909s_urc_sysstart_debug(void)
{
    ME909S_DEBUG_PRINTF(("\n\n%s\n", ME909S_CMD_URC_SYSSTART_RESPONSE));
    ME909S_DEBUG_PRINTF(("**********************************************************\n"));
    ME909S_DEBUG_PRINTF(("ATChannel Device System Is Ready !!!\n"));
    ME909S_DEBUG_PRINTF(("**********************************************************\n\n"));
}

#endif
