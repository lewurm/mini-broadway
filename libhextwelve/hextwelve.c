/*
	libhextwelve -- a USB library for the BootMii platform
 
Copyright (C) 2009	Alex Marshall <SquidMan72@gmail.com>
 
# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#include <hextwelve/core/core.h>
#include <hextwelve/host/host.h>
#include <hextwelve/core/usb.h>
#include <hextwelve/usbspec/usb11spec.h>
#include <hextwelve/lib/list.h>
#include <malloc.h>
#include <broadway.h>
#include <string.h>

int _irq_handler_ohci(u32 irq)
{
	u32 base = OHCI0_REG_BASE;
	if(irq == IRQ_OHCI1)
		base = OHCI1_REG_BASE;
	hcdi_irq(base);
	(void)irq;
	return 1;
}

int hextwelve_init()
{
	if(!irq_register_handler(IRQ_OHCI0, _irq_handler_ohci))
		return 0;
	if(!irq_register_handler(IRQ_OHCI1, _irq_handler_ohci))
		return 0;
	return 1;
}

int hextwelve_quit()
{
	if(irq_get_handler(IRQ_OHCI0))
		if(!irq_register_handler(IRQ_OHCI0, NULL))
			return 0;
	if(irq_get_handler(IRQ_OHCI1))
		if(!irq_register_handler(IRQ_OHCI1, NULL))
			return 0;
	return 1;
}

