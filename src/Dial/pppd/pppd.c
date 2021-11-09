#include <string.h>
#include "pppd.h"
#include "args.h"

#include <stdio.h>

static void argument_set(struct pppd_module_s *pppd, char *arg, pppd_args_t id)
{
    if (!pppd || !arg)
        return;

    char *pargument;
    int len = strlen(arg);
    int meml;

    switch (id) {
    case PPPD_ARG_INTERFACE:
        meml = sizeof(pppd->interface);
        pargument = pppd->interface;
        break;
    case PPPD_ARG_EXEC:
        meml = sizeof(pppd->exec);
        pargument = pppd->exec;
        break;
    case PPPD_ARG_CHAT_FILE:
        meml = sizeof(pppd->chat_file);
        pargument = pppd->chat_file;
        break;
    case PPPD_ARG_APN:
        meml = sizeof(pppd->apn);
        pargument = pppd->apn;
        break;
    }

    len = len < meml ? len : meml;

    memcpy(pargument, arg, len);
}

void generate_args(struct pppd_module_s *pppd)
{
    if (!pppd)
        return;

    args_setup(pppd->args, pppd->chat_file, pppd->apn);
}

int pppd_module_init(struct pppd_module_s *pppd)
{
    if (!pppd)
        return -1;

    memset(pppd, 0, sizeof(pppd_module_t));

    memcpy(pppd->interface, "ppp0", strlen("ppp0"));
    memcpy(pppd->exec, "pppd", strlen("pppd"));
    memcpy(pppd->chat_file, "/tmp/chat-defualt", strlen("/tmp/chat-defualt"));

    pppd->arg_set = argument_set;
    pppd->generate_args = generate_args;

    return 0;
}

