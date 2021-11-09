#include "cmdhandle.h"
#include "atconfigure.h"
#include "atcommand.h"
#include "cmdhandle.h"

#define ATCHANNEL_BUFFER_MAX_SIZE   1024

static char receivebuffer[ATCHANNEL_BUFFER_MAX_SIZE];

static void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    buf->base = receivebuffer;
    buf->len = ATCHANNEL_BUFFER_MAX_SIZE;
    memset(receivebuffer, 0, ATCHANNEL_BUFFER_MAX_SIZE);
}

static void on_stream_receive(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    struct atchannel_s *atchannel = stream->data;
    char *vail;

    if (nread <= 0) {
        fprintf(stderr, "\nERROR: Failed to receive atchannel stream, code: %d, err: %s\n", nread, uv_strerror(nread));

        /* loss ttyUSB2 port (AT^RESET command or USB device loss power) */
        if (nread == UV_EOF)  {
            atchannel->reset = ATRESET_RESETTING;
            atchannel->stop(atchannel);
        }
    }

    // fprintf(stderr, "%s\n", buf->base);
    // fprintf(stderr, "----------------------------------------------\n\n");

    command_handle(buf->base, atchannel);

    return;
}

static void at_command_sendto_processor(uv_async_t *handle)
{
    uv_buf_t *wb;
    struct atchannel_s *atchannel = handle->data;
    struct serial_s *serial = &atchannel->serial;

    do {
        wb = (uv_buf_t*)atchannel->_queue->poll(atchannel->_queue);

        if (wb) {
            serial->write(serial, wb->base, wb->len);

            free(wb->base);
            free(wb);
        }
    } while (wb);
}

static int at_command_sendto(struct atchannel_s *atchannel, uv_buf_t *wb)
{
    int r;

    if (!wb)
        return -1;

    if (!wb->base)
        goto err0;

    if (!atchannel)
        goto err1;

    r = atchannel->_queue->offer(atchannel->_queue, wb);
    if (r) {
        uv_async_send(&atchannel->_async);
        return 0;
    }

err1:
    free(wb->base);
err0:
    free(wb);
    return -1;
}

static void start(struct atchannel_s *atchannel, const char *uart_name, int netmode)
{
    if (!atchannel || !atchannel->loop)
        return;

    struct serial_s *serial = &atchannel->serial;

    if (netmode > -1)
        atchannel->netmode = netmode;

    if (serial_initalize(serial) != true)
        return;

    if (uart_name)
        atchannel->uart_name = (char*)uart_name;

    serial->fd = serial->open(serial, atchannel->uart_name);
    if (serial->fd < 0)
        return;

    if (serial->setup_baudrate(serial, atchannel->baudrate) != true)
        goto err0;

    /* 4G module params */
    if (!atchannel->me909s)
        atchannel->me909s = get_shm_4Gmodule();

    /* AT-Command URC configure */
    atchannel->_pipe.data = atchannel;
    uv_pipe_init(atchannel->loop, &atchannel->_pipe, 0);
    uv_pipe_open(&atchannel->_pipe, serial->fd);

    /* AT-Command write queue initalize */
    atchannel->_queue = commu_queue(1000);

    /* AT-Command async communication */
    atchannel->_async.data = atchannel;
    uv_async_init(atchannel->loop, &atchannel->_async, at_command_sendto_processor);

    /* Automatic adaptation */
    atchannel->_timer.data = atchannel;
    uv_timer_init(atchannel->loop, &atchannel->_timer);
    uv_timer_start(&atchannel->_timer, (uv_timer_cb)atconfigure_handler, 1000, 1000);

    uv_read_start((uv_stream_t*)&atchannel->_pipe, alloc_buffer, on_stream_receive);

    atchannel->status = ATCHANNEL_PROCESSING;

    return;

err1:
    uv_close((uv_handle_t*)&atchannel->_pipe, NULL);
err0:
    serial->close(serial);
    return;
}

static void stop(struct atchannel_s *atchannel)
{
    if (!atchannel)
        return;

    /* auto configure timer release */
    uv_timer_stop(&atchannel->_timer);
    uv_close((uv_handle_t*)&atchannel->_timer, NULL);

    /* Async release */
    uv_close((uv_handle_t*)&atchannel->_async, NULL);

    if (atchannel->_queue) {
        atchannel->_queue->free(atchannel->_queue);
        atchannel->_queue = NULL;
    }

    /* Serial pipe close */
    uv_read_stop((uv_stream_t *)&atchannel->_pipe);
    uv_close((uv_handle_t*)&atchannel->_pipe, NULL);

    /* Close serial port */
    atchannel->serial.close(&atchannel->serial);

    atchannel->status = ATCHANNEL_UNOPENED;
}

int atchannel_init(atchannel_t *atchannel, uv_loop_t *loop)
{
    if (!atchannel || !loop)
        return -1;

    atchannel->loop = loop;

    atchannel->baudrate = 115200;
    atchannel->uart_name = ME909S_USB_PORT;
    atchannel->status = ATCHANNEL_UNOPENED;
    atchannel->reset = ATRESET_NOACTIONS;

    atchannel->me909s = get_shm_4Gmodule();

    atchannel->netmode = ME909S_NETMODE_AUTO;

    atchannel->start = start;
    atchannel->stop = stop;
    atchannel->sendto = at_command_sendto;
    atchannel->determine_simswitch = determine_simswitch;

    atchannel->command.indentification = atcommand_indentification;
    atchannel->command.sysinfoex = atcommand_sysinfoex;
    atchannel->command.unconnect = atcommand_unconnect;
    atchannel->command.ledctrl_set = atcommand_ledctrl_set;
    atchannel->command.ledctrl_get = atcommand_ledctrl_get;
    atchannel->command.syscfgex_set = atcommand_netmode_set;
    atchannel->command.syscfgex_get = atcommand_netmode_get;
    atchannel->command.ipcfl_set = atcommand_ipcfl_set;
    atchannel->command.ipcfl_get = atcommand_ipcfl_get;
    atchannel->command.eCall_set = atcommand_eCall_set;
    atchannel->command.eCall_get = atcommand_eCall_get;
    atchannel->command.cpin_set = atcommand_cpin_set;
    atchannel->command.cpin_get = atcommand_cpin_get;
    atchannel->command.simswitch_set = atcommand_simswitch_set;
    atchannel->command.simswitch_get = atcommand_simswitch_get;
    atchannel->command.network_set = atcommand_network_set;
    atchannel->command.network_get = atcommand_network_get;
    atchannel->command.monsc = atcommand_monsc;
    atchannel->command.signal = atcommand_sim_signal;

    return 0;
}
