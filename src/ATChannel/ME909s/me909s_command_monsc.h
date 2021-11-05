#ifndef ME909S_COMMAND_MONSC_H_
#define ME909S_COMMAND_MONSC_H_

#include "me909s_basic.h"

/*
 * AT^MONSC-Query Serving Cell Information Through Network Monitor
 * 
 * Command Syntax:
 *      @ AT^MONSC
 * 
 * Interface Description:
 *      @desc: This command is used to query the parameter values of the current serving cell.
 *              If the module is using a WCDMA network, in non-DCH state, the command returns the
 *              parameters of the resident cell, and in DCH state, the command returns the parameters of the
 *              cell whose signal is the strongest among the active set. In DCH state, if you want to enable or
 *              disable the frequency lock, it is recommended to run AT^MONSC command firstly, and then
 *              run AT^FREQLOCK command.
 * 
 * Parameter Description:
 *      <RAT>: a string indicating the access technology.
 *          GSM        Reside on a GSM cell. <cell_paras> will be returned.
 *          WCDMA      Reside on a WCDMA cell. <cell_paras> will be returned.
 *          TD-SCDMA   Reside on a TD-SCDMA cell. <cell_paras> will be returned.
 *          LTE        Reside on a LTE cell. <cell_paras> will be returned.
 *      <cell_paras>: serving cell parameters. The returned parameters vary with the access technology and network condition. 
 *                    If no valid value is obtained for a parameter, the parameter will be left empty.
 *        GSM
 *          <MCC>  Mobile Country Code, which is a decimal number
 *          <MNC>  Mobile Network Code, which is a decimal number
 *          <BAND>  Frequency Band, which is a decimal number. The value range is 0–3:
 *              0: GSM 850
 *              1: GSM 900
 *              2: GSM 1800
 *              3: GSM1900
 *          <ARFCN>  Absolute Radio Frequency Channel Number of the BCCH carrier, which is a decimal number. The value range is 0–1023.
 *          <BSIC>  Base Station Identity Code, which is a decimal number. The value range is 0–63.
 *          <Cell_ID>  Cell identify in the SIB3 message, which is a hexadecimal. The value range is 0–FFFF.
 *          <LAC>  Location Area Code, which is a hexadecimal. The value range is 0–FFFF.
 *          <RXLEV>  Receiving Signal Strength in dBm, which is a decimal number. The value range is -130 to -10.
 *          <RxQuality>  Quality of Reception, which is a decimal number. This parameter is valid 
 *                      in data transmission or dedicated state. The value range is 0–7.
 *          <TA>  Timing Advance, which is a decimal number. The value range is 0–63.
 * 
 *        WCDMA
 *          <MCC>  Mobile Country Code, which is a decimal number.
 *          <MNC>  Mobile Network Code, which is a decimal number.
 *          <ARFCN>  Absolute Radio Frequency Channel Number of the BCCH carrier, which is a decimal number. The value range is 0–16383.
 *          <PSC>  Primary Scrambling Code, which is a decimal number. The value range is 0–511.
 *          <Cell_ID>  Cell identity in the SIB3 message, which is a hexadecimal and includes the RNC ID and cell ID. The value range is 0–FFFFFFFF.
 *          <LAC>  Location Area Code, which is a hexadecimal. The value range is 0–FFFF.
 *          <RSCP>  Received Signal Code Power in dBm, which is a decimal number. The value range is -130 to -20.
 *          <RXLEV>  Receiving Signal Strength in dBm, which is a decimal number. The value range is -101 to -20.
 *          <EC/N0>  Ratio of energy per modulating bit to the noise spectral density, which is a decimal number. The value range is -25 to 0.
 *          <DRX>  Discontinuous Reception Cycle Length, which is a decimal number. The value range is 6–9.
 *          <URA>  UTRAN Registration Area Identity, which is a decimal number, The value range is 0–65535.
 *          
 *        TD-SCDMA
 *          <MCC>  Mobile Country Code, which is a decimal number.
 *          <MNC>  Mobile Network Code, which is a decimal number.
 *          <ARFCN>  Absolute Radio Frequency Channel Number of the BCCH carrier, which is a decimal number. The value range is 9400-9600, 10050-10125, 11500-12000.
 *          <SYNC_ID>  Downlink pilot code, which is a decimal number. The value range is 0–31.
 *          <SC>  Scrambling code, which is a decimal number. The value range is 0–127.
 *          <Cell_ID>  Cell identity in the SIB3 message, which is a hexadecimal and includes the RNC ID and cell ID. The value range is 0–FFFFFFFF.
 *          <LAC>  Location Area Code, which is a hexadecimal. The value range is 0–FFFF.
 *          <RSCP>  Received Signal Code Power in dBm, which is a decimal number. The value range is -116 to 0.
 *          <DRX>  Discontinuous Reception Cycle Length, which is a decimal number, The value range is 6–9.
 *          <RAC>  Route area code, which is a hexadecimal number. The value range is 0–FFFF.     
 * 
 *        LTE
 *          <MCC>  Mobile Country Code, which is a decimal number.
 *          <MNC>  Mobile Network Code, which is a decimal number.
 *          <ARFCN>  Absolute Radio Frequency Channel Number of the BCCH carrier, which is a decimal number. The value range is 0–39649.
 *          <Cell_ID>  Cell identity in the SIB1 message, which is a hexadecimal and includes the RNC ID and cell ID. The value range is 0–FFFFFFFF.
 *          <PCI>  Physical cell ID, which is a decimal number, The value range is 0–503.
 *          <TAC>  Tracking Area Code, which is a hexadecimal number. The value range is 0–FFFF.
 *          <RSRP>  Reference Signal Received Power in dBm, which is a decimal number. The value range is -144 to -44 .
 *          <RSRQ>  Reference Signal Received Quality in dB, which is a decimal number, The value range is -30 to -3.
 *          <RXLEV>  Receiving Signal strength in dBm, which is a decimal number, The value range is -110 to -20.
 * 
 * Example:
 *   @1 Run:        AT^MONSC
 *      Response:   ^MONSC: WCDMA,472,99,10738,304,120514,2513,-79,-73,-6,6,0
 *                  OK
 */

