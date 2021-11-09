#include "atcommand.h"

int atcommand_indentification(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_INDENTIFICATION_SIZE);
    int ret;

    ret = me909s_indentification_code(payload);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}

int atcommand_sysinfoex(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_COMMAND_MAX_SIZE);
    int ret;

    ret = me909s_sysinfoex_code(payload, CMD_MOTION_INQUIRE);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}

int atcommand_unconnect(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_UNCONNECT_SIZE);
    int ret;

    ret = me909s_unconnect_code(payload);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}

int atcommand_monsc(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_MONSC_SIZE);
    int ret;

    ret = me909s_monsc_code(payload);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}

int atcommand_sim_signal(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_HCSQ_SIZE);
    int ret;

    ret = me909s_hcsq_code(payload);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}

int atcommand_ledctrl_set(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_LEDCTRL_SIZE);
    int ret;

    ret = me909s_ledctrl_code(payload, CMD_MOTION_SETUP, 1, 0, 0, 0, 0, 0, 0);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}


int atcommand_ledctrl_get(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_LEDCTRL_SIZE);
    int ret;

    ret = me909s_ledctrl_code(payload, CMD_MOTION_INQUIRE, 0, 0, 0, 0, 0, 0, 0);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}

int atcommand_cpin_set(struct atchannel_s *atchannel)
{
    /* non - support */
    return -1;
}

int atcommand_cpin_get(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_CPIN_SIZE);
    int ret;

    ret = me909s_cpin_code(payload, CMD_MOTION_INQUIRE, 0, 0);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}

int atcommand_ipcfl_set(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_IPCFL_SIZE);
    int ret;

    ret = me909s_ipcfl_code(payload, CMD_MOTION_SETUP, 10, 1472);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}

int atcommand_ipcfl_get(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_IPCFL_SIZE);
    int ret;

    ret = me909s_ipcfl_code(payload, CMD_MOTION_INQUIRE, 0, 0);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}

int atcommand_simswitch_set(struct atchannel_s *atchannel, int card)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_SIMSWITCH_SIZE);
    int ret;

    ret = me909s_simswitch_code(payload, CMD_MOTION_SETUP, (unsigned char)card);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}

int atcommand_simswitch_get(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_SIMSWITCH_SIZE);
    int ret;

    ret = me909s_simswitch_code(payload, CMD_MOTION_INQUIRE, 0);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}

int atcommand_eCall_set(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_ECLCFG_SIZE);
    int ret;

    ret = me909s_eCall_code(payload, CMD_MOTION_SETUP, 0, 1, 0);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}

int atcommand_eCall_get(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_ECLCFG_SIZE);
    int ret;

    ret = me909s_eCall_code(payload, CMD_MOTION_INQUIRE, 0, 0, 0);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}

int atcommand_network_set(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_CREG_SIZE);
    int ret;

    ret = me909s_creg_code(payload, CMD_MOTION_SETUP, 1);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}

int atcommand_network_get(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_CREG_SIZE);
    int ret;

    ret = me909s_creg_code(payload, CMD_MOTION_INQUIRE, 0);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}

int atcommand_netmode_set(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_SYSCFGEX_SIZE);
    int ret;

    ret = me909s_syscfgex_code(payload, CMD_MOTION_SETUP, atchannel->netmode);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}

int atcommand_netmode_get(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return -1;

    uv_buf_t *buf = malloc(sizeof(uv_buf_t));
    char *payload = malloc(ME909S_CMD_SYSCFGEX_SIZE);
    int ret;

    ret = me909s_syscfgex_code(payload, CMD_MOTION_INQUIRE, 0);
    if (ret <= 0) {
        free(buf);
        free(payload);
    }

    buf->base = payload;
    buf->len = ret;

    return atchannel->sendto(atchannel, buf);
}
