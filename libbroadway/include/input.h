/*
	BootMii - a Free Software replacement for the Nintendo/BroadOn bootloader.
	Requires mini.

Copyright (C) 2009		Andre Heider "dhewg" <dhewg@wiibrew.org>
Copyright (C) 2009		John Kelley <wiidev@kelley.ca>
Copyright (C) 2009		bLAStY <blasty@bootmii.org>
Copyright (C) 2009-2010		Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#ifndef __INPUT_H__
#define __INPUT_H__

#define PAD_LEFT	(1 <<  0)
#define PAD_RIGHT	(1 <<  1)
#define PAD_DOWN	(1 <<  2)
#define PAD_UP		(1 <<  3)
#define PAD_Z		(1 <<  4)
#define PAD_RS		(1 <<  5)
#define PAD_LS		(1 <<  6)
//    unused		(1 <<  7)
#define PAD_A		(1 <<  8)
#define PAD_B		(1 <<  9)
#define PAD_X		(1 << 10)
#define PAD_Y		(1 << 11)
#define PAD_START	(1 << 12)
#define PAD_ANY		(PAD_LEFT | PAD_RIGHT | PAD_DOWN | PAD_UP | \
			 PAD_Z    | PAD_RS    | PAD_LS   | PAD_START | \
			 PAD_A    | PAD_B     | PAD_X    | PAD_Y)

#define GPIO_POWER	(1 <<  1)
#define GPIO_RESET	(1 <<  8)
#define GPIO_EJECT	(1 << 12)

typedef struct {
        u16 btns_held;
        u16 btns_up;
        u16 btns_down;
	s8 x, y, cx, cy;
	u8 l, r;
} GC_Pad;

void input_init(void);

u16 pad_read(GC_Pad *pad, int chan);
u16 gpio_read(void);

u16 input_read(void);
u16 input_wait(void);

#endif
