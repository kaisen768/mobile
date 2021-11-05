#ifndef ME909S_COMMAND_CPIN_H_
#define ME909S_COMMAND_CPIN_H_

#include "me909s_basic.h"

/*
 * AT+CPIN-Enter PIN
 * 
 * Command Syntax:
 *      @ AT+CPIN=<pin>[,<newpin>]
 *      @ AT+CPIN?
 *      @ AT+CPIN=?
 * 
 * Interface Description:
 *      @desc: The set command verifies and unblocks PIN and PIN2.
 *             The read command returns a string indicating whether 
 *             a password is required or not.
 * 
 * Parameter Description:
 *      <pin>, <newpin>: string type values of the 4-8 digits. The character allowed in <pin> and
 *      <newpin> must range from '0' to '9', otherwise, an error message is returned.
 *      <code>: a string type, without quotation marks.
 *          READY MT is not pending for any password.
 *          SIM PIN MT is waiting for UICC/SIM PIN to be given.
 *          SIM PUK MT is waiting for UICC/SIM PUK to be given to unblock the blocked
 *          SIM PIN.
 *          SIM PIN2 MT is waiting for SIM PIN2 to be given.
 *          SIM PUK2 MT is waiting for UICC/SIM PUK2 to be given to unblock the blocked
 *          SIM PIN2.
 * 
 * Example:
 *   @1 Run:        AT+CPIN?
 *      Response:   +CPIN: SIM PUK2
 *                  OK
 * 
 *   @2 Run:        AT+CPIN="87654321","5678"  
 *      Response:   OK
 * 
 *   @3 Run:        AT+CPIN?
 *      Response:   +CPIN: READY
 *                  OK
 * 
 *   @4 Run:        AT+CPIN=?
 *      Response:   OK
 */

#define ME909S_CMD_CPIN             "AT+CPIN"
#define ME909S_CMD_CPIN_RESPONSE    "+CPIN:"
#define ME909S_CMD_CPIN_SIZE        64

typedef struct me909s_cpin_s me909s_cpin_t;

struct me909s_cpin_s 
{
    uint8_t code[16];
};

static inline void me909s_cpin_decode(const char *dat, me909s_cpin_t *cpin)
{
    if (!dat)
        return;

    if (!cpin)
        return;

    sscanf(dat, "+CPIN: %s", cpin->code);
    return;
}

static inline int me909s_cpin_code(char *cmd, me909s_cmd_motion_t motion, unsigned int pin, unsigned int new_pin)
{
    int r;

    if (!cmd)
        return -1;

    switch (motion)
    {
    case CMD_MOTION_SETUP:
        r = sprintf(cmd, "%s=\"%u\",\"%u\"\r\n", ME909S_CMD_CPIN, pin, new_pin);
        break;

    case CMD_MOTION_INQUIRE:
        r = sprintf(cmd, "%s?\r\n", ME909S_CMD_CPIN);
        break;

    default:
        break;
    }

    return r;
}

static inline void me909s_cpin_debug(me909s_cpin_t *cpin)
{
    if (!cpin)
        return;

    ME909S_DEBUG_PRINTF(("\n\n%s\n", ME909S_CMD_CPIN));
    ME909S_DEBUG_PRINTF(("**********************************************************\n"));
    ME909S_DEBUG_PRINTF(("code:\t%s\n", cpin->code));
    ME909S_DEBUG_PRINTF(("**********************************************************\n\n"));
}

#endif
