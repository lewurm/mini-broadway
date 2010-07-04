/*
	libdiskmii - a disk input/output library for the BootMii platform.
 
Copyright (C) 2009-2010		Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#include <malloc.h>
#include <string.h>

#include <diskmii.h>
#include <diskmii/ff.h>
#include <diskmii/ffconf.h>
#include <diskmii/diskio.h>

static int _diskmii_init = 0;
static FATFS _diskmii_fs[_DRIVES];
#define VALID_DRIVE(n)	(((n) >= 0) && (n < _DRIVES))

int diskmii_init()
{
	_diskmii_init = 1;
	return 1;
}

int diskmii_mountdevice(int drv)
{
	DSTATUS stat;
	if(!_diskmii_init)
		return STA_NOINIT;
	if(!VALID_DRIVE(drv))
		return STA_NOINIT;
	f_mount(drv, NULL);		/* Unmount the drive first */
	stat = disk_initialize(drv);	/* Init the disk */
	if(stat & STA_NODISK)
		return STA_NODISK;
	if(stat & STA_NOINIT)
		return STA_NOINIT;
	memset(&(_diskmii_fs[drv]), 0, sizeof(FATFS));
	return f_mount(drv, &(_diskmii_fs[drv]));
}

int diskmii_unmountdevice(int drv)
{
	if(!_diskmii_init)
		return STA_NOINIT;
	if(!VALID_DRIVE(drv))
		return 0;
	memset(&(_diskmii_fs[drv]), 0, sizeof(FATFS));
	f_mount(drv, NULL);
	return 1;
}

int diskmii_remountdevice(int drv)
{
	if(!diskmii_unmountdevice(drv))
		return 0;
	if(diskmii_mountdevice(drv) != RES_OK)
		return 0;
	return 1;
}

int diskmii_mount()
{
	int i;
	for(i = 0; i < _DRIVES; i++)
		if(diskmii_mountdevice(i) != RES_OK)
			return 0;
	return 1;
}

int diskmii_remount()
{
	int i;
	for(i = 0; i < _DRIVES; i++)
		if(!diskmii_remountdevice(i))
			return 0;
	return 1;
}

int diskmii_unmount()
{
	int i;
	for(i = 0; i < _DRIVES; i++)
		diskmii_unmountdevice(i);
	return 1;
}

int diskmii_is_inserted(int drv)
{
	if(!_diskmii_init)
		return STA_NOINIT;
	if(!VALID_DRIVE(drv))
		return 0;
	return ((disk_status(drv) == STA_NODISK) ? 0 : 1);
}

