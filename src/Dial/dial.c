#include <inttypes.h>
#include <stdlib.h>
#include "dial.h"
#include "ipaddr.h"

#define RECEIVE_BUFFER_SIZE 1024

static char receivebuffer[RECEIVE_BUFFER_SIZE];

static void alloc_callback(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    buf->base = receivebuffer;
    buf->len = RECEIVE_BUFFER_SIZE;
}

static void exit_callback(uv_process_t *req, int64_t exit_status, int term_signal)
{
    fprintf(stderr, "Process exited with status %" PRId64 ", signal %d\n", exit_status, term_signal);
    uv_close((uv_handle_t*)req, NULL);

    /* exit handler */
    dial_t *dial = req->data;

    /* pppd processes in the background */
    if (term_signal == 0)
        dial->status = DIAL_REMAIN;
}

static void on_stream_receive(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf)
{
    dial_t *dial = handle->data;

    /* pppd process receive stream */
    if (nread < 0) {
        fprintf(stderr, "read stream aborted status: %d, err: %s\n", nread, uv_strerror(nread));
        uv_close((uv_handle_t*)&dial->_pipe_read, NULL);
    }
}

static uv_pid_t invoke_script(struct dial_s *dial)
{
    if (!dial)
        return -1;

    pppd_module_t *pppd = &dial->pppd;
    char **args = pppd->args;

    dial->_opts.stdio_count = 3;
    uv_stdio_container_t child_stdio[3];
    child_stdio[0].flags = UV_IGNORE;
    child_stdio[1].flags = UV_CREATE_PIPE | UV_WRITABLE_PIPE;
    child_stdio[1].data.stream = (uv_stream_t *)&dial->_pipe_read;
    child_stdio[2].flags = UV_IGNORE;
    dial->_opts.stdio = child_stdio;

    dial->_opts.exit_cb = exit_callback;
    dial->_opts.file = args[0];
    dial->_opts.args = args;

    int r;
    if ((r = uv_spawn(dial->loop, &dial->_proc, &dial->_opts))) {
        fprintf(stderr, "%s\n", uv_strerror(r));
        return -1;
    }

    return uv_process_get_pid(&dial->_proc);
}

static void start(struct dial_s *dial, char *interface, char *proc, char *chat_file)
{
    if (!dial)
        return;

    pppd_module_t *pppd = &dial->pppd;

    dial->_proc.data = dial;
    dial->_pipe_read.data = dial;

    /* pipe initalize */
    uv_pipe_init(dial->loop, &dial->_pipe_read, 0);

    /* invoke pppd script */
    pppd->generate_args(pppd);
    pppd->pid = invoke_script(dial);
    if (pppd->pid == -1)
        return;

    uv_read_start((uv_stream_t*)&dial->_pipe_read, alloc_callback, on_stream_receive);

    dial->status = DIAL_PROCESSING;

    return;
}

static void stop(struct dial_s *dial)
{
    /* close stream read */
    uv_read_stop((uv_stream_t*)&dial->_pipe_read);
    uv_close((uv_handle_t*)&dial->_pipe_read, NULL);

    if (dial->_proc.status == 0)
        uv_process_kill(&dial->_proc, SIGTERM);

    uv_close((uv_handle_t*)&dial->_proc, NULL);

    if (dial->status != DIAL_REMAIN)
        dial->status = DIAL_UNOPENED;

    return;
}

static bool check_online(struct dial_s *dial)
{
    if (!dial)
        return false;

    pppd_module_t *pppd = &dial->pppd;

    if (pppd->pid < 0)
        return false;

    if (get_interface_ipaddr(pppd->interface, pppd->ip, sizeof(pppd->ip)) != 0)
        return false;

    return true;
}

int dial_init(dial_t *dial, uv_loop_t *loop)
{
    if (!dial || !loop)
        return -1;

    dial->loop = loop;
    dial->status = DIAL_UNOPENED;

    dial->start = start;
    dial->stop = stop;
    dial->online = check_online;

    pppd_module_init(&dial->pppd);

    return 0;
}