#define ME909S_CMD_MONSC             "AT^MONSC"
#define ME909S_CMD_MONSC_RESPONSE    "^MONSC:"
#define ME909S_CMD_MONSC_SIZE        16

typedef struct me909s_monsc_s me909s_monsc_t;
typedef enum me909s_monsc_rat_e me909s_monsc_rat_t;
typedef struct monsc_cell_paras_s monsc_cell_paras_t;

enum me909s_monsc_rat_e {
    RAT_GSM         = 0,
    RAT_WCDMA       = 1,
    RAT_TDSCDMA     = 2,
    RAT_LTE         = 3,
};

struct monsc_cell_paras_s {
    uint8_t mcc[12];
    uint8_t mnc[12];
    uint16_t band;
    uint16_t arfcn;
    uint16_t psc;
    uint8_t sync_id;
    uint32_t cell_id;
    uint8_t bsic;
    uint8_t sc;
    uint16_t pci;
    uint16_t lac;
    uint16_t tac;
    int32_t rscp;
    int32_t rxlev;
    int32_t rsrp;
    int32_t rsrq;
    uint8_t rxquality;
    int32_t ec_n0;
    uint8_t drx;
    uint8_t ta;
    uint16_t rac;
    uint16_t ura;
};

struct me909s_monsc_s 
{
    me909s_monsc_rat_t rat;
    monsc_cell_paras_t cell_paras;
}; 

