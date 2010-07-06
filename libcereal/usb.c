/*
	libcereal -- a USB library for the BootMii platform

Copyright (C) 2010	Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#include <malloc.h>
#include <broadway.h>
#include <string.h>
#include <lists.h>

#include <cereal.h>

s8 usb_control_msg(_cereal_dev_t *dev, u8 requesttype, u8 request,
		   u16 value, u16 index, u16 length, u8 *buf, u16 timeout)
{
	struct usb_irp *irp = (struct usb_irp*)malloc(sizeof(struct usb_irp));
	irp->dev = dev;
	irp->endpoint = 0;
	
	irp->epsize = dev->bMaxPacketSize0;
	irp->type = USB_CTRL;
	
	buf[0] = requesttype;
	buf[1] = request;
	buf[2] = value & 0xFF;
	buf[3] = value >> 8;
	buf[4] = index & 0xFF;
	buf[5] = index >> 8;
	buf[6] = length & 0xFF;
	buf[7] = length >> 8;
	
	irp->buffer = buf;
	irp->len = length;
	irp->timeout = timeout;
	
	usb_submit_irp(irp);
	free(irp);
	
	return 0;
}

