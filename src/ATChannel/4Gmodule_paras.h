#ifndef _4GMODULE_PARAS_H_
#define _4GMODULE_PARAS_H_

#include "ME909s/common.h"

typedef struct shm_4Gmodule_s {
    me909s_indentification_t indentification;
    me909s_sysinfoex_t sysinfoex;
    me909s_ledctrl_t ledctrl;
    me909s_syscfgex_t syscfgex;
    me909s_eCall_t eCall;
    me909s_creg_t network;
    me909s_monsc_t monsc;
    me909s_ipcfl_t ipcfl;
    me909s_cpin_t cpin;
    me909s_simswitch_t simswitch;
    me909s_cme_error_t cme;
    me909s_sim_state_t sim;
    me909s_srv_status_t srv;
    me909s_hcsq_t signal;
} shm_4Gmodule_t;

#define me909s_sim_state(shm) (shm->sim)
#define me909s_srv_state(shm) (shm->srv)
#define me909s_mcc(shm) (shm->monsc.cell_paras.mcc)
#define me909s_mnc(shm) (shm->monsc.cell_paras.mnc)
#define me909s_acqorder(shm) (shm->syscfgex.acqorder)

shm_4Gmodule_t *get_shm_4Gmodule(void);

typedef enum _me909s_para_id_e {
    ME909S_ID_ALL,
    ME909S_ID_INDENTIFICATION,
    ME909S_ID_SYSINFOEX,
    ME909S_ID_LEDCTRL,
    ME909S_ID_SYSCFGEX,
    ME909S_ID_ECALL,
    ME909S_ID_CREG,
    ME909S_ID_MONSC,
    ME909S_ID_IPCFL,
    ME909S_ID_CPIN,
    ME909S_ID_SIMSWITC,
    ME909S_ID_CMEERROR,
    ME909S_ID_SIMSTATE,
    ME909S_ID_SRVSTATE,
    ME909S_ID_HCSQ,
} me909s_para_id_t;

void reset_shm_4Gmodule(shm_4Gmodule_t *shm, me909s_para_id_t id);
bool judge_4Gmodule_para(shm_4Gmodule_t *shm, me909s_para_id_t id);

#endif
