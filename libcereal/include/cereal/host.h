/*
	libcereal -- a USB library for the BootMii platform

Copyright (C) 2010	Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#ifndef _CEREAL_HOST_H_
#define _CEREAL_HOST_H_

#include <types.h>
#include <broadway.h>

/* Driver info. */
typedef struct _cereal_drv_t {
	struct _cereal_drv_t*	next;
	struct _cereal_drv_t*	prev;
	
	char			name[64];
	char			build[64];
	char			author[64];
	/* host would be _cereal_host_t*, but it can't be defined yet :/ */
	int			(*init)  (void* host, struct _cereal_drv_t* me);
	int			(*fini)  (void* host, struct _cereal_drv_t* me);
	int			(*probe) (void* host, struct _cereal_drv_t* me);
	int			(*check) (void* host, struct _cereal_drv_t* me);
	int			(*remove)(void* host, struct _cereal_drv_t* me);
	
	void*			data;
} _cereal_drv_t;

/* Device info. */
typedef struct _cereal_dev_t {
	struct _cereal_dev_t*	next;
	struct _cereal_dev_t*	prev;
	
	int			port;
	int			speed;		/* 0 = Full, 1 = Slow */
	struct _cereal_host_t*	parent;		/* The host that this device is attached to */
	
	_cereal_drv_t*		driver;		/* The driver attached to this device */
	void*			extra;		/* Anything OHCI vs. EHCI specific */
} _cereal_dev_t;

/* USB host contexts. */
typedef struct _cereal_host_t {
	struct _cereal_host_t*	next;
	struct _cereal_host_t*	prev;
	int			type;		/* 0 = OHCI, 1 = EHCI */
	u32			base;
	_cereal_dev_t*		devices;	/* All attached devices */
	_cereal_drv_t*		drivers;	/* All installed drivers */
	void*			extra;		/* Anything OHCI vs. EHCI specific */
} _cereal_host_t;

/* The head of the host contexts */
extern struct _cereal_host_t* _cereal_hosts;

_cereal_host_t* _cereal_host_init(u32 base, int type);
_cereal_host_t* _cereal_get_host(u32 base, int type);
int _cereal_install_driver(_cereal_host_t* host, _cereal_drv_t** driver);
int _cereal_remove_driver( _cereal_host_t* host, _cereal_drv_t*  driver);
int _cereal_install_device(_cereal_host_t* host, _cereal_dev_t** device);
int _cereal_remove_device( _cereal_host_t* host, _cereal_dev_t*  device);

#endif /* _CEREAL_HOST_H_ */
