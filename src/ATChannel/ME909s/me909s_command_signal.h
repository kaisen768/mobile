#ifndef ME909S_COMMAND_SIGNAL_H_
#define ME909S_COMMAND_SIGNAL_H_

#include "me909s_basic.h"

/*
 * AT^HCSQ-Query and Report Signal Strength
 * 
 * Command Syntax:
 *      @ AT^HCSQ?
 *      @ AT^HCSQ=?
 *      @ <URC> = AT^HCSQ?
 * 
 * Interface Description:
 *      @desc: This command queries and reports the signal strength of the current service network. If the
 *              MT is registered with multiple networks in different service modes, you can query the signal
 *              strength of networks in each mode.
 *              No matter whether the MT is registered with a network or not, you can run this command to
 *              query the signal strength or allow the MT to unsolicitedly report the detected signal strength if
 *              the MT camps on the network. If the MT is not using any service network or the service mode
 *              is uncertain, "NOSERVICE" will be returned as the query result, and MT will not
 *              unsolicitedly report "NOSERVICE".
 *              The read command queries the current network signal strength detected by the MT.
 *              The test command returns the list of service modes supported by the MT.
 *              The URC command allows the MT to unsolicitedly report the current signal strength when the
 *              strength changes.
 * 
 * Parameter Description:
 *      <sysmode>: a string type value that indicates the service mode in which the MT will unsolicitedly report the signal strength.
 *          "NOSERVICE"     NOSERVICE mode
 *          "GSM"           GSM/GRPS/EDGE mode
 *          "WCDMA"         WCDMA/HSDPA/HSPA mode
 *          "TD-SCDMA"      TD-SCDMA mode
 *          "LTE"           LTE mode
 *          "CDMA"          CDMA mode (not supported currently)
 *          "EVDO"          EV-DO/eHRPD mode (not supported currently)
 *          "CDMA-EVDO"     CDMA/EV-DO (eHRPD) mode (not supported currently)
 *      <value1>, <value2>, <value3>, <value4>, <value5>: the following table lists the signal
 *          strength type corresponding to each service mode.
 *      |----------------------------------------------------------------------------------------|
 *      | <sysmode>   | <value1>     | <value2>      | <value3>      | <value4>    | <value5>    |
 *      |----------------------------------------------------------------------------------------|
 *      | "NOSERVICE" | -            | -             | -             | -           | -           |
 *      |-------------|--------------|---------------|---------------|-------------|-------------|
 *      | "GSM"       | gsm_rssi     | -             | -             | -           | -           |
 *      |-------------|--------------|---------------|---------------|-------------|-------------|
 *      | "WCDMA"     | wcdma_rssi   | wcdma_rscp    | wcdma_ecio    | -           | -           |
 *      |-------------|--------------|---------------|---------------|-------------|-------------|
 *      | "TD-SCDMA"  | tdscdma_rssi | tdscdma_rscp  | tdscdma_ecio  | -           | -           |
 *      |-------------|--------------|---------------|---------------|-------------|-------------|
 *      | "LTE"       | lte_rssi     | lte_rsrp      | lte_sinr      | lte_rsrq    | -           |
 *      |----------------------------------------------------------------------------------------|
 *  
 *      <gsm_rssi>, <wcdma_rssi>, <tdscdma_rssi>, <lte_rssi>: integer type values that indicate the
 *      received signal strength. These parameters are available for GSM, WCDMA, TD-SCDMA,
 *      LTE mode respectively.
 *          0  rssi < -120 dBm
 *          1  -120 dBm ≤ rssi < -119 dBm
 *          2  -119 dBm ≤ rssi < -118 dBm
 *          ... 
 *          94  -27 dBm ≤ rssi < -26 dBm
 *          95  -26 dBm ≤ rssi < -25 dBm
 *          96  -25 dBm ≤ rssi
 *          255  unknown or undetectable
 *      
 *      <wcdma_rscp>, <tdscdma_rscp>: an integer type value that indicates the received signal code
 *      power. This parameter is available for WCDMA and TD-SCDMA mode.
 *          0  rscp < -120 dBm
 *          1  -120 dBm ≤ rscp < -119 dBm
 *          2  -119 dBm ≤ rscp < -118 dBm
 *          ... 
 *          94  -27 dBm ≤ rscp < -26 dBm
 *          95  -26 dBm ≤ rscp < -25 dBm
 *          96  -25 dBm ≤ rscp
 *          255  unknown or undetectable
 * 
 *      <wcdma_ecio>, <tdscdma_ecio>: integer type values that indicate the downlink
 *      carrier-to-interference ratio. These parameters are available for WCDMA, TD-SCDMA mode respectively.
 *          0  Ec/Io < -32 dB
 *          1  -32 dB ≤ Ec/Io < -31.5 dB
 *          2  -31.5 dB ≤ Ec/Io < -31 dB
 *          ... 
 *          63  -1 dB ≤ Ec/Io < -0.5 dB
 *          64  -0.5 dB ≤ Ec/Io < 0 dB
 *          65  0 dB ≤ Ec/Io
 *          255  unknown or undetectable
 * 
 *      <lte_rsrp>: an integer type value that indicates the reference signal received power. This
 *      parameter is available for LTE mode.
 *          0  rsrp < -140 dBm
 *          1  -140 dBm ≤ rsrp < - 139 dBm
 *          2  -139 dBm ≤ rsrp < -138 dBm
 *          ... 
 *          95  -46 dBm ≤ rsrp < -45 dBm
 *          96  -45 dBm ≤ rsrp < -44 dBm
 *          97  -44 dBm ≤ rsrp
 *          255  unknown or undetectable
 * 
 *      <lte_sinr>: an integer type value that indicates the signal to interference plus noise ratio. This
 *      parameter is available for LTE mode.
 *          0  sinr < -20 dB
 *          1  -20 dB ≤ sinr < -19.8 dB
 *          2  -19.8 dB ≤ sinr < -19.6 dB
 *          ... 
 *          249  29.6 dB ≤ sinr < 29.8 dB
 *          250  29.8 dB ≤ sinr < 30 dB
 *          251  30 dB ≤ sinr
 *          255  unknown or undetectable
 *  
 *      <lte_rsrq>: an integer type value that indicates the reference signal received quality in dB.
 *          0  rsrq < -19.5 dB
 *          1  -19.5 dB ≤ rsrq < -19 dB
 *          2  -19 dB ≤ rsrq < -18.5 dB
 *          ... 
 *          32  -4 dB ≤ rsrq < -3.5 dB
 *          33  -3.5 dB ≤ rsrq < -3 dB
 *          34  -3 dB ≤ rsrq
 *          255  unknown or undetectable
 * 
 * Example:
 *   @1 Run:        AT^HCSQ=?
 *      Response:  ^HCSQ: "NOSERVICE","GSM","WCDMA","TD-SCDMA","LTE"
 *                  OK
 * 
 *   @2 Run:        AT^HCSQ? 
 *      Response:   ^HCSQ: "WCDMA",30,30,58
 *                  OK
 */

