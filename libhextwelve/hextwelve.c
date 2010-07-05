/*
	libhextwelve -- a USB library for the BootMii platform
 
Copyright (C) 2009-2010		Alex Marshall <trap15@raidenii.net>
 
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

int _irq_handler_ohci(u32 irq, void* data)
{
	u32 base;
	switch(irq) {
		default: /* Uhh... */
		case IRQ_HW_OHCI0:
			base = OHCI0_REG_BASE;
			break;
		case IRQ_HW_OHCI1:
			base = OHCI1_REG_BASE;
			break;
	}
	hcdi_irq(base);
	(void)data;
	return 1;
}

int hextwelve_init()
{
	int ret = 0;
	if(!irq_hw_register_handler(IRQ_HW_OHCI0, _irq_handler_ohci, NULL))
		ret |= 1;
	if(!irq_hw_register_handler(IRQ_HW_OHCI1, _irq_handler_ohci, NULL))
		ret |= 2;
	return ret;
}

int hextwelve_quit()
{
	int ret = 0;
	if(irq_hw_get_handler(IRQ_HW_OHCI0).exec)
		if(!irq_hw_register_handler(IRQ_HW_OHCI0, NULL, NULL))
			ret |= 1;
	if(irq_hw_get_handler(IRQ_HW_OHCI1).exec)
		if(!irq_hw_register_handler(IRQ_HW_OHCI1, NULL, NULL))
			ret |= 2;
	return ret;
}

