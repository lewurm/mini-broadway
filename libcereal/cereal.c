/*
	libcereal -- a USB library for the BootMii platform

Copyright (C) 2010	Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#include <malloc.h>
#include <broadway.h>
#include <string.h>

#include <cereal.h>

int _irq_handler_ohci(u32 irq, void* data)
{
	u32 base;
	switch(irq) {
		default: /* Uhh... */
			/* Not for us... */
			return 1;
		case IRQ_HW_OHCI0:
			base = OHCI0_REG_BASE;
			break;
		case IRQ_HW_OHCI1:
			base = OHCI1_REG_BASE;
			break;
	}
	_cereal_ohci_handle_irq(base);
	(void)data;
	return 1;
}

int _irq_handler_ehci(u32 irq, void* data)
{
	u32 base;
	switch(irq) {
		default: /* Uhh... */
			/* Not for us... */
			return 1;
		case IRQ_HW_EHCI:
			base = EHCI_REG_BASE;
			break;
	}
	_cereal_ehci_handle_irq(base);
	(void)data;
	return 1;
}

int cereal_init()
{
	int ret = 0;
	return ret;
}

int cereal_quit()
{
	int ret = 0;
	return ret;
}

int cereal_install(int irq)
{
	int kind;
	switch(irq) {
		default: /* Uhh... */
			return 0;
		case IRQ_HW_OHCI0:
			kind = 0;
			break;
		case IRQ_HW_OHCI1:
			kind = 1;
			break;
		case IRQ_HW_EHCI:
			kind = 2;
			break;
	}
	/* Initialize the certain controller */
	if(kind != 2) {
		if(!_cereal_ohci_init(kind ? OHCI1_REG_BASE : OHCI0_REG_BASE))
			return 0;
		return irq_hw_register_handler(irq, _irq_handler_ohci, NULL);
	}else{
		if(!_cereal_ehci_init(EHCI_REG_BASE))
			return 0;
		return irq_hw_register_handler(irq, _irq_handler_ehci, NULL);
	}
}

int cereal_uninstall(int irq)
{
	if((irq != IRQ_HW_OHCI0) && (irq != IRQ_HW_OHCI1) && (irq != IRQ_HW_EHCI))
		return 0;
	if(irq_hw_get_handler(irq).exec)
		return irq_hw_register_handler(irq, NULL, NULL);
	return 0;
}

int cereal_install_driver(int irq, _cereal_drv_t** driver)
{
	int type;
	u32 base;
	switch(irq) {
		default: /* Uhh... */
			return 0;
		case IRQ_HW_OHCI0:
			type = 0;
			base = OHCI0_REG_BASE;
			break;
		case IRQ_HW_OHCI1:
			type = 0;
			base = OHCI1_REG_BASE;
			break;
		case IRQ_HW_EHCI:
			type = 1;
			base = EHCI_REG_BASE;
			break;
	}
	return _cereal_install_driver(_cereal_get_host(base, type), driver);
}

int cereal_remove_driver(int irq, _cereal_drv_t* driver)
{
	int type;
	u32 base;
	switch(irq) {
		default: /* Uhh... */
			return 0;
		case IRQ_HW_OHCI0:
			type = 0;
			base = OHCI0_REG_BASE;
			break;
		case IRQ_HW_OHCI1:
			type = 0;
			base = OHCI1_REG_BASE;
			break;
		case IRQ_HW_EHCI:
			type = 1;
			base = EHCI_REG_BASE;
			break;
	}
	return _cereal_remove_driver(_cereal_get_host(base, type), driver);
}
