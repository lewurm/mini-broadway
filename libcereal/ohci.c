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

static void _cereal_ohci_detect_devices(u32 base, _cereal_host_t* host, int port, u32 val)
{
	_cereal_dev_t* dev;
	dev = calloc(sizeof(_cereal_dev_t), 1);
	if(dev == NULL)
		return;
	dev->parent = host;
	dev->port = port;
	/* Speed of device */
	dev->speed = (val & RH_PS_LSDA) ? 1 : 0;
}

static void _cereal_ohci_handle_status_change(u32 base, _cereal_host_t* host, int port)
{
	u32 reg;
	u32 val;
	switch(port) {
		case 0:
			reg = OHCI_HC_RH_PORT_STATUS_1;
			break;
		case 1:
			reg = OHCI_HC_RH_PORT_STATUS_2;
			break;
		default:
			/* wat */
			return;
	}
	/* Read the HcRhPortStatus register */
	val = read32(base + reg);
	/* If we had a change in the connected status register, we are probably
	 * going to do something here. */
	if(val & RH_PS_CSC) {
		/* Clear the connected status change bit */
		write32(base + reg, RH_PS_CSC);
		/* We had a device inserted. Let's go detect it. */
		if(val & RH_PS_CCS) {
			/* Enable the Port */
			write32(base + reg, RH_PS_PES);
			/* Wait until the port gets enabled. NOTE! THIS MIGHT HANG!!!! FIGURE SOMETHING OUT! */
			while(!(read32(base + reg) & RH_PS_PES));
			/* Reset the Port */
			write32(base + reg, RH_PS_PRS);
			/* Wait until the port finishes resetting. NOTE! THIS MIGHT HANG!!!! FIGURE SOMETHING OUT! */
			while(!(read32(base + reg) & RH_PS_PRSC));
			
			/* Now we're good to detect our device :P */
			_cereal_ohci_detect_devices(base, host, port, val);
		}else{ /* We had a device removed. We need to go delete it. */
			_cereal_ohci_delete_devices(base, host, port);
		}
	}
}

void _cereal_ohci_handle_irq(u32 base)
{
	u32 flags;
	u32 enabled;
	u32 useflags;
	_cereal_host_t* host;
	host = _cereal_get_host(base, 0);
	
	/* Read which OHCI interrupts fired */
	flags = read32(base + OHCI_HC_INT_STATUS);
	enabled = read32(base + OHCI_HC_INT_ENABLE);
	useflags = flags & enabled;
	if(flags == 0xFFFFFFFF) {
		/* Something bad happened! */
		printf("OHCI Error occurred!\n");
		return;
	}
	
	if(useflags == 0) {
		/* Spurious interrupt. Who cares. */
		return;
	}
	
	if(useflags & OHCI_INTR_RHSC) {
		/* Root Hub Status Change! */
		printf("=== Roothub Status Change on %s ===\n", base == OHCI0_REG_BASE ? "OHCI0" : "OHCI1");
		printf("OHCI_HC_RH_DESCRIPTOR_A:\t0x%08X\n", read32(base + OHCI_HC_RH_DESCRIPTOR_A));
		printf("OHCI_HC_RH_DESCRIPTOR_B:\t0x%08X\n", read32(base + OHCI_HC_RH_DESCRIPTOR_B));
		printf("OHCI_HC_RH_STATUS:\t\t0x%08X\n", read32(base + OHCI_HC_RH_STATUS));
		printf("OHCI_HC_RH_PORT_STATUS_1:\t0x%08X\n", read32(base + OHCI_HC_RH_PORT_STATUS_1));
		printf("OHCI_HC_RH_PORT_STATUS_2:\t0x%08X\n", read32(base + OHCI_HC_RH_PORT_STATUS_2));
		/* We have 2 Port Statuses, so we need to handle them both the same. */
		_cereal_ohci_handle_status_change(base, host, 0);
		_cereal_ohci_handle_status_change(base, host, 1);
	}
}

int _cereal_ohci_init(u32 base)
{
	_cereal_host_t* host;
	host = _cereal_host_init(base, 0);
	if(host == NULL)
		return 0;
	return 1;
}
