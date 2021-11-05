#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 * The following two non-standard baudrates should have been defined by the system
 * If not, just fallback to number
 */ 
#ifndef B460800
#define B460800 460800
#endif

#ifndef B921600
#define B921600 921600
#endif

// ------------------------------------------------------------------------------
//   Serial Ports 
// ------------------------------------------------------------------------------

/**
 * Serial Port Attribute
 * 
 * This object is the definition of serial port's attribute. 
 * 
 * contain :
 *  @baudrate: baud rate
 *  @data_bits: data bits
 *  @stop_bits: stop bits
 *  @parity: parity check, Odd('O') Even('E') or None('N')
 *  @hardware_control: hardware control
 * 
 * General configuration:
 *  baudrate:   57600
 *  data_bits:  8
 *  stop_bits:  1
 *  parity:     false
 *  hardware_control: false
 */
struct serial_attr_s {
    int baudrate;
    int data_bits;
    int stop_bits;
    char parity;
    bool hardware_control;
};

/*
 * Serial Port Class
 *
 * This object handles the opening and closing of the offboard computer's
 * serial port over which we'll communicate.  It also has methods to write
 * a byte stream buffer.  It's just a serialization interface.  To help with 
 * read and write pthreading.
 */
struct serial_s {
    /** 
     * setup port attribute (baudrate)
     * 
     * @thiz: this
     * @baudrate: baud rate
     * @return true if setup success, else false
     */
    bool (*setup_baudrate)(struct serial_s *thiz, int baudrate);

    /** 
     * setup port attribute except baudrate
     * 
     * @thiz: this
     * @attr: serial port attribute
     * @return true if setup success, else false
     */
    bool (*setup)(struct serial_s *thiz, struct serial_attr_s *attr);

    /** 
     * open serial port
     * 
     * @thiz: this
     * @_uart_name: serial port name
     * @return file descriptor if success, else -1
     */
    int (*open)(struct serial_s *thiz, const char *_uart_name);

    /** 
     * close serial port
     * 
     * @thiz: this
     * @return none
     */
    void (*close)(struct serial_s *thiz);

    /** 
     * read data
     * 
     * @thiz: this
     * @rb: read buffer
     * @len: read bytes
     * @return the number of bytes read is returned if success, else -1
     */
    int (*read)(struct serial_s *thiz, char *rb, int len);
    
    /** 
     * write data
     * 
     * @thiz: this
     * @wb: write buffer
     * @len: write bytes
     * @return the number of bytes write is returned if success, else -1
     */
    int (*write)(struct serial_s *thiz, const char *wb, int len);

    /** 
     * check baudrate is vailable
     * 
     * @baudrate: baud rate
     * @return true if baudrate is vail, else false
     */
    bool (*check_baudrate)(int baudrate);

    int fd;
    const char *_uart_name;
    struct serial_attr_s _attr;

    /** 
     * serial port initalize default
     * 
     * @thiz: this
     * @return true if success, else false
     * 
     * @note default configuration
     *  _uart_name: "/dev/ttyUSB0"
     *  _attr: { 
            57600, 
            8, 
            1, 
            false, 
            false 
        }
     */
    bool (*_default)(struct serial_s *thiz);
};

bool serial_initalize(struct serial_s *port);

#endif
