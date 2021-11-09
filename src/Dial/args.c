#include <stdio.h>
#include <string.h>
#include "args.h"
#include "dial.h"

// ----------------------------------------------------------------------------
//  PPPD Chat Connect File Configure
// ----------------------------------------------------------------------------

static inline int chat_file_config(const char *filename, const char *apn)
{
    int i = 0;
    int l_num = 0;
    FILE *fp = NULL;

    char *line[256];

    char cgdcont_cmd[64] = {0};
    if (apn)
        sprintf(cgdcont_cmd, "OK \\rAT+CGDCONT=1,\"IP\",\"%s\",\"\",0,0\n", apn);
    else
        sprintf(cgdcont_cmd, "OK \\rAT+CGDCONT=1,\"IP\",\"3gnet\",\"\",0,0\n");

    line[l_num++] = "TIMEOUT 1800\n";
    line[l_num++] = "ABORT 'NO CARRIER'\n";
    line[l_num++] = "ABORT 'ERROR'\n";
    line[l_num++] = "ABORT 'NODIALTONE'\n";
    line[l_num++] = "ABORT 'BUSY'\n";
    line[l_num++] = "ABORT 'NO ANSWER'\n";
    line[l_num++] = "'' \\rAT\n";
    line[l_num++] = "OK \\rATZ\n";
    line[l_num++] = cgdcont_cmd;
    line[l_num++] = "OK-AT-OK ATDT*99#\n";
    line[l_num++] = "CONNECT \\d\\c\n";

    if ((fp = fopen(filename, "w+")) == NULL)
        return -1;

    while (i < l_num) {
        fwrite(line[i], strlen(line[i]), 1, fp);
        i++;
    }

    fclose(fp);
    return 0;
}

// ----------------------------------------------------------------------------
//  PPPD Process Arguments Configure
// ----------------------------------------------------------------------------
// pppd debug nodetach /dev/ttyUSB0 115200 usepeerdns noauth noipdefault novj novjccomp noccp defaultroute ipcp-accept-local ipcp-accept-remote connect '/usr/bin/chat -s -v -f /tmp/chat-connect'

int args_setup(void *ctx, char *apn)
{
    dial_t *dial = ctx;
    PPPDmodule_t *pppd = dial->pppd;

    if (!dial)
        return -1;

    if (!pppd)
        return -2;

    if (!pppd->chat_file)
        return -3;

    if (chat_file_config(pppd->chat_file, apn) != 0)
        return -4;

    int i = 0;
    char **argv = pppd->args;

    static char chat_connect[64];
    memset(chat_connect, 0, sizeof(chat_connect));
    sprintf(chat_connect, "chat -s -v -f %s", pppd->chat_file);

    argv[i++] = "pppd";
    // argv[i++] = "modem";
	// argv[i++] = "crtscts";
    argv[i++] = "debug";
    argv[i++] = "nodetach";
    argv[i++] = "/dev/ttyUSB0";
    argv[i++] = "115200";
    argv[i++] = "usepeerdns";
    argv[i++] = "noauth";
    argv[i++] = "noipdefault";
    argv[i++] = "novj";
    argv[i++] = "novjccomp";
    argv[i++] = "noccp";
    argv[i++] = "defaultroute";
    argv[i++] = "ipcp-accept-local";
    argv[i++] = "ipcp-accept-remote";
    argv[i++] = "connect";
    argv[i++] = chat_connect;
    // argv[i++] = "disconnect"
    // argv[i++] = chat_disconnect;
    argv[i++] = (char*)0;

    return 0;
}
