#include <stdio.h>
#include <string.h>
#include "args.h"

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

int args_setup(char *args[32], char *chat_file, char *apn)
{
    if (chat_file_config(chat_file, apn) != 0)
        return -1;

    int i = 0;

    static char chat_connect[64];
    memset(chat_connect, 0, sizeof(chat_connect));
    sprintf(chat_connect, "chat -s -v -f %s", chat_file);

    args[i++] = "pppd";
    // args[i++] = "modem";
	// args[i++] = "crtscts";
    args[i++] = "debug";
    args[i++] = "nodetach";
    args[i++] = "/dev/ttyUSB0";
    args[i++] = "115200";
    args[i++] = "usepeerdns";
    args[i++] = "noauth";
    args[i++] = "noipdefault";
    args[i++] = "novj";
    args[i++] = "novjccomp";
    args[i++] = "noccp";
    args[i++] = "defaultroute";
    args[i++] = "ipcp-accept-local";
    args[i++] = "ipcp-accept-remote";
    args[i++] = "connect";
    args[i++] = chat_connect;
    // args[i++] = "disconnect"
    // args[i++] = chat_disconnect;
    args[i++] = (char*)0;

    return 0;
}
