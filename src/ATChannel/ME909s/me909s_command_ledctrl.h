#ifndef ME909S_COMMAND_LEDCTRL_
#define ME909S_COMMAND_LEDCTRL_

#include "me909s_basic.h"

/*
 * AT^LEDCTRL-Control LED GPIO PIN
 * 
 * Command Syntax:
 *      @ AT^LEDCTRL=<mode>[,<stat>,<index>[,<ON_duration1>,<OFF_duration1>[,<ON_duration2>,<OFF_duration2>]]]
 *      @ AT^LEDCTRL?
 *      @ AT^LEDCTRL=?
 * 
 * Interface Description:
 *      @desc: The set command controls the blinking mode for modules:
 *             When the <mode> is set to 0, the blinking function of modules is disabled;
 *             when the <mode> is set to 1, Huawei's default blinking mode is used;
 *             Users can set the blinking mode for modules in different service states using this command.
 *             The read command queries the blinking mode of the current module.
 *             The test command returns the supported parameters.
 * 
 * Parameter Description:
 *      <mode>: indicates blinking mode.
 *          0  Do not blink. (default value)
 *          1  Use Huawei's default blinking mode.
 *          2  Use the customized blinking mode.
 *      <stat>: indicates service status. It is a 32-bit hexadecimal number. One binary digit
 *              corresponds to a service state. The values are listed in the following table.
 *      |-------------------------------------------------------------------------------------------|
 *      | Parameter Value   | Service Status                                                        |
 *      |-------------------------------------------------------------------------------------------|
 *      | 00000001          | Airplane mode                                                         |
 *      | 00000002          | Power-on and initiation                                               |
 *      | 00000004          | Registration failure and no service                                   |
 *      | 00000008          | Network disconnection during connecting                               |
 *      | 00000010          | Registration success in GSM mode                                      |
 *      | 00000020          | Radio bearer establishment success in GSM/GPRS/EDGE mode              |
 *      | 00000040          | Data transmission in GSM/GPRS/EDGE mode                               |
 *      | 00000080          | Registration success in WCDMA mode                                    |
 *      | 00000100          | Radio bearer establishment success in WCDMA mode                      |
 *      | 00000200          | Data transmission in WCDMA mode                                       |
 *      | 00000400          | Radio bearer establishment success in HSDPA/HSUPA/HSPA+/DC-HSPA+ mode |
 *      | 00000800          | Data transmission in HSDPA/HSUPA/HSPA+/DC-HSPA+ mode                  |
 *      | 00001000          | Registration success in LTE mode                                      |
 *      | 00002000          | Dial-up success in LTE mode                                           |
 *      | 00004000          | Data transmission in LTE mode                                         |
 *      | 3FFFFFFF          | All service states                                                    |
 *      |-------------------------------------------------------------------------------------------|
 *      <index>: indicates GPIO pin of the LED indicator. The length is 8 bits and the valid value
 *               ranges from 1 to 7.
 *               The least significant 3 bits (Bit [2-0]) indicate the GPIO pin configuration of the LED
 *               indicator.
 *          0  Deactivate the corresponding GPIO pin of the LED indicator and the pin was 
 *             pulled down during a blinking period.
 *          1  Activate the corresponding GPIO pin of the LED indicator and the pin was pulled
 *             up or down based on the configured blinking mode during a blinking period.
 *      |----------------------------------------------------------------------------------------|
 *      | Bit 7     | Bit 6     | Bit 5     | Bit 4     | Bit 3     | Bit 2  | Bit 1  | Bit 0    |
 *      |----------------------------------------------------------------------------------------|
 *      | Reserved  | Reserved  | Reserved  | Reserved  | Reserved  | Pin 3  | Pin 2  | Pin 1    |
 *      |----------------------------------------------------------------------------------------|
 *      <ON_duration1>: an integer value type that indicates the duration for first pulling up the
 *                      GPIO pin of the LED indicator specified by the <index> during a blinking period. 
 *                      The value ranges from 1 to 100 (unit: 100 ms).
 *      <OFF_duration1>: an integer value type that indicates the duration for first pulling down the
 *                      GPIO pin of the LED indicator specified by the <index> during a blinking period. 
 *                      The value ranges from 1 to 100 (unit: 100 ms).
 *      <ON_duration2>: an integer value type that indicates the duration for second pulling up the
 *                      GPIO pin of the LED indicator specified by the <index> during a blinking period. 
 *                      The value ranges from 1 to 100 (unit: 100 ms).
 *      <OFF_duration2>: an integer value type that indicates the duration for second pulling down
 *                      the GPIO pin of the LED indicator specified by the <index> during a blinking period. 
 *                      The value ranges from 1 to 100 (unit: 100 ms).    
 * 
 * Example:
 *   @1 Run:        AT^LEDCTRL=2,00000001,1,1,19
 *      Response:   OK
 *                  
 *   @2 Run:        AT^LEDCTRL?
 *      Response:   ^LEDCTRL: 2,00000001,1,1,19
 *                  OK
 * 
 *   @3 Run:        AT^LEDCTRL=?
 *      Response:   ^LEDCTRL:
 *                  (0-2),00007FFF,(0-1),(1-100),(1-100),(1-100),(1-100)
 *                  OK
 */

#define ME909S_CMD_LEDCTRL              "AT^LEDCTRL"
#define ME909S_CMD_LEDCTRL_RESPONSE     "^LEDCTRL:"
#define ME909S_CMD_LEDCTRL_SIZE         64

typedef struct me909s_ledctrl_s me909s_ledctrl_t;

