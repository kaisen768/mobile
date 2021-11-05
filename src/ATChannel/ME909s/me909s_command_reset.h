#ifndef ME909S_COMMMAN_RESET_H_
#define ME909S_COMMMAN_RESET_H_

#include "me909s_basic.h"

/** 
 * AT^RESET-Reset the Module
 * 
 * Command Syntax:
 *      @ AT^RESET
 * 
 * Interface Description:
 *      @desc: This command is used to reset the module.
 * 
 * Parameter Description:
 *      None
 * 
 * Example:
 *      Run:        AT^RESET
 *      Response:   OK
 */

#define ME909S_CMD_RESET              "AT^RESET"
#define ME909S_CMD_RESET_RESPONSE     "AT^RESET"
#define ME909S_CMD_RESET_SIZE         16

static inline int me909s_reset_code(char *cmd)
{
    if (!cmd)
        return -1;

    return sprintf(cmd, "%s\r\n", ME909S_CMD_RESET);
}

static inline void me909s_reset_debug(void)
{
    ME909S_DEBUG_PRINTF(("\n\n%s\n", ME909S_CMD_RESET));
    ME909S_DEBUG_PRINTF(("**********************************************************\n"));
    ME909S_DEBUG_PRINTF(("ATChannel Will Reset !!!\n"));
    ME909S_DEBUG_PRINTF(("**********************************************************\n"));
}

#endif
