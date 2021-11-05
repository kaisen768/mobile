#include "abnormal.h"
#include <stdbool.h>
#include <stdlib.h>

struct abnormal_param_s {
    abnormal_handletask_cb handletask;
    void *arg;
    bool *dup;
};

static bool duplication[ABNORMAL_ID_NUM] = {false};

static void abnormal_handler(uv_work_t *handle)
{
    struct abnormal_param_s *abparam = handle->data;
    
    *abparam->dup = true;

    abparam->handletask(abparam->arg);
}

static void abnormal_handle_exit(uv_work_t *handle, int status)
{
    struct abnormal_param_s *abparam = handle->data;
    *abparam->dup = false;

    free(abparam);
    free(handle);
}

void abnormal_handletask_add(uv_loop_t *loop, abnormal_handletask_cb handletask, void *arg, abnormal_id_t abnormalid)
{
    if (!loop || !handletask)
        return;

    if (duplication[abnormalid] == true)
        return;

    struct abnormal_param_s *abparam = malloc(sizeof(struct abnormal_param_s));
    abparam->handletask = handletask;
    abparam->arg = arg;
    abparam->dup = &duplication[abnormalid];

    uv_work_t *req = malloc(sizeof(uv_work_t));
    req->data = abparam;

    uv_queue_work(loop, req, abnormal_handler, abnormal_handle_exit);
}