struct me909s_ledctrl_s 
{
    uint8_t mode;
    uint32_t stat;
    uint8_t index;
    uint8_t ON_duration1;
    uint8_t OFF_duration1;
    uint8_t ON_duration2;
    uint8_t OFF_duration2;
};

static inline void me909s_ledctrl_decode(const char *dat, me909s_ledctrl_t *ledctrl)
{
    if (!dat)
        return;

    if (!ledctrl)
        return;        

    int i;
    char *element = NULL;
    char *vail = NULL;
    unsigned char tm;

    if ((vail = (char*)strstr(dat, ME909S_CMD_LEDCTRL_RESPONSE)) == NULL)
        return;

    element = strtok(vail, ":\r\n,");

    for (i = 1; element; i++)
    {
        element = strtok(NULL, ":\r\n,");

        if (!element)
            return;

        switch (i)
        {
        case 1:
            tm = atoi(element);
            if (tm != 2) {
                memset(ledctrl, 0, sizeof(me909s_ledctrl_t));
                ledctrl->mode = tm;
                return;
            }
            ledctrl->mode = tm;
            break;

        case 2:
            if (ledctrl->mode == 2)
                ledctrl->stat = hexadecimal_to_uint32(element);
            break;

        case 3:
            if (ledctrl->mode == 2)
                ledctrl->index = atoi(element);
            break;

        case 4:
            if (ledctrl->mode == 2)
                ledctrl->ON_duration1 = atoi(element);
            break;

        case 5:
            if (ledctrl->mode == 2)
                ledctrl->OFF_duration1 = atoi(element);
            break;

        case 6:
            if (ledctrl->mode == 2)
                ledctrl->ON_duration2 = atoi(element);
            break;

        case 7:
            if (ledctrl->mode == 2)
                ledctrl->OFF_duration2 = atoi(element);
            break;

        default:
            break;                
        }
    }
}

static inline bool is_mode_vail(unsigned char mode)
{
    return (mode >= 0 && mode <= 2);
}

static inline bool is_stat_vail(unsigned int stat)
{
    switch (stat)
    {
    case 0x00000001:
    case 0x00000002:
    case 0x00000004:
    case 0x00000008:
    case 0x00000010:
    case 0x00000020:
    case 0x00000040:
    case 0x00000080:
    case 0x00000100:
    case 0x00000200:
    case 0x00000400:
    case 0x00000800:
    case 0x00001000:
    case 0x00002000:
    case 0x00004000:
    case 0x3FFFFFFF:
        return true;

    default:
        return false;
    }
}

static inline bool is_index_vail(unsigned char index)
{
    return (index >= 0 && index <= 1);
}

static inline int me909s_ledctrl_code(char *cmd, me909s_cmd_motion_t motion, 
                    unsigned char mode, unsigned int stat, unsigned char index, 
                    unsigned char ON_duration1, unsigned OFF_duration1,
                    unsigned char ON_duration2, unsigned OFF_duration2)
{
    int r;

    if (!cmd)
        return -1;

    if (false == is_mode_vail(mode))
        return -1;

    if (mode == 2 && (false == is_stat_vail(stat) || false == is_index_vail(index)))
        return -1;

    switch (motion)
    {
    case CMD_MOTION_SETUP:
        if (mode != 2)
        {
            r = sprintf(cmd, "%s=%u\r\n", ME909S_CMD_LEDCTRL, mode);
        }
        else 
        {
            if ((ON_duration1 >= 1 && ON_duration1 <= 100) 
                && (OFF_duration1 >= 1 && OFF_duration1 <= 100))
            {
                if ((ON_duration2 >= 1 && ON_duration2 <= 100) 
                && (OFF_duration2 >= 1 && OFF_duration2 <= 100))
                    r = sprintf(cmd, "%s=%u,%08X,%u,%u,%u,%u,%u", ME909S_CMD_LEDCTRL, mode, stat, index, \
                            ON_duration1, OFF_duration1, ON_duration2, OFF_duration2);
                else
                    r = sprintf(cmd, "%s=%u,%08X,%u,%u,%u", ME909S_CMD_LEDCTRL, mode, stat, index, \
                            ON_duration1, OFF_duration1);
            }
            else
            {
                mode = 1;
                r = sprintf(cmd, "%s=%u\r\n", ME909S_CMD_LEDCTRL, mode);
            }
        }
        break;

    case CMD_MOTION_INQUIRE:
        r = sprintf(cmd, "%s?\r\n", ME909S_CMD_LEDCTRL);
        break;

    default:
        break;
    }

    return r;
}

static inline void me909s_ledctrl_debug(me909s_ledctrl_t *ledctrl)
{
    ME909S_DEBUG_PRINTF(("\n\n%s\n", ME909S_CMD_LEDCTRL));
    ME909S_DEBUG_PRINTF(("**********************************************************\n"));
    ME909S_DEBUG_PRINTF(("mode:\t%u\n", ledctrl->mode));
    ME909S_DEBUG_PRINTF(("stat:\t%08X\n", ledctrl->stat));
    ME909S_DEBUG_PRINTF(("index:\t%u\n", ledctrl->index));
    ME909S_DEBUG_PRINTF(("ON_duration1:\t%u\n", ledctrl->ON_duration1));
    ME909S_DEBUG_PRINTF(("OFF_duration1:\t%u\n", ledctrl->OFF_duration1));
    ME909S_DEBUG_PRINTF(("ON_duration2:\t%u\n", ledctrl->ON_duration2));
    ME909S_DEBUG_PRINTF(("OFF_duration2:\t%u\n", ledctrl->OFF_duration2));
    ME909S_DEBUG_PRINTF(("**********************************************************\n\n"));
}

#endif