static inline void monsc_steps_assigment(me909s_monsc_t *m, const char *sval, int step)
{
    switch (m->rat)
    {
    case RAT_GSM:
        {
            switch (step)
            {
            case 1:
                m->cell_paras.band = (uint16_t)atoi(sval);
                break;
            case 2:
                m->cell_paras.arfcn = (uint16_t)atoi(sval);
                break;
            case 3:
                m->cell_paras.bsic = (uint8_t)atoi(sval);
                break;
            case 4:
                m->cell_paras.cell_id = (uint16_t)hexadecimal_to_uint32(sval);
                break;
            case 5:
                m->cell_paras.lac = (uint16_t)hexadecimal_to_uint32(sval);
                break;
            case 6:
                m->cell_paras.rxlev = (int32_t)atoi(sval);
                break;
            case 7:
                m->cell_paras.rxquality = (uint8_t)atoi(sval);
                break;
            case 8:
                m->cell_paras.ta = (uint8_t)atoi(sval);
                break;
            }
        }
        break;
    
    case RAT_WCDMA:
        {
            switch (step)
            {
            case 1:
                m->cell_paras.arfcn = (uint16_t)atoi(sval);
                break;
            case 2:
                m->cell_paras.psc = (uint16_t)atoi(sval);
                break;
            case 3:
                m->cell_paras.cell_id = (uint32_t)hexadecimal_to_uint32(sval);
                break;
            case 4:
                m->cell_paras.lac = (uint16_t)hexadecimal_to_uint32(sval);
                break;
            case 5:
                m->cell_paras.rscp = (int32_t)atoi(sval);
                break;
            case 6:
                m->cell_paras.rxlev = (int32_t)atoi(sval);
                break;
            case 7:
                m->cell_paras.ec_n0 = (int32_t)atoi(sval);
                break;
            case 8:
                m->cell_paras.drx = (uint8_t)atoi(sval);
                break;
            case 9:
                m->cell_paras.ura = (uint16_t)atoi(sval);
                break;
            }
        }
        break;

    case RAT_TDSCDMA:
        {
            switch (step)
            {
            case 1:
                m->cell_paras.arfcn = (uint16_t)atoi(sval);
                break;
            case 2:
                m->cell_paras.sync_id = (uint8_t)atoi(sval);
                break;
            case 3:
                m->cell_paras.sc = (uint8_t)atoi(sval);
                break;
            case 4:
                m->cell_paras.cell_id = (uint32_t)hexadecimal_to_uint32(sval);
                break;
            case 5:
                m->cell_paras.lac = (uint16_t)hexadecimal_to_uint32(sval);
                break;
            case 6:
                m->cell_paras.rscp = (int32_t)atoi(sval);
                break;
            case 7:
                m->cell_paras.drx = (uint8_t)atoi(sval);
                break;
            case 8:
                m->cell_paras.rac = (uint16_t)atoi(sval);
                break;
            }
        }
        break;

    case RAT_LTE:
        {
            switch (step)
            {
            case 1:
                m->cell_paras.arfcn = (uint16_t)atoi(sval);
                break;
            case 2:
                m->cell_paras.cell_id = (uint32_t)hexadecimal_to_uint32(sval);
                break;
            case 3:
                m->cell_paras.pci = (uint16_t)atoi(sval);
                break;
            case 4:
                m->cell_paras.tac = (uint16_t)hexadecimal_to_uint32(sval);;
                break;
            case 5:
                m->cell_paras.rsrp = (int32_t)atoi(sval);
                break;
            case 6:
                m->cell_paras.rsrq = (int32_t)atoi(sval);
                break;
            case 7:
                m->cell_paras.rxlev = (int32_t)atoi(sval);
                break;
            }
        }
        break;

    default:
        break;
    }
}

