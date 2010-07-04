/*
	libdiskmii - a disk input/output library for the BootMii platform.

Copyright (C) 2009-2010		Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#ifndef __DISKMII_H__
#define __DISKMII_H__

#include <diskmii/ff.h>
#include <diskmii/diskio.h>

#define ELM_SD		0
#define ELM_NAND	1

int diskmii_init();
int diskmii_mountdevice(int drv);
int diskmii_remountdevice(int drv);
int diskmii_unmountdevice(int drv);
int diskmii_mount();
int diskmii_remount();
int diskmii_unmount();
int diskmii_is_inserted(int drv);

#endif
