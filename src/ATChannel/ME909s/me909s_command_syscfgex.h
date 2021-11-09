#ifndef ME909S_COMMAND_SYSCFGEX_H_
#define ME909S_COMMAND_SYSCFGEX_H_

#include "me909s_basic.h"

/*
 * AT^SYSCFGEX-Configure Extended System
 * 
 * Command Syntax:
 *      @ AT^SYSCFGEX=<acqorder>,<band>,<roam>,<srvdomain>,<lteband>,<reserve1>,<reserve2>
 *      @ AT^SYSCFGEX?
 *      @ AT^SYSCFGEX=?
 * 
 * Interface Description:
 *      @desc: This command sets the system mode, network access order, frequency band, 
 *             roaming support,domain, and other features.
 * 
 * Parameter Description:
 *      <acqorder>: a string type value that specifies the network access order. Its value can be "00",
 *                  "99" or a combination of the following values:
 *            "00"  Automatic
 *            "01"  GSM
 *            "02"  UMTS
 *            "03"  LTE
 *            "04"  CDMA 1X (not supported currently)
 *            "06"  WiMAX (not supported currently)
 *            "07"  CDMA EVDO (not supported currently)
 *            "99"  Not change
 *          For example, the "03" value indicates LTE only.
 *          The "030201" value indicates the order of LTE->WCDMA->GSM.
 *          The "0302" value indicates the order of LTE->WCDMA, without GSM.
 *          The "030402" value indicates the order of LTE->CDMA->WCDMA.
 *          In specialty, the "99" value is not combined with other values, 
 *              indicating no change of the network access order.
 *          The "00" value is not combined with other values, indicating automatic network access
 *      <band>: a hexadecimal value that specifies the frequency band, which is related to the system
 *              mode and dependent on the module performance. The possible values of <band> are the
 *              following values and their combinations (excluding 0x3FFFFFFF and 0x40000000):
 *          00080000 (CM_BAND_PREF_GSM_850)                 GSM 850
 *          00000080 (CM_BAND_PREF_GSM_DCS_1800)            GSM DCS systems
 *          00000100 (CM_BAND_PREF_GSM_EGSM_900)            Extended GSM 900
 *          00000200 (CM_BAND_PREF_GSM_PGSM_900)            Primary GSM 900
 *          00100000 (CM_BAND_PREF_GSM_RGSM_900)            Railway GSM 900
 *          00200000 (CM_BAND_PREF_GSM_PCS_1900)            GSM PCS
 *          00400000 (CM_BAND_PREF_WCDMA_I_IMT_2000)        WCDMA IMT 2100
 *          00800000 (CM_BAND_PREF_WCDMA_II_PCS_1900)       WCDMA_II_PCS_1900
 *          04000000 (CM_BAND_PREF_WCDMA_V_850)             WCDMA_V_850
 *          08000000(CM_BAND_PREF_WCDMA_VI_800)             WCDMA_VI_800   
 *          3FFFFFFF (CM_BAND_PREF_ANY)                     All bands
 *          40000000 (CM_BAND_PREF_NO_CHANGE)               Band not changed
 *          0004000000000000 (CM_BAND_PREF_WCDMA_IX_1700)   WCDMA_IX_1700
 *          0002000000000000 (CM_BAND_PREF_WCDMA_VIII_900)  WCDMA_VIII_900
 *          1000000000000000 (CM_BAND_PREF_WCDMA_XIX_850)   WCDMA_XIX_850
 *          2000000 (CM_BAND_PREF_WCDMA_IX_1700)            AWS
 *          00680380                                        Automatic
 * 
 *      <band_name>: a string type value that indicates the frequency band name.
 *          For WCDMA, it is named in WCDMA BCx format, in which, x indicates the actual
 *                  Band Class (refer to 3GPP TS 25.101 Table 5.1). If multiple bands are simultaneously
 *                  supported, it is separated by / (for example, WCDMA BC I/WCDMA BC II).
 *          For GSM, it is named in GSM850/GSM1800.... format.
 *      <roam>: indicates whether roaming is supported.
 *          0  Not supported
 *          1  Supported
 *          2  No change
 *      <srvdomain>: indicates the domain setting.
 *          0  CS_ONLY
 *          1  PS_ONLY
 *          2  CS_PS
 *          3  ANY
 *          4  No change
 *      <lteband>: a hexadecimal value that specifies the LTE frequency band. The value of
 *              <lteband> can be one of the following values and their combinations (excluding 
 *              0x7FFFFFFFFFFFFFFF):
 *          7FFFFFFFFFFFFFFF  (CM_BAND_PREF_ANY)  All bands
 *          1               (CM_BAND_PREF_LTE_EUTRAN_BAND1)     LTE BC1
 *          2               (CM_BAND_PREF_LTE_EUTRAN_BAND2)     LTE BC2
 *          4               (CM_BAND_PREF_LTE_EUTRAN_BAND3)     LTE BC3
 *          8               (CM_BAND_PREF_LTE_EUTRAN_BAND4)     LTE BC4
 *          10              (CM_BAND_PREF_LTE_EUTRAN_BAND5)     LTE BC5
 *          20              (CM_BAND_PREF_LTE_EUTRAN_BAND6)     LTE BC6
 *          40              (CM_BAND_PREF_LTE_EUTRAN_BAND7)     LTE BC7
 *          80              (CM_BAND_PREF_LTE_EUTRAN_BAND8)     LTE BC8
 *          100             (CM_BAND_PREF_LTE_EUTRAN_BAND9)     LTE BC9
 *          200             (CM_BAND_PREF_LTE_EUTRAN_BAND10)    LTE BC10
 *          400             (CM_BAND_PREF_LTE_EUTRAN_BAND11)    LTE BC11
 *          800             (CM_BAND_PREF_LTE_EUTRAN_BAND12)    LTE BC12
 *          1000            (CM_BAND_PREF_LTE_EUTRAN_BAND13)    LTE BC13
 *          2000            (CM_BAND_PREF_LTE_EUTRAN_BAND14)    LTE BC14
 *          10000           (CM_BAND_PREF_LTE_EUTRAN_BAND17)    LTE BC17
 *          20000           (CM_BAND_PREF_LTE_EUTRAN_BAND18)    LTE BC18
 *          40000           (CM_BAND_PRE_LTE_EUTRAN_BAND19)     LTE BC19
 *          80000           (CM_BAND_PREF_LTE_EUTRAN_BAND20)    LTE BC20
 *          100000          (CM_BAND_PREF_LTE_EUTRAN_BAND21)    LTE BC21
 *          1000000         (CM_BAND_PREF_LTE_EUTRAN_BAND25)    LTE BC25
 *          2000000         (CM_BAND_PREF_LTE_EUTRAN_BAND26)    LTE BC26
 *          8000000         (CM_BAND_PREF_LTE_EUTRAN_BAND28)    LTE BC28
 *          100000000       (CM_BAND_PREF_LTE_EUTRAN_BAND33)    LTE BC33
 *          200000000       (CM_BAND_PREF_LTE_EUTRAN_BAND34)    LTE BC34
 *          400000000       (CM_BAND_PREF_LTE_EUTRAN_BAND35)    LTE BC35
 *          800000000       (CM_BAND_PREF_LTE_EUTRAN_BAND36)    LTE BC36
 *          1000000000      (CM_BAND_PREF_LTE_EUTRAN_BAND37)    LTE BC37
 *          2000000000      (CM_BAND_PREF_LTE_EUTRAN_BAND38)    LTE BC38
 *          4000000000      (CM_BAND_PREF_LTE_EUTRAN_BAND39)    LTE BC39
 *          8000000000      (CM_BAND_PREF_LTE_EUTRAN_BAND40)    LTE BC40
 *          10000000000     (CM_BAND_PREF_LTE_EUTRAN_BAND41)    LTE BC41
 *          20000000000     (CM_BAND_PREF_LTE_EUTRAN_BAND42)    LTE BC42
 *          40000000000     (CM_BAND_PREF_LTE_EUTRAN_BAND43)    LTE BC43
 *          40000000        (CM_BAND_PREF_NO_CHANGE)            No band change
 * 
 * Example:
 *   @1 Run:        AT^SYSCFGEX=?
 *      Response:   ^SYSCFGEX:
 *                  ("00","03","02","01","99"),((6000000400380,"GSM900/GSM1800/WCDMA BCVIII/WCDMA BCIX/WCDMA BCI"),(4000000,"WCDMABCV"),(3fffffff,"Allbands")),(0-2),(0-4),((1e000000085,"LTE BC1/LTE BC3/LTEBC8/LTE BC38/LTE BC39/LTE BC40/LTEBC41"),(7fffffffffffffff,"All bands"))
 *                  OK
 * 
 *   @2 Run:        AT^SYSCFGEX? 
 *      Response:   ^SYSCFGEX: "00",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF
 *                  OK
 * 
 *   @3 Run:        AT^SYSCFGEX="02",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF,,
 *      Response:   OK
 */