static inline void me909s_monsc_decode(const char *dat, me909s_monsc_t *monsc)
{
    if (!dat)
        return;

    if (!monsc)
        return;

    int i;
    char *element = NULL;
    char *vail = NULL;

    if ((vail = (char*)strstr(dat, ME909S_CMD_MONSC_RESPONSE)))
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
                if (strstr(element, "GSM"))
                    monsc->rat = RAT_GSM;
                else if (strstr(element, "WCDMA"))
                    monsc->rat = RAT_WCDMA;
                else if (strstr(element, "TD-SCDMA"))
                    monsc->rat = RAT_TDSCDMA;
                else if (strstr(element, "LTE"))
                    monsc->rat = RAT_LTE;
                else
                    return;
                break;

            case 2:
                me909s_strcopy((char*)monsc->cell_paras.mcc, element, strlen(element));
                break;

            case 3:
                me909s_strcopy((char*)monsc->cell_paras.mnc, element, strlen(element));
                break;

            case 4:
                monsc_steps_assigment(monsc, (const char*)element, 1);
                break;

            case 5:
                monsc_steps_assigment(monsc, (const char*)element, 2);
                break;

            case 6:
                monsc_steps_assigment(monsc, (const char*)element, 3);
                break;

            case 7:
                monsc_steps_assigment(monsc, (const char*)element, 4);
                break;

            case 8:
                monsc_steps_assigment(monsc, (const char*)element, 5);
                break;

            case 9:
                monsc_steps_assigment(monsc, (const char*)element, 6);
                break;

            case 10:
                monsc_steps_assigment(monsc, (const char*)element, 7);
                break;

            case 11:
                monsc_steps_assigment(monsc, (const char*)element, 8);
                break;

            case 12:
                monsc_steps_assigment(monsc, (const char*)element, 9);
                break;

            default:
                break;                
            }
        }
    }
}

static inline int me909s_monsc_code(char *cmd)
{
    if (!cmd)
        return -1;

    return sprintf(cmd, "%s\r\n", ME909S_CMD_MONSC);
}

