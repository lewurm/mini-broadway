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

struct _cereal_host_t* _cereal_hosts = NULL;

_cereal_host_t* _cereal_host_init(u32 base, int type)
{
	_cereal_host_t* host;
	host = linked_add_last(&_cereal_hosts, linked_make(sizeof(_cereal_host_t)));
	if(host == NULL)
		return NULL;
	
	/* Do all the other init I need to do here */
	host->base = base;
	host->type = type;
	
	return host;
}

_cereal_host_t* _cereal_get_host(u32 base, int type)
{
	_cereal_host_t* walk;
	linked_walk(_cereal_hosts, (walk->base != base) || (walk->type != type));
	return walk;
}

int _cereal_install_driver(_cereal_host_t* host, _cereal_drv_t** driver)
{
	_cereal_drv_t* drv;
	drv = linked_add_last(&(host->drivers), *driver);
	if(drv == NULL)
		return 0;
	*driver = drv;
	(*driver)->init(host, *driver);
	return 1;
}

int _cereal_remove_driver(_cereal_host_t* host, _cereal_drv_t* driver)
{
	linked_del(host->drivers, driver);
	driver->fini(host, driver);
	return 1;
}

int _cereal_install_device(_cereal_host_t* host, _cereal_dev_t** device)
{
	_cereal_dev_t* dev;
	dev = linked_add_last(&(host->devices), *device);
	if(dev == NULL)
		return 0;
	*device = dev;
	return 1;
}

int _cereal_remove_device(_cereal_host_t* host, _cereal_dev_t* device)
{
	linked_del(host->devices, device);
	return 1;
}
