#include <sys/shm.h>
#include <errno.h>
#include "4Gmodule_paras.h"

#define SHM_KEY    0x100
#define SHM_IFLAGS  (IPC_CREAT | IPC_EXCL)

shm_4Gmodule_t *get_shm_4Gmodule(void)
{
    int shmid = 0;
    shm_4Gmodule_t *shm = NULL;

    if ((shmid = shmget(SHM_KEY, sizeof(shm_4Gmodule_t), 0600|SHM_IFLAGS)) < 0) {
        if (errno == EEXIST) {
            shmid = shmget(SHM_KEY, sizeof(shm_4Gmodule_t), 0600|IPC_CREAT);
            if (shmid < 0)
                return NULL;
        }
    } else {
        return NULL;
    }

    if ((shm = shmat(shmid, 0, 0)) == NULL)
        return NULL;

    return shm;
}

void reset_shm_4Gmodule(shm_4Gmodule_t *shm, me909s_para_id_t id)
{
    if (!shm)
        return;

    switch (id) {
    case ME909S_ID_ALL:
        memset(shm, 0, sizeof(shm_4Gmodule_t));
        shm->cme = CME_INITALIZE;
        shm->sim = SIM_STATE_AVAILABLE;
        break;

    case ME909S_ID_INDENTIFICATION:
        memset(&shm->indentification, 0, sizeof(me909s_indentification_t));
        break;

    case ME909S_ID_SYSINFOEX:
        memset(&shm->sysinfoex, 0, sizeof(me909s_sysinfoex_t));
        break;

    case ME909S_ID_LEDCTRL:
        memset(&shm->ledctrl, 0, sizeof(me909s_ledctrl_t));
        break;

    case ME909S_ID_SYSCFGEX:
        memset(&shm->syscfgex, 0, sizeof(me909s_syscfgex_t));
        break;

    case ME909S_ID_ECALL:
        memset(&shm->eCall, 0, sizeof(me909s_eCall_t));
        break;

    case ME909S_ID_CREG:
        memset(&shm->network, 0, sizeof(me909s_creg_t));
        break;

    case ME909S_ID_MONSC:
        memset(&shm->monsc, 0, sizeof(me909s_monsc_t));
        break;

    case ME909S_ID_IPCFL:
        memset(&shm->ipcfl, 0, sizeof(me909s_ipcfl_t));
        break;

    case ME909S_ID_CPIN:
        memset(&shm->cpin, 0, sizeof(me909s_cpin_t));
        break;

    case ME909S_ID_SIMSWITC:
        memset(&shm->simswitch, 0, sizeof(me909s_simswitch_t));
        break;

    case ME909S_ID_CMEERROR:
        shm->cme = CME_INITALIZE;
        break;

    case ME909S_ID_SIMSTATE:
        shm->sim = SIM_STATE_AVAILABLE;
        break;

    case ME909S_ID_SRVSTATE:
        shm->srv = SRV_STATUS_NOSERVICES;
        break;

    case ME909S_ID_HCSQ:
        memset(&shm->signal, 0, sizeof(me909s_hcsq_t));
        break;
    }
}

bool judge_4Gmodule_para(shm_4Gmodule_t *shm, me909s_para_id_t id)
{
    bool judge = false;

    if (!shm)
        return false;

    switch (id) {
    case ME909S_ID_INDENTIFICATION:
        {
            me909s_indentification_t *inde = &shm->indentification;
            if (strstr(inde->manufacturer, "Huawei Technologies Co., Ltd."))
                judge = true;
        }
        break;

    case ME909S_ID_LEDCTRL:
        {
            me909s_ledctrl_t *ledctrl = &shm->ledctrl;
            judge = (ledctrl->mode == 1) ? true : false;
        }
        break;

    case ME909S_ID_ECALL:
        {
            me909s_eCall_t *eCall = &shm->eCall;
            if ((eCall->mode == 0) && 
                (eCall->voc_config == 1) && 
                (eCall->redial_config == 0)) 
            {
                judge = true;
            }
        }
        break;

    case ME909S_ID_CREG:
    case ME909S_ID_SYSINFOEX:
        {
            me909s_creg_t *network = &shm->network;
            me909s_sysinfoex_t *sysinfoex = &shm->sysinfoex;
            if (network->stat == CREG_STAT_REGISTED_LOCALNET 
                || network->stat == CREG_STAT_REGISTED_ROAMNET
                || sysinfoex->srv_status == SRV_STATUS_VALID
                || sysinfoex->srv_domain == SRV_DOMAIN_ONLY_PS
                || sysinfoex->srv_domain == SRV_DOMAIN_PS_CS) 
            {
                judge = true;
            }
        }
        break;

    case ME909S_ID_IPCFL:
        {
            me909s_ipcfl_t *ipcfl = &shm->ipcfl;
            judge = (ipcfl->parameter[IPCFL_PARAMETER_ID_10].value == 1472) ? 
                    true : false;
        }
        break;

    case ME909S_ID_CPIN:
        {
            me909s_cpin_t *cpin = &shm->cpin;
            judge = (cpin->code[0] != '\0') ? true : false;
        }
        break;

    case ME909S_ID_CMEERROR:
        judge = (shm->cme != CME_INITALIZE) ? true : false;
        break;

    case ME909S_ID_SIMSTATE:
        judge = (shm->sim == SIM_STATE_AVAILABLE) ? true : false;
        break;

    default:
        break;
    }

    return judge;
}
