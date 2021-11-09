#ifndef _PPPDMODULE_PARAS_H_
#define _PPPDMODULE_PARAS_H_

typedef struct PPPDmodule_s {
    int pid;
    char ip[20];
    char *interface;
    char *name;
    char *args[32];
    char *chat_file;
} PPPDmodule_t;

#define pppd_pid(pppd) (pppd->pid)

PPPDmodule_t *get_PPPDmodule(void);

void PPPDmodule_setup(PPPDmodule_t *mem, char *interface, char *name, char *chat_file);
void PPPDmodule_release(PPPDmodule_t *mem);

#endif
