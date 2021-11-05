#ifndef _USBDEVICE_H_
#define _USBDEVICE_H_

#include <stdbool.h>

#define USBDEVICE_4G_USB0   "/dev/ttyUSB0"
#define USBDEVICE_4G_USB1   "/dev/ttyUSB1"
#define USBDEVICE_4G_USB2   "/dev/ttyUSB2"
#define USBDEVICE_4G_USB3   "/dev/ttyUSB3"
#define USBDEVICE_4G_USB4   "/dev/ttyUSB4"

bool check_usbdevice_status(const char *vid, const char *did);
bool check_usbport_status(const char *port);

#endif
