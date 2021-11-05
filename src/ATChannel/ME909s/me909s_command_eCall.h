#ifndef ME909S_COMMAND_ECALL_H_
#define ME909S_COMMAND_ECALL_H_

#include "me909s_basic.h"

/*
 * AT^ECLCFG-Configure eCall
 * 
 * Command Syntax:
 *      @ AT^ECLCFG=<mode>[,<voc_config>[,<redial_config>]]
 *      @ AT^ECLCFG?
 *      @ AT^ECLCFG=?
 * 
 * Interface Description:
 *      @desc: The set command configures the mode that MT gets MSD data, enables or disables the mute
 *             for IVS side speaker in MSD transmission and the automatic redial function.
 *             The read command queries the current settings information.
 *             The test command returns the available value range of each parameter.
 * 
 * Parameter Description:
 *      <mode>: an integer type value indicates the mode that MT gets MSD data.
 *          0  Transparent transmission mode
 *      <voc_config>: an integer type value indicates whether to enable or disable mute for IVS side
 *                    speaker in MSD transmission.
 *          0  Disable mute and noise will be heard. (default value)
 *          1  Enable mute.
 *      <redial_config>: an integer value that indicates whether to enable or disable the automatic=redial function.
 *          0  Disable the automatic redial function.
 *          1  Enable the automatic redial function. (default value)
 * 
 * Example:
 *   @1 Run:        AT^ECLCFG=0,1,0 
 *      Response:   OK
 * 
 *   @2 Run:        AT^ECLSTART=0,0,"13987654321",1
 *      Response:   OK
 *                  ^ORIG: 1,0
 * 
 *   @3 Run:        AT^ECLCFG？
 *      Response:   ^ECLCFG: 0,1,0
 *                  OK
 */

#define ME909S_CMD_ECLCFG               "AT^ECLCFG"
#define ME909S_CMD_ECLCFG_RESPONSE      "^ECLCFG:"
#define ME909S_CMD_ECLCFG_SIZE          32

typedef struct me909s_eCall_s me909s_eCall_t;

struct me909s_eCall_s 
{
    uint8_t mode;
    uint8_t voc_config;
    uint8_t redial_config;
};

static inline void me909s_eCall_decode(const char *dat, me909s_eCall_t *eCall)
{
    if (!dat)
        return;

    if (!eCall)
        return;        

    int mode = 0;
    int voc = 0;
    int redial = 0;

    sscanf(dat, "^ECLCFG: %d,%d,%d", &mode, &voc, &redial);

    eCall->mode = mode;
    eCall->voc_config = voc;
    eCall->redial_config = redial;

    return;
}

static inline int me909s_eCall_code(char *cmd, me909s_cmd_motion_t motion, \
                    unsigned char mode, unsigned char voc, unsigned char redial)
{
    int r;

    if (!cmd)
        return -1;

    switch (motion)
    {
    case CMD_MOTION_SETUP:
        // check mode/voc/redial is available
        r = sprintf(cmd, "%s=%u,%u,%u\r\n", ME909S_CMD_ECLCFG, mode, voc, redial);
        break;

    case CMD_MOTION_INQUIRE:
        r = sprintf(cmd, "%s?\r\n", ME909S_CMD_ECLCFG);
        break;

    default:
        break;
    }

    return r;
}

static inline void me909s_eCall_debug(me909s_eCall_t *eCall)
{
    ME909S_DEBUG_PRINTF(("\n\n%s\n", ME909S_CMD_ECLCFG));
    ME909S_DEBUG_PRINTF(("**********************************************************\n"));
    ME909S_DEBUG_PRINTF(("mode:\t\t%u\n", eCall->mode));
    ME909S_DEBUG_PRINTF(("voc_config:\t%u\n", eCall->voc_config));
    ME909S_DEBUG_PRINTF(("redial_config:\t%u\n", eCall->redial_config));
    ME909S_DEBUG_PRINTF(("**********************************************************\n\n"));
}

#endif