#define ME909S_CMD_HCSQ             "AT^HCSQ"
#define ME909S_CMD_HCSQ_RESPONSE    "^HCSQ: "
#define ME909S_CMD_HCSQ_SIZE        16

typedef struct me909s_hcsq_s me909s_hcsq_t;
typedef enum me909s_hcsq_sysmode_e me909s_hcsq_sysmode_t;

enum me909s_hcsq_sysmode_e
{
    SYSMODE_NOSERVICE = 0,
    SYSMODE_GSM,
    SYSMODE_WCDMA,
    SYSMODE_TDSCDMA,
    SYSMODE_LTE,
};

struct me909s_hcsq_s 
{
    me909s_hcsq_sysmode_t sysmode;
    uint8_t value1;
    uint8_t value2;
    uint8_t value3;
    uint8_t value4;
    uint8_t value5;
};

static inline void me909s_hcsq_decode(const char *dat, me909s_hcsq_t *hcsq)
{
    if (!dat)
        return;

    if (!hcsq)
        return;

    char sysmode[16];
    char unuse[16];
    int value1 = 0;
    int value2 = 0;
    int value3 = 0;
    int value4 = 0;
    int value5 = 0;

    sscanf(dat, "^HCSQ: \"%[^\"]", sysmode);
    sscanf(dat, "^HCSQ: \"%[^\"]\",%d,%d,%d,%d,%d", unuse, &value1, &value2, &value3, &value4, &value5);

    if (strstr(sysmode, "LTE"))
        hcsq->sysmode = SYSMODE_LTE;
    else if (strstr(sysmode, "WCDMA"))
        hcsq->sysmode = SYSMODE_WCDMA;
    else if (strstr(sysmode, "TD-SCDMA"))
        hcsq->sysmode = SYSMODE_TDSCDMA;
    else if (strstr(sysmode, "GSM"))
        hcsq->sysmode = SYSMODE_GSM;
    else if (strstr(sysmode, "NOSERVICE"))
        hcsq->sysmode = SYSMODE_NOSERVICE;
    else
        hcsq->sysmode = SYSMODE_NOSERVICE;

    if (hcsq->sysmode != SYSMODE_NOSERVICE)
    {
        hcsq->value1 = (uint8_t)value1;
        hcsq->value2 = (uint8_t)value2;
        hcsq->value3 = (uint8_t)value3;
        hcsq->value4 = (uint8_t)value4;
        hcsq->value5 = (uint8_t)value5;
    }
    else
    {
        hcsq->value1 = 0;
        hcsq->value2 = 0;
        hcsq->value3 = 0;
        hcsq->value4 = 0;
        hcsq->value5 = 0;
    }
    
    return;
}

