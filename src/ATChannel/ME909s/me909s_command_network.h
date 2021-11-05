#ifndef ME909S_COMMAND_NETWORK_H_
#define ME909S_COMMAND_NETWORK_H_

#include "me909s_basic.h"

/*
 * AT+CREG-Register Network
 * 
 * Command Syntax:
 *      @ AT+CREG=[<n>]
 *      @ AT+CREG?
 *      @ AT+CREG=?
 * 
 * Interface Description:
 *      @desc: The set command controls the presentation of an unsolicited result code +CREG.
 *             The read command returns the current registration status <stat>. Location information
 *             elements <lac>, <ci> and <AcT> are returned only when <n>=2.
 * 
 * Parameter Description:
 *      <n>:
 *       0  Disable network registration unsolicited result code +CREG. (default value)
 *       1  Enable network registration unsolicited result code +CREG: <stat>.
 *       2  Enable network registration and location information unsolicited result code
 *          +CREG: <stat>[,<lac>,<ci>[,<AcT>]]
 *      <stat>:
 *       0  Not registered, MS is not currently searching for a new operator to register with.
 *       1  Registered, home network.
 *       2  Not registered, but MS is currently searching for a new operator to register with.
 *       3  Registration denied.
 *       4  Unknown.
 *       5  Registered, roaming.
 *      <lac>: a string type value that indicates two byte location area code or tracking area code in 
 *             hexadecimal format (e.g. "00C3" equals 195 in decimal).
 *      <ci>: a string type value that indicates four byte serving cell ID in hexadecimal format.
 *      <AcT>: an integer type value that indicates access technology of the serving cell.
 *       0  GSM
 *       1  GSM Compact
 *       2  UTRAN
 *       3  GSM w/EGPRS [1]
 *       4  UTRAN w/HSDPA [2]
 *       5  UTRAN w/HSUPA [2]
 *       6  UTRAN w/HSDPA and HSUPA [2]
 *       7  E-UTRAN
 * 
 * Example:
 *   @1 Run:        AT+CREG=1
 *      Response:   OK
 * 
 *   @2 Run:        AT+CREG? 
 *      Response:   +CREG: 1,1
 *                  OK
 * 
 *   If the location area code is 0x2513, the cell ID is 0x E01F4, and the network mode is
 *      WCDMA, the terminal will receive the unsolicited reports:
 * 
 *   @3 Run:        AT+CREG?
 *      Response:   +CREG: 2,1,"2513","E01F4",2
 *                  OK
 */

#define ME909S_CMD_CREG             "AT+CREG"
#define ME909S_CMD_CREG_RESPONSE    "+CREG:"
#define ME909S_CMD_CREG_SIZE        16

typedef struct me909s_creg_s me909s_creg_t;

enum me909s_creg_stat_e {
    CREG_STAT_NOTREG_NOTSEARCH  = 0,
    CREG_STAT_REGISTED_LOCALNET = 1,
    CREG_STAT_NOTREG_INSEARCH   = 2,
    CREG_STAT_REGDENIED         = 3,
    CREG_STAT_UNKNOW            = 4,
    CREG_STAT_REGISTED_ROAMNET  = 5
};

struct me909s_creg_s 
{
    uint8_t n;
    enum me909s_creg_stat_e stat;
};

static inline void me909s_creg_decode(const char *dat, me909s_creg_t *creg)
{
    if (!dat)
        return;

    if (!creg)
        return;        

#if 0
    int i;
    char *element = NULL;
    char *vail = NULL;

    if ((vail = (char*)strstr(dat, ME909S_CMD_CREG_RESPONSE)))
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
                creg->n = atoi(element);
                break;

            case 2:
                creg->stat = atoi(element);
                return;

            default:
                break;                
            }
        }
    }
#endif

    int n = 0;
    int stat = 0;

    sscanf(dat, "+CREG: %d,%d", &n, &stat);

    creg->n = (uint8_t)n;
    creg->stat = (enum me909s_creg_stat_e)stat;

    return;
}

static inline int me909s_creg_code(char *cmd, me909s_cmd_motion_t motion, unsigned char n)
{
    int r;

    if (!cmd)
        return -1;

    switch (motion)
    {
    case CMD_MOTION_SETUP:
        r = sprintf(cmd, "%s=%u\r\n", ME909S_CMD_CREG, n ? 1 : 0);
        break;

    case CMD_MOTION_INQUIRE:
        r = sprintf(cmd, "%s?\r\n", ME909S_CMD_CREG);
        break;

    default:
        break;
    }

    return r;
}

static inline void me909s_creg_debug(me909s_creg_t *creg)
{
    ME909S_DEBUG_PRINTF(("\n\n%s\n", ME909S_CMD_CREG));
    ME909S_DEBUG_PRINTF(("**********************************************************\n"));
    ME909S_DEBUG_PRINTF(("n:\t%u\n", creg->n));
    ME909S_DEBUG_PRINTF(("stat:\t%d\n", creg->stat));
    ME909S_DEBUG_PRINTF(("**********************************************************\n\n"));
}

#endif
