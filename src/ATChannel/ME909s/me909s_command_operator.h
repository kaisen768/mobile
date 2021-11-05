#ifndef ME909S_COMMAND_OPERATOR_H_
#define ME909S_COMMAND_OPERATOR_H_

#include "me909s_basic.h"

/*
 * AT+COPS-Select Operator
 * 
 * Command Syntax:
 *      @ AT+COPS=[<mode>[,<format>[,<oper>[,<AcT>]]]]
 *      @ AT+COPS?
 *      @ AT+COPS=?
 * 
 * Interface Description:
 *      @desc: This command queries the network state and registers network selection mode currently by the MS.
 *             The execution command selects the network automatically or manually.
 *             The read command queries the current network selection mode. If the registration is
 *             successful, the current operator information will be returned.
 *              The test command returns the list of (up to 20) operators existent in the current network.
 * 
 * Parameter Description:
 *      <mode>:
 *          0  Automatic (<oper> field is ignored).
 *          1  Manual (<oper> field shall be present, and <AcT> optionally).
 *          2  Deregister from network. (not supported currently)
 *          3  Set only <format> (for read command AT+COPS?), do not attempt registration/deregistration (<oper> and <AcT> 
 *              fields are ignored); this value is not applicable in read command response.
 *          4  Manual/automatic (<oper> field shall be present); if manual selection fails, 
 *              automatic mode (<mode>=0) is entered. (not supported currently)
 *      <oper>: string type.
 *      <format>: indicates if the format is alphanumeric or numeric; long alphanumeric format can
 *                  be upto 16 characters long and short format up to 8 characters (refer GSM MoU SE.13).
 *                  Numeric format is the GSM Location Area Identification number (refer 3GPP TS 24.008
 *                  subclause 10.5.1.3) which consists of a three BCD digit country code coded as in ITU-T
 *                  E.212 Annex A, plus a two BCD digit network code, which is administration specific;
 *                  returned <oper> shall not be in BCD format, but in IRA characters converted from BCD.
 *                  Hence the number has structure: (country code digit 3)(country code digit 2)(country code
 *                  digit 1)(network code digit 3)(network code digit 2)(network code digit 1).
 *          0  Long format alphanumeric <oper>
 *          1  Short format alphanumeric <oper>
 *          2  Numeric <oper>
 *      <stat>:
 *          0  Unknown
 *          1  Available
 *          2  Current
 *          3  Forbidden
 *      <AcT>: indicates access technology selected.
 *          0  GSM
 *          1  GSM Compact (not supported currently)
 *          2  UTRAN
 *          3  GSM w/EGPRS (not supported currently)
 *          4  UTRAN w/HSDPA (not supported currently)
 *          5  UTRAN w/HSUPA (not supported currently)
 *          6  UTRAN w/HSDPA and HSUPA (not supported currently)
 *          7  E-UTRAN
 * 
 * Example:
 *   @1 Run:        AT+COPS=?
 *      Response:   +COPS: (3,"CHN-UNICOM","UNICOM","46001",0),(3,"CHINA MOBILE","CMCC","46000",0),,(0,1,3),(0,1,2)
 *                  OK
 * 
 *   @2 Run:        AT+COPS=0
 *      Response:   OK
 *
 *   @3 Run:        AT+COPS=1,2,"46000",0
 *      Response:   OK
 */

#if 0
#define ME909S_CMD_CREG             "AT+CREG"
#define ME909S_CMD_CREG_RESPONSE    "+CREG:"

typedef struct me909s_creg_s me909s_creg_t;

struct me909s_creg_s 
{
    uint8_t n;
    uint8_t stat;
};

static inline void me909s_creg_decode(const char *dat, me909s_creg_t *creg)
{
    if (!dat)
        return;

    if (!creg)
        return;        

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
}

static inline int me909s_creg_code(char *cmd, me909s_cmd_motion_t motion, unsigned char n)
{
    int r;

    if (!cmd)
        return -1;

    switch (motion)
    {
    case CMD_MOTION_SETUP:
        r = sprintf(cmd, "%s=%u\r\n", ME909S_CMD_CREG, n == 0? n : 1);
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
    ME909S_DEBUG_PRINTF(("stat:\t%u\n", creg->stat));
    ME909S_DEBUG_PRINTF(("**********************************************************\n\n"));
}

#endif
#endif