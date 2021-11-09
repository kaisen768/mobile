#ifndef MOBILE_APNCONF_H_
#define MOBILE_APNCONF_H_

#include <stdbool.h>

#define APN_MATCH_MAX   10
#define APN_STRING_LEN  32

typedef enum apnconf_priority_e {
    APNCONF_PRIORITY_CNF = 0,
    APNCONF_PRIORITY_XML = 1,
} apnconf_priority_t;

typedef enum apnconf_result_e {
    APNCONF_CONTEXT_NONE = -1,
    APNCONF_MNCC_INVAIL = -2,
    APNCONF_MATCH_FAILED = -3,
} apnconf_result_t;

typedef struct apnconf_s {
    struct apn_ori_s {
        char *xml;
        char *cnf;
    } f;

    char apns[APN_MATCH_MAX][APN_STRING_LEN];
    char *mcc;
    char *mnc;
    unsigned int count;
    int _poll;
    bool _available;

    apnconf_priority_t _pri;

    /** 
     * Apn Configure
     * 
     * @thiz: this
     * @mcc:  current mcc code
     * @mnc:  current mnc code
     * @apn:  apn pointer (No need to allocate memory space)
     * @return   0 if apn configure success, else error code is returned
     *          -1 "thiz" is NULL,
     *          -2 "mcc" or "mnc" is NULL
     *          -3 apn match fialed
     */
    int (*configure)(struct apnconf_s *thiz, char *mcc, char *mnc, char **apn);

    /** 
     * Change apn match mathod to xml config
     * 
     * @thiz: this
     * @return none
     */
    void (*changeto_xml)(struct apnconf_s *thiz);

    /** 
     * Hold index iterm of apn list
     * 
     * @thiz: this
     * @idx: index of apn list (0...9) (-1 auto)
     * @return none
     */
    void (*hold)(struct apnconf_s *thiz, int idx);

    /** 
     * Clear flags for update APN list
     * 
     * @thiz: this
     * @return none
     */
    void (*reacquire)(struct apnconf_s *thiz);
} apnconf_t;

int apnconf_init(struct apnconf_s *apnconf);

#endif
