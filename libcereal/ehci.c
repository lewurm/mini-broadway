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

void _cereal_ehci_handle_irq(u32 base)
{
	_cereal_host_t* host;
	host = _cereal_get_host(base, 1);
	/* Do IRQ junk */
}

int _cereal_ehci_init(u32 base)
{
	_cereal_host_t* host;
	host = _cereal_host_init(base, 1);
	if(host == NULL)
		return 0;
	return 1;
}
