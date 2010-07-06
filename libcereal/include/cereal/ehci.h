/*
	libcereal -- a USB library for the BootMii platform

Copyright (C) 2010	Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#ifndef _CEREAL_EHCI_H_
#define _CEREAL_EHCI_H_

#include <types.h>
#include <broadway.h>

void _cereal_ehci_handle_irq(u32 base);
int  _cereal_ehci_init(u32 base);

#endif /* _CEREAL_EHCI_H_ */
