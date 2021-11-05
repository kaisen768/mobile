#ifndef ME909S_COMMON_TYPES_H_
#define ME909S_COMMON_TYPES_H_

typedef enum me909s_srv_status_e me909s_srv_status_t;
typedef enum me909s_srv_domain_e me909s_srv_domain_t;
typedef enum me909s_sim_state_e  me909s_sim_state_t;

enum me909s_srv_status_e {
    SRV_STATUS_NOSERVICES            = 0,
    SRV_STATUS_RESTRICTED            = 1,
    SRV_STATUS_VALID                 = 2,
    SRV_STATUS_RESTRICTED_REGIONAL   = 3,
    SRV_STATUS_POWERSAVING_HIBERNATE = 4
};

enum me909s_srv_domain_e {
    SRV_DOMAIN_NOSERVICE    = 0,
    SRV_DOMAIN_ONLY_CS      = 1,
    SRV_DOMAIN_ONLY_PS      = 2,
    SRV_DOMAIN_PS_CS        = 3,
    SRV_DOMAIN_SEARCHING    = 4,
    // SRV_DOMAIN_CDMA         = 255 (not supported currently)
};

enum me909s_sim_state_e 
{
    SIM_STATE_INVAIL       = 0,
    SIM_STATE_AVAILABLE    = 1,
    SIM_STATE_INVAIL_CS    = 2,
    SIM_STATE_INVAIL_PS    = 3,
    SIM_STATE_INVAIL_PS_CS = 4,
    SIM_STATE_ROMSIM       = 240,
    SIM_STATE_NOINSERTED   = 255,
};

#endif
