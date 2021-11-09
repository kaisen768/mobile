#ifndef _PPPD_H_
#define _PPPD_H_

typedef enum pppd_args_e {
    PPPD_ARG_INTERFACE,
    PPPD_ARG_EXEC,
    PPPD_ARG_CHAT_FILE,
    PPPD_ARG_APN,
} pppd_args_t;

typedef struct pppd_module_s {
    char interface[32];
    char exec[32];
    char chat_file[32];
    char apn[32];

    int pid;
    char ip[20];
    char *args[32];

    void (*arg_set)(struct pppd_module_s *pppd, char *arg, pppd_args_t id);
    void (*generate_args)(struct pppd_module_s *pppd);
} pppd_module_t;

int pppd_module_init(struct pppd_module_s *pppd);

#endif