static inline void me909s_monsc_debug(me909s_monsc_t *monsc)
{
    ME909S_DEBUG_PRINTF(("\n\n%s\n", ME909S_CMD_MONSC));
    ME909S_DEBUG_PRINTF(("**********************************************************\n"));
    switch (monsc->rat)
    {
    case RAT_GSM:
        ME909S_DEBUG_PRINTF(("RAT:\t\t%s\n", "GSM"));
        ME909S_DEBUG_PRINTF(("MCC:\t\t%s\n", monsc->cell_paras.mcc));
        ME909S_DEBUG_PRINTF(("MNC:\t\t%s\n", monsc->cell_paras.mnc));
        ME909S_DEBUG_PRINTF(("BAND:\t\t%u\n", monsc->cell_paras.band));
        ME909S_DEBUG_PRINTF(("ARFCN:\t\t%u\n", monsc->cell_paras.arfcn));
        ME909S_DEBUG_PRINTF(("BSIC:\t\t%u\n", monsc->cell_paras.bsic));
        ME909S_DEBUG_PRINTF(("Cell_ID:\t%lX\n", (long unsigned int)monsc->cell_paras.cell_id));
        ME909S_DEBUG_PRINTF(("LAC:\t\t%X\n", monsc->cell_paras.lac));
        ME909S_DEBUG_PRINTF(("RXLEV:\t\t%d\n", monsc->cell_paras.rxlev));
        ME909S_DEBUG_PRINTF(("RxQuality:\t%u\n", monsc->cell_paras.rxquality));
        ME909S_DEBUG_PRINTF(("TA:\t\t%u\n", monsc->cell_paras.ta));
        break;
    
    case RAT_WCDMA:
        ME909S_DEBUG_PRINTF(("RAT:\t\t%s\n", "WCDMA"));
        ME909S_DEBUG_PRINTF(("MCC:\t\t%s\n", monsc->cell_paras.mcc));
        ME909S_DEBUG_PRINTF(("MNC:\t\t%s\n", monsc->cell_paras.mnc));
        ME909S_DEBUG_PRINTF(("ARFCN:\t\t%u\n", monsc->cell_paras.arfcn));
        ME909S_DEBUG_PRINTF(("PSC:\t\t%u\n", monsc->cell_paras.psc));
        ME909S_DEBUG_PRINTF(("Cell_ID:\t%lX\n", (long unsigned int)monsc->cell_paras.cell_id));
        ME909S_DEBUG_PRINTF(("LAC:\t\t%X\n", monsc->cell_paras.lac));
        ME909S_DEBUG_PRINTF(("RSCP:\t\t%d\n", monsc->cell_paras.rscp));
        ME909S_DEBUG_PRINTF(("RXLEV:\t\t%d\n", monsc->cell_paras.rxlev));
        ME909S_DEBUG_PRINTF(("EC/N0:\t\t%d\n", monsc->cell_paras.ec_n0));
        ME909S_DEBUG_PRINTF(("DRX:\t\t%u\n", monsc->cell_paras.drx));
        ME909S_DEBUG_PRINTF(("URA:\t\t%u\n", monsc->cell_paras.ura));
        break;

    case RAT_TDSCDMA:
        ME909S_DEBUG_PRINTF(("RAT:\t\t%s\n", "TD-SCDMA"));
        ME909S_DEBUG_PRINTF(("MCC:\t\t%s\n", monsc->cell_paras.mcc));
        ME909S_DEBUG_PRINTF(("MNC:\t\t%s\n", monsc->cell_paras.mnc));
        ME909S_DEBUG_PRINTF(("ARFCN:\t\t%u\n", monsc->cell_paras.arfcn));
        ME909S_DEBUG_PRINTF(("SYNC_ID:\t%u\n", monsc->cell_paras.sync_id));
        ME909S_DEBUG_PRINTF(("SC:\t\t%u\n", monsc->cell_paras.sc));
        ME909S_DEBUG_PRINTF(("Cell_ID:\t%lX\n", (long unsigned int)monsc->cell_paras.cell_id));
        ME909S_DEBUG_PRINTF(("LAC:\t\t%X\n", monsc->cell_paras.lac));
        ME909S_DEBUG_PRINTF(("RSCP:\t\t%d\n", monsc->cell_paras.rscp));
        ME909S_DEBUG_PRINTF(("RXLEV:\t\t%d\n", monsc->cell_paras.rxlev));
        ME909S_DEBUG_PRINTF(("DRX:\t\t%u\n", monsc->cell_paras.drx));
        ME909S_DEBUG_PRINTF(("RAC:\t\t%u\n", monsc->cell_paras.rac));
        break;

    case RAT_LTE:
        ME909S_DEBUG_PRINTF(("RAT:\t\t%s\n", "LTE"));
        ME909S_DEBUG_PRINTF(("MCC:\t\t%s\n", monsc->cell_paras.mcc));
        ME909S_DEBUG_PRINTF(("MNC:\t\t%s\n", monsc->cell_paras.mnc));
        ME909S_DEBUG_PRINTF(("ARFCN:\t\t%u\n", monsc->cell_paras.arfcn));
        ME909S_DEBUG_PRINTF(("Cell_ID:\t%lX\n", (long unsigned int)monsc->cell_paras.cell_id));
        ME909S_DEBUG_PRINTF(("PCI:\t\t%u\n", monsc->cell_paras.pci));
        ME909S_DEBUG_PRINTF(("TAC:\t\t%X\n", monsc->cell_paras.tac));
        ME909S_DEBUG_PRINTF(("RSRP:\t\t%d\n", monsc->cell_paras.rsrp));
        ME909S_DEBUG_PRINTF(("RSRQ:\t\t%d\n", monsc->cell_paras.rsrq));
        ME909S_DEBUG_PRINTF(("RXLEV:\t\t%d\n", monsc->cell_paras.rxlev));
        break;

    default:
        break;
    }
    ME909S_DEBUG_PRINTF(("**********************************************************\n\n"));
}

#endif
