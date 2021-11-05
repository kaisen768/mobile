#ifndef ME909S_COMMAND_IPCFL_H_
#define ME909S_COMMAND_IPCFL_H_

#include "me909s_basic.h"

/*
 * AT^IPCFL-Configure TCP/UDP Static Parameters
 * 
 * Command Syntax:
 *      @ AT^IPCFL=<mode>[,<stat>,<index>[,<ON_duration1>,<OFF_duration1>[,<ON_duration2>,<OFF_duration2>]]]
 *      @ AT^IPCFL?
 *      @ AT^IPCFL=?
 * 
 * Interface Description:
 *      @desc: This command configures static parameters related to IPSTACK, including the sending delay
 *              timer, the maximum packet length during transparent transmission, and the selection of the
 *              transparent transmission mode.
 *              The set command sets one parameter at a time. The user needs to enter parameter ID and the
 *              corresponding value.
 *              The read command queries the supported parameters and their corresponding values.
 *              Parameters that are not supported by the terminal are not returned.
 *              The test command returns the value range corresponding to the supported parameters.
 * 
 * Parameter Description:
 *      |---------------------------------------------------------------------------------------------------|
 *      | <parameter_id>   | Range of <value> (Unit)       | Description                                    |
 *      |---------------------------------------------------------------------------------------------------|
 *      | 5                | 1-100 (0.1s) (10 by default)  | Specifies the value of the timer for           |
 *      |                  |                               | the transparent transmission.                  |
 *      |------------------|-------------------------------|------------------------------------------------|
 *      | 10               | 1-1472 (byte) (1024 default)  | Specifies the length of the TCP/UDP data       |
 *      |                  |                               | packet for packet sending.                     |
 *      |------------------|-------------------------------|------------------------------------------------|
 *      | 12               | 0-1 (0 by default)            | For selecting the transparent transmission     |
 *      |                  |                               | mode, see the description of AT^IPENTRANS.     |
 *      |------------------|-------------------------------|------------------------------------------------|
 *      | 13               | 0-1 (0 by default)            | 0: The FTP/HTTP file data obtained from the    |
 *      |                  |                               |    server is transparently reported and        |
 *      |                  |                               |    forwarded to the user.                      |
 *      |                  |                               | 1: The FTP/HTTP file data obtained from the    |
 *      |                  |                               |    server adds the 0x7E end character and      |
 *      |                  |                               |    forwards to the user. (not supported )      |
 *      |------------------|-------------------------------|------------------------------------------------|
 *      | 14               | 0-2 (0 by default)            | 0: The transparent transmission mode           |
 *      |                  |                               | 1: The commend transmission mode               |
 *      |                  |                               | 2: The get buffer transmission mode            |
 *      |------------------|-------------------------------|------------------------------------------------|
 * 
 * Example:
 *   @1 Run:        AT^IPCFL=5,2
 *      Response:   OK
 */

#define ME909S_CMD_IPCFL              "AT^IPCFL"
#define ME909S_CMD_IPCFL_RESPONSE     "^IPCFL:"
#define ME909S_CMD_IPCFL_SIZE         24

typedef struct me909s_ipcfl_s me909s_ipcfl_t;
typedef enum me909s_ipclf_parameter_id me909s_ipclf_parameter_id_t;

enum me909s_ipclf_parameter_id {
    IPCFL_PARAMETER_ID_5 = 0,
    IPCFL_PARAMETER_ID_10 = 1,
    IPCFL_PARAMETER_ID_12 = 2,
    IPCFL_PARAMETER_ID_13 = 3,
    IPCFL_PARAMETER_ID_14 = 4,
    IPCFL_PARAMETER_ID_NUM,
};

struct ipcfl_parameter_iterm {
    uint8_t id;
    uint16_t value;
};

struct me909s_ipcfl_s 
{
    struct ipcfl_parameter_iterm parameter[IPCFL_PARAMETER_ID_NUM];
};

