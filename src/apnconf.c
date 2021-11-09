#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mxml.h"
#include "apnconf.h"

#define APNCONFIG_XML_FILE  "/etc/apns-full-conf.xml"
#define APNCONFIG_CNF_FILE  "/mnt/sdcard/apn.conf"

// ----------------------------------------------------------------------------
//  APN Configure /  XML Configure apns-full-conf.xml
// ----------------------------------------------------------------------------

static int apns_element_xml_match(apnconf_t *ctx)
{
    int i = 0;

    if (!ctx)
        return -1;

    FILE *fp = fopen(ctx->f.xml, "r");
    if (!fp) 
        return -2;

    mxml_node_t *apn = NULL;
    mxml_node_t *xml = mxmlLoadFile(NULL, fp, MXML_OPAQUE_CALLBACK);
    if (!xml)
        goto err;

    apn = mxmlFindElement(xml, xml, "apn", "mcc", \
                ctx->mcc, MXML_DESCEND);

    while (apn) {
        if (!strcmp(mxmlElementGetAttr(apn, "mnc"), \
                                        ctx->mnc)) {
            memcpy(ctx->apns[i++], \
                mxmlElementGetAttr(apn, "apn"), \
                strlen(mxmlElementGetAttr(apn, "apn")));
        }
        
        if (i >= APN_MATCH_MAX)
            break;

        apn = mxmlFindElement(apn, xml, "apn", \
                    "mcc", ctx->mcc, MXML_DESCEND);
    }

err:
    fclose(fp);

    return i;
}

// ----------------------------------------------------------------------------
//  APN Configure /  SD Card apn.conf
// ----------------------------------------------------------------------------

static inline int apn_strvail_proc(const char *src, char *apnvail)
{
    int h_space = 0;
    int t_space = 0;
    int avail_len = 0;
    int src_len;

    if (!src || !apnvail)
        return -1;

    src_len = strlen(src);

    while (src && *(src + h_space) == ' ')
        h_space++;

    while ((t_space < src_len) && \
            ((*(src + ((src_len-1) - t_space)) == ' ') || \
            (*(src + ((src_len-1) - t_space)) == '\n')))
        t_space++;

    avail_len = src_len - t_space - h_space;
    if (avail_len <= 0)
        return -1;

    for (int i = 0; i < avail_len; i++) {
        apnvail[i] = src[h_space + i];
    }

    return 0;
}

static int apn_element_file_conf(apnconf_t *ctx)
{
    char line[128] = {0};
    int idx = 0;

    if (ctx == NULL)
        return -1;

    FILE *fp = fopen(ctx->f.cnf, "r");
    if (!fp)
        return -2;

    while (fgets(line, sizeof(line), fp)) {
        if (!apn_strvail_proc(line, ctx->apns[idx]))
            idx++;
        else
            memset(ctx->apns[idx], 0, APN_STRING_LEN);
        memset(line, 0, sizeof(line));
    }

    fclose(fp);
    return idx;
}


// ----------------------------------------------------------------------------
//  APN Configure / Class Functions
// ----------------------------------------------------------------------------

static int configure(apnconf_t *ctx, char *mcc, char *mnc, char **apn)
{
    int r;
    int idx;
    static int increase = 0;

    if (!ctx)
        return -1;

    if (ctx->_available == false) {
        memset(ctx->apns, 0, sizeof(ctx->apns));

        /* 
         * if SD card apn.conf exist 
         * do this
         */
        if (!access(ctx->f.cnf, F_OK)  && \
                (ctx->_pri == APNCONF_PRIORITY_CNF)) {
            r = apn_element_file_conf(ctx);
        }

        /* 
         * if no SD card and apn.conf file
         * do this
         */
        if (access(ctx->f.cnf, F_OK) || (r <= 0)) {
            if (!mcc || !mnc) 
                return -2;
            else if (mcc[0] == '\0' || mnc[0] == '\0')
                return -2;
            
            ctx->mcc = (char*)mcc;
            ctx->mnc = (char*)mnc;

            r = apns_element_xml_match(ctx);
        }

        if (r < 0)
            return -3;

        ctx->count = r;
        ctx->_available = true;
        increase = 0;
    }

    if (ctx->_poll < 0)
        idx = increase < ctx->count ? increase++ : (increase = 1, 0);
    else
        idx = ctx->_poll < ctx->count ? ctx->_poll : (ctx->count - 1);

    (*apn) = ctx->apns[idx];

    return 0;
}

static void changeto_xml(apnconf_t *ctx)
{
    if (!ctx)
        return;

    if (ctx->_pri == APNCONF_PRIORITY_XML)
        return;

    ctx->_pri = APNCONF_PRIORITY_XML;
    
    ctx->reacquire(ctx);

    return;
}

static void _hold(apnconf_t *ctx, int idx)
{
    if (!ctx)
        return;
    ctx->_poll = idx;
}

static void _reacquire(apnconf_t *ctx)
{
    if (!ctx)
        return;

    ctx->count = 0;
    ctx->_available = false;
}

int apnconf_init(struct apnconf_s *ctx)
{
    if (!ctx)
        return -1;

    ctx->f.xml = APNCONFIG_XML_FILE;
    ctx->f.cnf = APNCONFIG_CNF_FILE;

    ctx->_available = false;
    ctx->count = 0;
    ctx->_poll = -1;
    ctx->_pri = APNCONF_PRIORITY_CNF;
    ctx->configure = configure;
    ctx->changeto_xml = changeto_xml;
    ctx->hold = _hold;
    ctx->reacquire = _reacquire;

    return 0;
}
