#include "serial.h"

static bool check_baudrate(int baudrate)
{
    switch (baudrate)
    {
    case 1200:
    case 1800:
    case 9600:
    case 19200:
    case 38400:
    case 57600:
    case 115200:
    case 460800:
    case 921600:
        return true;
    default:
        return false;
    }
}

static bool initalize_default(struct serial_s *thiz)
{
    if (!thiz)
        return false;

    thiz->_uart_name = (char *)"/dev/ttyUSB0";
    
    thiz->_attr.baudrate = 57600;
    thiz->_attr.data_bits = 8;
    thiz->_attr.stop_bits = 1;
    thiz->_attr.parity = 'N';
    thiz->_attr.hardware_control = false;

    return true;
}

static bool setup_port_baudrate(struct serial_s *thiz, int baudrate)
{
    if (!thiz)
        return false;

    if (baudrate <= 0)
        return false;

    if (thiz->check_baudrate(baudrate))
        thiz->_attr.baudrate = baudrate;

    thiz->setup(thiz, &thiz->_attr);

    return true;
}

static bool setup_port(struct serial_s *thiz, struct serial_attr_s *attr)
{
    if (!thiz || !attr)
        return false;

    if (!isatty(thiz->fd))
    {
        fprintf(stderr, "\nERROR: file descriptor %d is NOT a serial port\n", thiz->fd);
        return false;
    }

    struct termios config;

    if (tcgetattr(thiz->fd, &config) < 0)
    {
        fprintf(stderr, "\nERROR: could not read configuration of fd %d\n", thiz->fd);
        return false;
    }

    /*
     * Input flags - Turn off input processing
	 * convert break to null byte, no CR to NL translation,
	 * no NL to CR translation, don't mark parity errors or breaks
	 * no input parity check, don't strip high bit off,
	 * no XON/XOFF software flow control
     */
	config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
						                    INLCR | PARMRK | INPCK | ISTRIP | IXON);

    /*
     * Output flags - Turn off output processing
	 * no CR to NL translation, no NL to CR-NL translation,
	 * no NL to CR translation, no column 0 CR suppression,
	 * no Ctrl-D suppression, no fill characters, no case mapping,
	 * no local output processing
     */
	config.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
						                    ONOCR | OFILL | OPOST);

    #ifdef OLCUC
		config.c_oflag &= ~OLCUC;
	#endif

	#ifdef ONOEOT
		config.c_oflag &= ~ONOEOT;
	#endif

    /*
     * No line processing:
	 * echo off, echo newline off, canonical mode off,
	 * extended input processing off, signal chars off
     */
	config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

    /* clear current char size mask */
	config.c_cflag &= ~(CSIZE);

    /* parity checking */
    switch (attr->parity)
    {
    case 'N':
        config.c_cflag &= ~(PARENB);
        break;
    case 'O':
        config.c_cflag |= (PARENB | PARODD);
        break;
    case 'E':
        config.c_cflag |= (PARENB | PARODD);
        break;
    default:
        fprintf(stderr, "\nERROR: invail parity argument [%d], it must be 'N'/'O' or 'E'\n", attr->parity);
        return false;
        break;
    }

    /* force data bit input */
    if (attr->data_bits == 7)
	    config.c_cflag |= CS7;
    else
        config.c_cflag |= CS8;

    /*
     * One input byte is enough to return from read()
	 * Inter-character timer off
     */
	config.c_cc[VMIN]  = 1;
	config.c_cc[VTIME] = 10;

    /*
     * Get the current options for the port
	 *  struct termios options;
	 *  tcgetattr(fd, &options);
     */

	/* Apply baudrate */
    switch (attr->baudrate)
    {
    case 1200:
        if (cfsetispeed(&config, B1200) < 0 || cfsetospeed(&config, B1200) < 0)
		{
			fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", thiz->_attr.baudrate);
			return false;
		}
        break;

    case 1800:
		cfsetispeed(&config, B1800);
		cfsetospeed(&config, B1800);
		break;

    case 9600:
		cfsetispeed(&config, B9600);
		cfsetospeed(&config, B9600);
		break;

    case 19200:
		cfsetispeed(&config, B19200);
		cfsetospeed(&config, B19200);
		break;

    case 38400:
		if (cfsetispeed(&config, B38400) < 0 || cfsetospeed(&config, B38400) < 0)
		{
			fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", thiz->_attr.baudrate);
			return false;
		}
		break;

    case 57600:
		if (cfsetispeed(&config, B57600) < 0 || cfsetospeed(&config, B57600) < 0)
		{
			fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", thiz->_attr.baudrate);
			return false;
		}
		break;

    case 115200:
		if (cfsetispeed(&config, B115200) < 0 || cfsetospeed(&config, B115200) < 0)
		{
			fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", thiz->_attr.baudrate);
			return false;
		}
		break;

    case 460800:
		if (cfsetispeed(&config, B460800) < 0 || cfsetospeed(&config, B460800) < 0)
		{
			fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", thiz->_attr.baudrate);
			return false;
		}
		break;

	case 921600:
		if (cfsetispeed(&config, B921600) < 0 || cfsetospeed(&config, B921600) < 0)
		{
			fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", thiz->_attr.baudrate);
			return false;
		}
		break;

    default:
        fprintf(stderr, "ERROR: Desired baud rate %d could not be set, aborting.\n", thiz->_attr.baudrate);
        return false;

        break;
    }

    /* Apply the configuration */
    if(tcsetattr(thiz->fd, TCSAFLUSH, &config) < 0)
	{
		fprintf(stderr, "\nERROR: could not set configuration of fd %d\n", thiz->fd);
		return false;
	}

    return true;
}

static int open_port(struct serial_s *thiz, const char *_uart_name)
{
    if (!thiz)
        return -1;

    if (thiz->fd > 0)
        goto err;

    if (_uart_name)
        thiz->_uart_name = _uart_name;

    /*
     * Open serial port
	 * O_RDWR - Read and write
	 * O_NOCTTY - Ignore special chars like CTRL-C
     */
    thiz->fd = open(thiz->_uart_name, O_RDWR | O_NOCTTY | O_NDELAY);

    /*
     * Check for error
     */
    if (thiz->fd < 0)
        goto err;

    fcntl(thiz->fd, F_SETFL, 0);

err:
    return thiz->fd;
}

static void close_port(struct serial_s *thiz)
{
    if (!thiz)
        return;

    /*
     * Check fd is available
     */
    if (thiz->fd > 0)
        close(thiz->fd);

    thiz->fd = -1;

    return;
}

static int read_port(struct serial_s *thiz, char *rb, int len)
{
    if (!thiz)
        return -1;

    if (!rb || !len)
        return -1;

    return read(thiz->fd, rb, len);
}

static int write_port(struct serial_s *thiz, const char *wb, int len)
{
    if (!thiz)
        return -1;

    if (!wb || !len)
        return -1;

    return write(thiz->fd, wb, len);
}

bool serial_initalize(struct serial_s *port)
{
    if (!port)
        return false;
    
    port->setup_baudrate = setup_port_baudrate;
    port->setup = setup_port;
    port->open = open_port;
    port->close = close_port;
    port->_default = initalize_default;
    port->read = read_port;
    port->write = write_port;
    port->check_baudrate = check_baudrate;

    port->fd = -1;
    port->_default(port);

    return true;
}
