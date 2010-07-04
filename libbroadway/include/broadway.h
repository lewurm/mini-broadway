/*  broadway.h -- all the includes you'll ever need for libbroadway.
                  also includes some functions that didn't have
                  a place anywhere else.

Copyright (C) 2009-2010		Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#ifndef _LIBBROADWAY_H
#define _LIBBROADWAY_H

#include <types.h>

#include <broadway/bootmii_ppc.h>
#include <broadway/hollywood.h>
#include <broadway/irq.h>
#include <broadway/ipc.h>
#include <broadway/mini_ipc.h>

#define ATTRIBUTE_ALIGN(n)	__attribute__((aligned (n)))

#endif