static inline int IPCFL_PARAMETER_INDEX(int id)
{
    switch (id)
    {
    case 5:
        return IPCFL_PARAMETER_ID_5;
    
    case 10:
        return IPCFL_PARAMETER_ID_10;
    
    case 12:
        return IPCFL_PARAMETER_ID_12;

    case 13:
        return IPCFL_PARAMETER_ID_13;

    case 14:
        return IPCFL_PARAMETER_ID_14;

    default:
        break;
    }
    return 0;
}

static inline void me909s_ipcfl_decode(const char *dat, me909s_ipcfl_t *ipcfl)
{
    if (!dat)
        return;

    if (!ipcfl)
        return;

    int id = 0;
    int value = 0;
    int offset;

    do
    {
        offset = 0;

        if (sscanf(dat, "^IPCFL: %d,%d", &id, &value) <= 0)
            break;

        ipcfl->parameter[IPCFL_PARAMETER_INDEX(id)].id = id;
        ipcfl->parameter[IPCFL_PARAMETER_INDEX(id)].value = value;

        while (*dat && dat[offset++] != '\n');
        dat += offset;
    } while (1);
}

static inline bool is_parameter_avail(unsigned char parameter_id, unsigned short int value)
{
    switch (parameter_id)
    {
    case 5:
        return (value >= 1 && value <= 100);

    case 10:
        return (value >= 1 && value <= 1472);

    case 12:
    case 13:
        return (value >= 0 && value <= 1);

    case 14:
        return (value >= 0 && value <= 2);

    default:
        return false;
    }
}

static inline int me909s_ipcfl_code(char *cmd, me909s_cmd_motion_t motion, unsigned char parameter_id, unsigned short int value)
{
    int r;

    if (!cmd)
        return -1;
    
    switch (motion)
    {
    case CMD_MOTION_SETUP:
        if (is_parameter_avail(parameter_id, value) == false)
            return -2;
        r = sprintf(cmd, "%s=%u,%u\r\n", ME909S_CMD_IPCFL, parameter_id, value);
        break;

    case CMD_MOTION_INQUIRE:
        r = sprintf(cmd, "%s?\r\n", ME909S_CMD_IPCFL);
        break;

    default:
        break;
    }

    return r;
}

static inline void me909s_ipcfl_debug(me909s_ipcfl_t *ipcfl)
{
    ME909S_DEBUG_PRINTF(("\n\n%s\n", ME909S_CMD_IPCFL));
    ME909S_DEBUG_PRINTF(("**********************************************************\n"));
    ME909S_DEBUG_PRINTF(("parameter[0]:\n"));
    ME909S_DEBUG_PRINTF(("id:\t%u\n", ipcfl->parameter[0].id));
    ME909S_DEBUG_PRINTF(("value:\t%u\n", ipcfl->parameter[0].value));
    ME909S_DEBUG_PRINTF(("parameter[1]:\n"));
    ME909S_DEBUG_PRINTF(("id:\t%u\n", ipcfl->parameter[1].id));
    ME909S_DEBUG_PRINTF(("value:\t%u\n", ipcfl->parameter[1].value));
    ME909S_DEBUG_PRINTF(("parameter[2]:\n"));
    ME909S_DEBUG_PRINTF(("id:\t%u\n", ipcfl->parameter[2].id));
    ME909S_DEBUG_PRINTF(("value:\t%u\n", ipcfl->parameter[2].value));
    ME909S_DEBUG_PRINTF(("parameter[3]:\n"));
    ME909S_DEBUG_PRINTF(("id:\t%u\n", ipcfl->parameter[3].id));
    ME909S_DEBUG_PRINTF(("value:\t%u\n", ipcfl->parameter[3].value));
    ME909S_DEBUG_PRINTF(("parameter[4]:\n"));
    ME909S_DEBUG_PRINTF(("id:\t%u\n", ipcfl->parameter[4].id));
    ME909S_DEBUG_PRINTF(("value:\t%u\n", ipcfl->parameter[4].value));
    ME909S_DEBUG_PRINTF(("**********************************************************\n\n"));
}

#endif
