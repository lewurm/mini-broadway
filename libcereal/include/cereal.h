/*
	libcereal -- a USB library for the BootMii platform

Copyright (C) 2010	Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#ifndef _CEREAL_H_
#define _CEREAL_H_

#include <types.h>
#include <broadway.h>
#include <cereal/ohci.h>
#include <cereal/ehci.h>
#include <cereal/host.h>

int cereal_init();
int cereal_quit();
int cereal_install(  int irq);
int cereal_uninstall(int irq);
int cereal_install_driver(int irq, _cereal_drv_t** driver);
int cereal_remove_driver( int irq, _cereal_drv_t*  driver);

#endif /* _CEREAL_H_ */
