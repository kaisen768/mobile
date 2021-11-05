#ifndef ME909S_COMMAND_SIMSWITCH_H_
#define ME909S_COMMAND_SIMSWITCH_H_

#include "me909s_basic.h"

/*
 * AT^SIMSWITCH-Switch the SIM Card
 * 
 * Command Syntax:
 *      @ AT^SIMSWITCH=<n>
 *      @ AT^SIMSWITCH?
 *      @ AT^SIMSWITCH=?
 * 
 * Interface Description:
 *      @desc: The set command specifies which SIM card to switch.
 *             The read command queries the current SIM card selection.
 *             The test command returns the supported parameter values.
 * 
 * Parameter Description:
 *      <n>: an integer type value.
 *       0  Use SIM CARD2
 *       1  Use SIM CARD1 (default value)
 * 
 * Example:
 *   @1 Run:        AT^SIMSWITCH=0
 *      Response:   OK
 * 
 *   @2 Run:        AT^SIMSWITCH?
 *      Response:   ^SIMSWITCH: 0
 *                  OK
 * 
 *   @3 Run:        AT^SIMSWITCH=?
 *      Response:   ^SIMSWITCH: (0,1)
 *                  OK
 */

#define ME909S_CMD_SIMSWITCH             "AT^SIMSWITCH"
#define ME909S_CMD_SIMSWITCH_RESPONSE    "^SIMSWITCH: "
#define ME909S_CMD_SIMSWITCH_SIZE        24

typedef struct me909s_simswitch_s me909s_simswitch_t;

struct me909s_simswitch_s 
{
    uint8_t n;
};

static inline void me909s_simswitch_decode(const char *dat, me909s_simswitch_t *simswitch)
{
    if (!dat)
        return;

    if (!simswitch)
        return;

    int simsw = 1;

    sscanf(dat, "+SIMSWITCH: %d", &simsw);

    simswitch->n = (uint8_t)simsw;

    return;
}

static inline int me909s_simswitch_code(char *cmd, me909s_cmd_motion_t motion, unsigned char n)
{
    int r;

    if (!cmd)
        return -1;

    switch (motion)
    {
    case CMD_MOTION_SETUP:
        r = sprintf(cmd, "%s=%u\r\n", ME909S_CMD_SIMSWITCH, (n ? 1 : 0));
        break;

    case CMD_MOTION_INQUIRE:
        r = sprintf(cmd, "%s?\r\n", ME909S_CMD_SIMSWITCH);
        break;

    default:
        break;
    }

    return r;
}

static inline void me909s_simswitch_debug(me909s_simswitch_t *simswitch)
{
    if (!simswitch)
        return;

    ME909S_DEBUG_PRINTF(("\n\n%s\n", ME909S_CMD_SIMSWITCH));
    ME909S_DEBUG_PRINTF(("**********************************************************\n"));
    ME909S_DEBUG_PRINTF(("switch:\t%u\n", simswitch->n));
    ME909S_DEBUG_PRINTF(("**********************************************************\n\n"));
}

#endif
