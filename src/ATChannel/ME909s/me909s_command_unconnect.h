#ifndef ME909S_COMMMAN_UNCONNECT_H_
#define ME909S_COMMMAN_UNCONNECT_H_

#include "me909s_basic.h"

/** 
 * ATH-Hang Up Call
 * 
 * Command Syntax:
 *      @ ATH[<value>]
 * 
 * Interface Description:
 *      @desc: This command disconnects the MT from remote users under the single mode.
 *              When a multiparty call is hung up, each connected user is disconnected. The difference
 *              between the ATH command and the AT+CHUP command is that the AT+CHUP command is
 *              used for calls of multiple modes. The AT+CHUP command is not a replacement of the ATH
 *              command. At present, the ATH command can only disconnect the data service.
 * 
 * Parameter Description:
 *      <value>: an integer type value.
 *            If <value> is 0, all users are disconnected and OK is returned.
 *            If <value> is not 0, the connection cannot be disconnected, and ERROR is returned.
 * 
 * Example:
 *      Run:        ATH
 *      Response:   OK
 *      
 *      Run:        ATH0
 *      Response:   OK
 */

#define ME909S_CMD_UNCONNECT              "ATH"
#define ME909S_CMD_UNCONNECT_SIZE         8

static inline int me909s_unconnect_code(char *cmd)
{
    if (!cmd)
        return -1;

    return sprintf(cmd, "%s\r\n", ME909S_CMD_UNCONNECT);
}

#endif
