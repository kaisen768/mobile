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

}

bool judge_4Gmodule_para(void *para, me909s_para_id_t id)
{

}
