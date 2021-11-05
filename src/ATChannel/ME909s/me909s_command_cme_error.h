#ifndef ME909S_COMMAND_CME_ERROR_H_
#define ME909S_COMMAND_CME_ERROR_H_

#include "me909s_basic.h"

/** 
 * General CME Error List
 * 
 * The following describes the mapping between numeric mode and verbose mode.
 * 
 * |-------------------------------------------------------------------------------------------|
 * | Numberic mode | Verbose mode                                                              |
 * |---------------|---------------------------------------------------------------------------|
 * | 0             | phone failure                                                             |
 * | 1             | no connection to phone                                                    |
 * | 2             | phone adaptor link reserved                                               |
 * | 3             | operation not allowed                                                     |
 * | 4             | operation not supported                                                   |
 * | 5             | PH-SIM PIN required                                                       |
 * | 6             | PH-FSIM PIN required                                                      |
 * | 7             | PH-FSIM PUK required                                                      |
 * | 10            | SIM not inserted (not supported currently. return SIM failure)            |
 * | 11            | SIM PIN required                                                          |
 * | 12            | SIM PUK required                                                          |
 * | 13            | SIM failure                                                               |
 * | 14            | SIM busy                                                                  |
 * | 15            | SIM wrong                                                                 |
 * | 16            | incorrect password                                                        |
 * | 17            | SIM PIN2 required                                                         |
 * | 18            | SIM PUK2 required                                                         |
 * | 20            | memory full                                                               |
 * | 21            | invalid index                                                             |
 * | 22            | not found                                                                 |
 * | 23            | memory failure                                                            |
 * | 24            | text string too long                                                      |
 * | 25            | invalid characters in text string                                         |
 * | 26            | dial string too long                                                      |
 * | 27            | invalid characters in dial string                                         |
 * | 30            | no network service                                                        |
 * | 31            | network timeout                                                           |
 * | 32            | network not allowed - emergency calls only                                |
 * | 40            | network personalization PIN required                                      |
 * | 41            | network personalization PUK required                                      |
 * | 42            | network subset personalization PIN required                               |
 * | 43            | network subset personalization PUK required                               |
 * | 44            | service provider personalization PIN required                             |
 * | 45            | service provider personalization PUK required                             |
 * | 46            | corporate personalization PIN required                                    |
 * | 47            | corporate personalization PUK required                                    |
 * | 48            | hidden key required                                                       |
 * | 49            | EAP method not supported                                                  |
 * | 50            | Incorrect parameters                                                      |
 * | 51            | Parameter length error for all Auth commands                              |
 * | 52            | Temporary error for all auth cmds                                         |
 * | 100           | unknown                                                                   |
 * | 103           | Illegal Mem_Store                                                         |
 * | 106           | Illegal ME                                                                |
 * | 107           | GPRS services not allowed                                                 |
 * | 111           | PLMN not allowed                                                          |
 * | 112           | Location area not allowed                                                 |
 * | 113           | Roaming not allowed in this location area                                 |
 * | 132           | service option not supported                                              |
 * | 133           | requested service option not subscribed                                   |
 * | 134           | service option temporarily out of order (#34)                             |
 * | 148           | unspecified GPRS error                                                    |
 * | 149           | PDP authentication failure                                                |
 * | 150           | invalid mobile class                                                      |
 * | 257           | network rejected request                                                  |
 * | 258           | retry operation                                                           |
 * | 259           | invalid deflected to number                                               |
 * | 260           | deflected to own number                                                   |
 * | 261           | unknown subscriber                                                        |
 * | 262           | service not available                                                     |
 * | 263           | unknown class                                                             |
 * | 264           | unknown network message                                                   |
 * | 273           | Minimum TFT per PDP address error                                         |
 * | 274           | Duplicate TFT eval prec index                                             |
 * | 275           | Invalid TFT param combination                                             |
 * | 323           | Parameters error                                                          |
 * |-------------------------------------------------------------------------------------------|
 * 
 * General CME ERROR Codes (Huawei proprietary)
 * |-------------------------------------------------------------------------------------------|
 * | Numberic mode | Verbose mode                                                              |
 * |---------------|---------------------------------------------------------------------------|
 * | 65280         | call index error                                                          |
 * | 65281         | call state error                                                          |
 * | 65282         | sys state error                                                           |
 * | 65283         | parameters error                                                          |
 * | 65284         | spn file wrong                                                            |
 * | 65285         | spn file accessed denied                                                  |
 * | 65286         | spn file not exist                                                        |
 * | 65287         | another SPN query operation still not finished                            |
 * | 65289         | input value is out of range                                               |
 * | 65290         | amr file header lost                                                      |
 * |-------------------------------------------------------------------------------------------|
 * 
 * IPSTACK related CME ERROR Codes (Huawei proprietary)
 * ...
 * 
 * FOTA related CME ERROR Codes (Huawei proprietary)
 * ...
 * 
 * RSAP related "CME ERROR" Codes (Huawei proprietary)
 * ...
 */

