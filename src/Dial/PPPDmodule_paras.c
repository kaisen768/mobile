#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <errno.h>
#include "PPPDmodule_paras.h"

#define SHM_KEY    0x200
#define SHM_IFLAGS  (IPC_CREAT | IPC_EXCL)

#define PPPDMODULE_PARAMS_USE_SHM  0

PPPDmodule_t *get_PPPDmodule(void)
{
#if PPPDMODULE_PARAMS_USE_SHM
    int shmid = 0;
    PPPDmodule_t *shm = NULL;

    if ((shmid = shmget(SHM_KEY, sizeof(PPPDmodule_t), 0600|SHM_IFLAGS)) < 0) {
        if (errno == EEXIST) {
            shmid = shmget(SHM_KEY, sizeof(PPPDmodule_t), 0600|IPC_CREAT);
            if (shmid < 0)
                return NULL;
        } else {
            return NULL;
        }
    }

    if ((shm = (PPPDmodule_t*)(shmat(shmid, 0, 0))) == NULL)
        return NULL;

    return shm;
#else
    return malloc(sizeof(PPPDmodule_t));
#endif
}

static inline void PPPDmodule_paras_defualt(PPPDmodule_t *mem)
{
    mem->interface = (char *)"ppp0";
    mem->name = (char *)"pppd";
    mem->chat_file = (char *)"/tmp/chat-connect";
}

void PPPDmodule_setup(PPPDmodule_t *mem, char *interface, char *name, char *chat_file)
{
    if (!mem)
        return;

    PPPDmodule_paras_defualt(mem);

    if (interface)
        mem->interface = interface;

    if (name)
        mem->name = name;

    if (chat_file)
        mem->chat_file = chat_file;
}

void PPPDmodule_release(PPPDmodule_t *mem)
{
    if (!mem)
        return;

#if PPPDMODULE_PARAMS_USE_SHM
    mem->pid = -1;
    mem->name = NULL;
    memset(mem->args, 0, sizeof(mem->args));
    mem->chat_file = NULL;
#else
    free(mem);
    return;
#endif
}
