/*
	libhextwelve -- a USB library for the BootMii platform

Copyright (C) 2009	Alex Marshall <SquidMan72@gmail.com>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#ifndef _HEXTWELVE_H_
#define _HEXTWELVE_H_

#include <types.h>
#include <broadway.h>

#include <hextwelve/core/core.h>
#include <hextwelve/core/usb.h>
#include <hextwelve/drivers/class/hid.h>
#include <hextwelve/drivers/class/hub.h>
#include <hextwelve/drivers/class/storage.h>
#include <hextwelve/drivers/mon/mon.h>
#include <hextwelve/host/host.h>
#include <hextwelve/host/ohci.h>
#include <hextwelve/lib/list.h>
#include <hextwelve/usbspec/usb11spec.h>

int hextwelve_init();
int hextwelve_quit();

#endif /* _HEXTWELVE_H_ */