typedef enum me909s_cme_error_e me909s_cme_error_t;

enum me909s_cme_error_e {
    CME_PHONE_FAILURE           = 0,
    CME_PHONE_NOCONNECTION      = 1,
    CME_PHONE_ADAPTOR           = 2,
    CME_OPERATION_NOTALLOWED    = 3,
    CME_OPERATION_NOTSUPPORTED  = 4,
    CME_PH_SIM_PIN_REQUIRED     = 5,
    CME_PH_FSIM_PIN_REQUIRED    = 6,
    CME_PH_FSIM_PUK_REQUIRED    = 7,
    CME_SIM_NOTINSERTED         = 10,
    CME_SIM_PIN_REQUIRED        = 11,
    CME_SIM_PUK_REQUIRED        = 12,
    CME_SIM_FAILURE             = 13,
    CME_SIM_BUSY                = 14,
    CME_SIM_WRONG               = 15,
    CME_INCORRECT_PASSWORD      = 16,
    CME_SIM_PIN2_REQUIRED       = 17,
    CME_SIM_PUK2_REQUIRED       = 18,
    CME_MEMORY_FULL             = 20,
    CME_INVALID_INDEX           = 21,
    CME_NOT_FOUND               = 22,
    CME_MEMORY_FAILURE          = 23,
    CME_TEXT_STRING_TOOLONG     = 24,
    CME_INVALID_CHARACTERS_TEXT = 25,
    CME_DIAL_STRING_TOOLONG     = 26,
    CME_INVALID_CHARACTERS_DIAL = 27,
    CME_NO_NETWORK_SERVICE      = 30,
    CME_NETWORK_TIMEOUT         = 31,
    CME_NETWORK_NOTALLOWED      = 32,
    CME_NETWORK_PPIN_REQUIRED   = 40,
    CME_NETWORK_PPUK_REQUIRED   = 41,
    CME_NETWORK_SPPIN_REQUIRED  = 42,
    CME_NETWORK_SPPUK_REQUIRED  = 43,
    CME_SERVICE_PPIN_REQUIRED   = 44,
    CME_SERVICE_PPUK_REQUIRED   = 45,
    CME_CORPORATE_PPIN_REQUIRED = 46,
    CME_CORPORATE_PPUK_REQUIRED = 47,
    CME_HIDDEN_KEY_REQUIRED     = 48,
    CME_EAP_METHOD_NOTSUPPORTED = 49,
    CME_INCORRECT_PARAMETERS    = 50,
    CME_PARAMETER_LEN_ERROR     = 51,
    CME_TEMPORARY_ERROR         = 52,
    CME_UNKNOWN                 = 100,
    CME_INITALIZE               = 65535
};

#define ME909S_CMD_CME_ERROR_RESPONSE   "+CME ERROR:"

static inline void me909s_cme_error_decode(const char *dat, me909s_cme_error_t *cme)
{
    if (!dat)
        return;

    if (!cme)
        return;

    int code = 0;

    sscanf(dat, "+CME ERROR: %d", &code);

    *cme = (me909s_cme_error_t)code;

    return;
}

static inline void me909s_cme_error_debug(me909s_cme_error_t *cme)
{
    if (!cme)
        return;

    ME909S_DEBUG_PRINTF(("\n\n%s\n", "CME ERROR:"));
    ME909S_DEBUG_PRINTF(("**********************************************************\n"));
    ME909S_DEBUG_PRINTF(("code:\t%d\n", *cme));
    ME909S_DEBUG_PRINTF(("**********************************************************\n\n"));
}

#endif
