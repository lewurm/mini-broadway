/*
	BootMii - a Free Software replacement for the Nintendo/BroadOn bootloader.
	Requires mini.

	Input-handling functions for BootMii. Inspired by GC_PAD.c from GCLIB.

Copyright (C) 2008, 2009	Haxx Enterprises <bushing@gmail.com>
Copyright (C) 2009		Andre Heider "dhewg" <dhewg@wiibrew.org>
Copyright (C) 2009		John Kelley <wiidev@kelley.ca>
Copyright (C) 2009		bLAStY <blasty@bootmii.org>
Copyright (C) 2009-2010		Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#include <broadway.h>
#include <input.h>
#include <string.h>

#define PADREG(x)				(0x0D006400 + (x)*4)
#define PAD_CMDWORD_MAKE(cmd, arg0, arg1)	((cmd) << 16 | (arg0) << 8 | (arg1))
#define PAD_CMD_ENABLE_POLL			(0x40)
#define SIC0OUTBUF				PADREG(0)
#define SIC1OUTBUF				PADREG(3)
#define SIC2OUTBUF				PADREG(6)
#define SIC3OUTBUF				PADREG(9)
#define SIPOLL					PADREG(12)
#define SISR					PADREG(14)

static GC_Pad _pad;

static void gcpad_init(void)
{
	write32(SIC0OUTBUF,  PAD_CMDWORD_MAKE(PAD_CMD_ENABLE_POLL, 0x03, 0x00)); /* Enable read on all GC pads */
	write32(SIC1OUTBUF,  PAD_CMDWORD_MAKE(PAD_CMD_ENABLE_POLL, 0x03, 0x00));
	write32(SIC2OUTBUF,  PAD_CMDWORD_MAKE(PAD_CMD_ENABLE_POLL, 0x03, 0x00));
	write32(SIC3OUTBUF,  PAD_CMDWORD_MAKE(PAD_CMD_ENABLE_POLL, 0x03, 0x00));
	write32(SIPOLL,      0x000701f0); // enable poll chan 1, X = 7, Y = 1
	write32(SISR,        0x80000000); // transfer all buffer
}

static void gpio_init(void)
{
	// setup power and eject button hollywood IRQ for PPC
	mask32(HW_GPIO1OWNER,           0, 0x41); // set GPIO owner to PPC
	irq_hw_enable(IRQ_GPIO1B);
	mask32(HW_GPIO1BINTENABLE,      0, 0x41);
	mask32(HW_GPIO1BINTLVL,         0, 0x41);
}

void input_init(void)
{
	memset(&_pad, 0, sizeof(GC_Pad));

	gpio_init();
	gcpad_init();

	// Check for any pending GPIO irq's, which should be ACK'd so we don't get ghost presses later.
	// Try over and over again, until we are out of them.
	while(read32(HW_PPCIRQFLAG) & IRQF_GPIO1B) {
		if (read32(HW_GPIO1INTFLAG) & 1) {
			while(read32(HW_GPIO1BIN) & 1);
			write32(HW_GPIO1BINTFLAG, 1);
		}else if(read32(HW_GPIO1INTFLAG) & 0x40) {
			write32(HW_GPIO1BINTFLAG, 0x40);
		}

		write32(HW_PPCIRQFLAG, IRQF_GPIO1B);
	}

	// No IRQ's left to be ACK'd, continue our business.
}

u16 pad_read(GC_Pad *pad, int chan)
{
	u32 pdata = read32(PADREG(3 * chan + 1));
	u32 pdata2 = read32(PADREG(3 * chan + 2));

	u16 btns = pdata >> 16;

	if (pad) {
		u16 prev = pad->btns_held;

		pad->btns_held = btns;
		pad->btns_up = prev & ~btns;
		pad->btns_down = btns & (btns ^ prev);

		pad->x = 128 + ((pdata >> 8) & 0xff);
		pad->y = 128 - (pdata & 0xff);

		pad->cx = 128 + (pdata2 >> 24);
		pad->cy = 128 - ((pdata2 >> 16) & 0xff);
		pad->l = (pdata2 >> 8) & 0xff;
		pad->r = pdata2 & 0xff;

		return pad->btns_down;
	}

	return btns;
}

// TODO: Hackity hack, prevent ghost presses
static u8 reset_delay = 5;

u16 gpio_read(void)
{
	u16 res = 0;
	u32 irq_flag = 0;

	if(!((read32(BW_PI_IRQFLAG) >> 16) & 1) && reset_delay == 0) {
		res |= GPIO_RESET;
		reset_delay = 5;
	}

	if(reset_delay > 0)
		reset_delay--;

	if(read32(HW_PPCIRQFLAG) & (1<<10)) {
		irq_flag = read32(HW_GPIO1INTFLAG);

		if (irq_flag & 1) {
			res |= GPIO_POWER;

			while(read32(HW_GPIO1BIN) & 1);
			write32(HW_GPIO1BINTFLAG, 1);
		} else if (irq_flag & 0x40) {
			res |= GPIO_EJECT;

			while(read32(HW_GPIO1BIN) & 0x40);
			write32(HW_GPIO1BINTFLAG, 0x40);
		}

		write32(HW_PPCIRQFLAG, IRQF_GPIO1B); // ack GPIO irq
	}

	return res;
}

u16 input_read(void)
{
	return pad_read(&_pad, 0) | gpio_read();
}

u16 input_wait(void)
{
	u16 res;

	do {
		udelay(20000);
		res = input_read();
	} while (!(res & PAD_ANY));
	
	return res;
}

