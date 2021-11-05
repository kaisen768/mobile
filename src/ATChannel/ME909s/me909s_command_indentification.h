#ifndef ME909S_COMMMAN_INDENTIFICATION_H_
#define ME909S_COMMMAN_INDENTIFICATION_H_

#include "me909s_basic.h"

/** 
 * ATI-Request Identification
 * 
 * Command Syntax:
 *      @ ATI[<value>]
 *      @ <CR><LF><list of MS ID info><CR><LF><CR><LF>OK<CR><LF>
 * 
 * Interface Description:
 *      @desc: The ATI command queries the ID information about the MS, including:
 *                  @Manufacturer (AT+GMI)
 *                  @Product model (AT+GMM)
 *                  @Software version (AT+GMR)
 *                  @ESN/IMEI (AT+GSN)
 *                  @Capability list (AT+GCAP)
 * 
 * Parameter Description:
 *      <value>:
 *          queries the previously described MS ID information. The value ranges from 0 to 255
 *          (these values are meaningless)
 * 
 * Example:
 *      Run:        ATI
 *      Response:   Manufacturer: Huawei Technologies Co., Ltd.
 *                  Model: ME909s-120   
 *                  Revision: 11.670.01.00.00
 *                  IMEI: 356112010004540
 *                  +GCAP: +CGSM,+DS,+ES
 *                  OK
 */

#define ME909S_CMD_INDENTIFICATION              "ATI"
#define ME909S_CMD_INDENTIFICATION_RESPONSE     "Manufacturer:"
#define ME909S_CMD_INDENTIFICATION_SIZE         8

typedef struct me909s_indentification_s me909s_indentification_t;

struct me909s_indentification_s 
{
    char manufacturer[64];
    char model[32];
    char revision[32];
    uint64_t imei;
    char gcap[64];
};

static inline void me909s_indentification_decode(const char *dat, me909s_indentification_t *indec)
{
    if (!dat || !indec)
        return;

    int i;
    char *element = NULL;
    char *vail = NULL;

    if ((vail = (char*)strstr(dat, ME909S_CMD_INDENTIFICATION_RESPONSE)))
    {
        element = strtok(vail, ":\r\n");

        for (i = 1; element; i++)
        {
            element = strtok(NULL, ":\r\n");
            if (!element)
                return;

            switch (i)
            {
            case 1:
                memcpy(indec->manufacturer, element, strlen((const char*)element));
                break;
            
            case 3:
                memcpy(indec->model, element, strlen((const char*)element));
                break;

            case 5:
                memcpy(indec->revision, element, strlen((const char*)element));
                break;

            case 7:
                indec->imei = atoll(element);
                break;

            case 9:
                memcpy(indec->gcap, element, strlen((const char*)element));
                return;

            default:
                break;
            }
        }
    }
}

static inline int me909s_indentification_code(char *cmd)
{
    if (!cmd)
        return -1;

    return sprintf(cmd, "%s\r\n", ME909S_CMD_INDENTIFICATION);
}

static inline void me909s_indentification_debug(me909s_indentification_t *indec)
{
    if (!indec)
        return;

    ME909S_DEBUG_PRINTF(("\n\n%s\n", ME909S_CMD_INDENTIFICATION));
    ME909S_DEBUG_PRINTF(("**********************************************************\n"));
    ME909S_DEBUG_PRINTF(("Manufacturer:\t%s\n", indec->manufacturer));
    ME909S_DEBUG_PRINTF(("Model:\t\t%s\n", indec->model));
    ME909S_DEBUG_PRINTF(("Revision:\t%s\n", indec->revision));
    ME909S_DEBUG_PRINTF(("IMEI:\t\t %llu\n", indec->imei));
    ME909S_DEBUG_PRINTF(("GCAP:\t\t%s\n", indec->gcap));
    ME909S_DEBUG_PRINTF(("**********************************************************\n\n"));
}

#endif