static inline int me909s_hcsq_code(char *cmd)
{
    if (!cmd)
        return -1;

    return sprintf(cmd, "%s?\r\n", ME909S_CMD_HCSQ);
}

static inline void me909s_hcsq_debug(me909s_hcsq_t *hcsq)
{
    if (!hcsq)
        return;

    ME909S_DEBUG_PRINTF(("\n\n%s\n", ME909S_CMD_HCSQ));
    ME909S_DEBUG_PRINTF(("**********************************************************\n"));
    switch (hcsq->sysmode)
    {
    case SYSMODE_LTE:
        ME909S_DEBUG_PRINTF(("sysmode:\tLTE\n"));
        ME909S_DEBUG_PRINTF(("rssi:\t\t%u\n", hcsq->value1));
        ME909S_DEBUG_PRINTF(("rsrp:\t\t%u\n", hcsq->value2));
        ME909S_DEBUG_PRINTF(("sinr:\t\t%u\n", hcsq->value3));
        ME909S_DEBUG_PRINTF(("rsrq:\t\t%u\n", hcsq->value4));
        break;
    case SYSMODE_WCDMA:
        ME909S_DEBUG_PRINTF(("sysmode:\tWCDMA\n"));
        ME909S_DEBUG_PRINTF(("rssi:\t\t%u\n", hcsq->value1));
        ME909S_DEBUG_PRINTF(("rscp:\t\t%u\n", hcsq->value2));
        ME909S_DEBUG_PRINTF(("ecio:\t\t%u\n", hcsq->value3));
        break;
    case SYSMODE_TDSCDMA:
        ME909S_DEBUG_PRINTF(("sysmode:\tTD-SCDMA\n"));
        ME909S_DEBUG_PRINTF(("rssi:\t\t%u\n", hcsq->value1));
        ME909S_DEBUG_PRINTF(("rscp:\t\t%u\n", hcsq->value2));
        ME909S_DEBUG_PRINTF(("ecio:\t\t%u\n", hcsq->value3));
        break;
    case SYSMODE_GSM:
        ME909S_DEBUG_PRINTF(("sysmode:\tGSM\n"));
        ME909S_DEBUG_PRINTF(("rssi:\t\t%u\n", hcsq->value1));
        break;
    case SYSMODE_NOSERVICE:
        ME909S_DEBUG_PRINTF(("sysmode:\tNOSERVICE\n"));
        break;
    }
    ME909S_DEBUG_PRINTF(("**********************************************************\n\n"));
}

#endif