#define ME909S_CMD_SYSCFGEX             "AT^SYSCFGEX"
#define ME909S_CMD_SYSCFGEX_RESPONSE    "^SYSCFGEX:"
#define ME909S_CMD_SYSCFGEX_SIZE        64

typedef struct me909s_syscfgex_s 
{
    int8_t acqorder[16];
    uint64_t band;
    uint8_t roam;
    uint8_t srvdomain;
    uint64_t lteband;
    char reserve1[64];
    char reserve2[64];
} me909s_syscfgex_t;

typedef enum me909s_syscfgex_netmode_e {
    ME909S_NETMODE_AUTO = 0,
    ME909S_NETMODE_4G3G2G,
    ME909S_NETMODE_4G3G,
    ME909S_NETMODE_3G2G,
    ME909S_NETMODE_4G,
    ME909S_NETMODE_3G,
    ME909S_NETMODE_2G,
    ME909S_NETMODE_INVAIL,
} me909s_syscfgex_netmode_t;

static char *me909s_syscfgex_mode_to_acqorder(me909s_syscfgex_netmode_t mode);
static me909s_syscfgex_netmode_t me909s_syscfgex_acqorder_to_mode(const char *acqorder);

static inline void me909s_syscfgex_decode(const char *dat, me909s_syscfgex_t *syscfgex)
{
    if (!dat)
        return;

    if (!syscfgex)
        return;

    int i;
    char *element = NULL;
    char *vail = NULL;

    if ((vail = (char*)strstr(dat, ME909S_CMD_SYSCFGEX_RESPONSE)))
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
                me909s_strcopy((char*)syscfgex->acqorder, element, strlen(element));
                break;

            case 2:
                syscfgex->band = hexadecimal_to_uint64(element);
                break;

            case 3:
                syscfgex->roam = atoi(element);
                break;

            case 4:
                syscfgex->srvdomain = atoi(element);
                break;

            case 5:
                syscfgex->lteband = hexadecimal_to_uint64(element);
                return;

            default:
                break;                
            }
        }
    }
}

static inline int me909s_syscfgex_code(char *cmd, me909s_cmd_motion_t motion, me909s_syscfgex_netmode_t mode)
{
    int r;

    if (!cmd)
        return -1;

    switch (motion)
    {
    case CMD_MOTION_SETUP:
        r = sprintf(cmd, "%s=\"%s\",3FFFFFFF,1,2,7FFFFFFFFFFFFFFF,,\r\n", ME909S_CMD_SYSCFGEX, 
                me909s_syscfgex_mode_to_acqorder(mode));
        break;

    case CMD_MOTION_INQUIRE:
        r = sprintf(cmd, "%s?\r\n", ME909S_CMD_SYSCFGEX);
        break;

    default:
        break;
    }

    return r;
}

static inline void me909s_syscfgex_debug(me909s_syscfgex_t *syscfgex)
{
    ME909S_DEBUG_PRINTF(("\n\n%s\n", ME909S_CMD_SYSCFGEX));
    ME909S_DEBUG_PRINTF(("**********************************************************\n"));
    ME909S_DEBUG_PRINTF(("acqorder:\t%s\n", syscfgex->acqorder));
    if (syscfgex->band > 0x100000000)
        ME909S_DEBUG_PRINTF(("band:\t\t%016llX\n", syscfgex->band));
    else
        ME909S_DEBUG_PRINTF(("band:\t\t%08X\n", (uint32_t)syscfgex->band));
    ME909S_DEBUG_PRINTF(("roam:\t\t%u\n", syscfgex->roam));
    ME909S_DEBUG_PRINTF(("srvdomain:\t%u\n", syscfgex->srvdomain));
    ME909S_DEBUG_PRINTF(("lteband:\t%016llX\n", syscfgex->lteband));
    ME909S_DEBUG_PRINTF(("**********************************************************\n\n"));
}

char *me909s_syscfgex_mode_to_acqorder(me909s_syscfgex_netmode_t mode)
{
    switch (mode) {
    case ME909S_NETMODE_4G3G2G:
        return (char *)"030201";

    case ME909S_NETMODE_4G3G:
        return (char *)"0302";

    case ME909S_NETMODE_3G2G:
        return (char *)"0201";

    case ME909S_NETMODE_4G:
        return (char *)"03";

    case ME909S_NETMODE_3G:
        return (char *)"02";

    case ME909S_NETMODE_2G:
        return (char *)"01";

    case ME909S_NETMODE_INVAIL:
    case ME909S_NETMODE_AUTO:
    default:
        return (char *)"00";
    }
}

me909s_syscfgex_netmode_t me909s_syscfgex_acqorder_to_mode(const char *acqorder)
{
    if (strcmp(acqorder, "00"))
        return ME909S_NETMODE_AUTO;
    else if (strcmp(acqorder, "03"))
        return ME909S_NETMODE_4G;
    else if (strcmp(acqorder, "02"))
        return ME909S_NETMODE_3G;
    else if (strcmp(acqorder, "01"))
        return ME909S_NETMODE_2G;
    else if (strcmp(acqorder, "030201"))
        return ME909S_NETMODE_4G3G2G;
    else if (strcmp(acqorder, "0302"))
        return ME909S_NETMODE_4G3G;
    else if (strcmp(acqorder, "0201"))
        return ME909S_NETMODE_3G2G;
    else
        return ME909S_NETMODE_INVAIL;
}

#endif
