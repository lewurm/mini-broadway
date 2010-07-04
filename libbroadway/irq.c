/*
	libbroadway - A general purpose library to control the Wii.
	IRQ support

Copyright (C) 2009		Bernhard Urban <lewurm@gmx.net>
Copyright (C) 2009		Sebastian Falbesoner <sebastian.falbesoner@gmail.com>
Copyright (C) 2009-2010		Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#include <broadway.h>

static irq_handler_t	irq_handler_table[IRQ_MAX];
static irq_handler_t	irq_bw_pi_handler_table[BW_PI_IRQ_MAX];

int irqs_on = 0;
u32 bw_enabled_irq = 0;
u32 hw_enabled_irq = 0;
u32 msrvalue = 0;

int irq_register_handler(u32 irqn, irq_handler_t irqh)
{
	irq_handler_table[irqn] = irqh;
	return 1;
}

irq_handler_t irq_get_handler(u32 irqn)
{
	return irq_handler_table[irqn];
}

int irq_bw_pi_register_handler(u32 irqn, irq_handler_t irqh)
{
	irq_bw_pi_handler_table[irqn] = irqh;
	return 1;
}

irq_handler_t irq_bw_pi_get_handler(u32 irqn)
{
	return irq_bw_pi_handler_table[irqn];
}


int _irq_handler_timer(u32 irq)
{
	(void)irq;
	return 1;
}

int _irq_handler_nand(u32 irq)
{
	/* Hmmm... should be done by MINI? */
	/* Let's shut it up... */
	write32(NAND_CMD, 0x7FFFFFFF);
	(void)irq;
	return 1;
}

int _irq_handler_gpio1b(u32 irq)
{
	/* Hmmm... should be done by MINI? */
	/* Let's shut it up... */
	write32(HW_GPIO1BINTFLAG, 0xFFFFFF);
	(void)irq;
	return 1;
}

int _irq_handler_gpio1(u32 irq)
{
	/* Hmmm... should be done by MINI? */
	/* Let's shut it up... */
	write32(HW_GPIO1INTFLAG, 0xFFFFFF);
	(void)irq;
	return 1;
}

int _irq_handler_reset(u32 irq)
{
	(void)irq;
	return 1;
}

int _irq_handler_ipc(u32 irq)
{
	/* Not necessary here? */
	/* ipc_irq(); */
	(void)irq;
	return 1;
}

int _irq_handler_aes(u32 irq)
{
	(void)irq;
	return 1;
}

int _irq_handler_sdhc(u32 irq)
{
	/* sdhc_irq(); */
	(void)irq;
	return 1;
}

int _irq_bw_pi_handler_reset(u32 irq)
{
	boot2_run(1, 2); /* Launch sysmenu */
	(void)irq;
	return 1;
}

int _irq_bw_pi_handler_hardware(u32 irq)
{
	int i;
	u32 enabled = read32(HW_PPCIRQMASK);
	u32 flags = read32(HW_PPCIRQFLAG);
	
	flags &= enabled;
	
	for(i = 0; i < IRQ_MAX; i++) {
		if(flags & IRQF(i)) { 
			if(irq_get_handler(i)) {
				(irq_get_handler(i))(i);
			}
			write32(HW_PPCIRQFLAG, IRQF(i));
		}
	}
	
	flags &= ~IRQF_ALL;
	if(flags) {
		printf("IRQ: unknown 0x%08x\n", flags);
		write32(HW_PPCIRQFLAG, flags);
	}
	(void)irq;
	return 1;
}

void _irq_init_defaults()
{
	irq_bw_pi_register_handler(BW_PI_IRQ_RESET,	_irq_bw_pi_handler_reset);
	irq_bw_pi_register_handler(BW_PI_IRQ_HW,	_irq_bw_pi_handler_hardware);

	irq_register_handler(IRQ_TIMER,			_irq_handler_timer);
	irq_register_handler(IRQ_NAND,			_irq_handler_nand);
	irq_register_handler(IRQ_GPIO1B,		_irq_handler_gpio1b);
	irq_register_handler(IRQ_GPIO1,			_irq_handler_gpio1);
	irq_register_handler(IRQ_RESET,			_irq_handler_reset);
	irq_register_handler(IRQ_IPC,			_irq_handler_ipc);
	irq_register_handler(IRQ_AES,			_irq_handler_aes);
	irq_register_handler(IRQ_SDHC,			_irq_handler_sdhc);
}

void show_frame_no(void);

void irq_initialize(void)
{
	int i;
	// clear flipper-pic (processor interface)
	write32(BW_PI_IRQMASK, 0);
	write32(BW_PI_IRQFLAG, 0xFFFFFFFF);

	// clear hollywood-pic
	write32(HW_PPCIRQMASK, 0);
	write32(HW_PPCIRQFLAG, 0xFFFFFFFF);

	/* ??? -- needed?!
	 * in mini they do
	 *
	 * write32(HW_ARMIRQMASK+0x04, 0);
	 * write32(HW_ARMIRQMASK+0x20, 0);
	 *
	 *
	 * may it's here following; on the other 
	 * hand it's already done by mini...
	 *
	 * write32(HW_PPCIRQMASK+0x04+0x08, 0);
	 * write32(HW_PPCIRQMASK+0x20+0x08, 0);
	 */

	for(i = 0; i < IRQ_MAX; i++) {
		irq_handler_table[i] = NULL;
	}
	for(i = 0; i < BW_PI_IRQ_MAX; i++) {
		irq_handler_table[i] = NULL;
	}
	_irq_init_defaults();
	irq_enable();
}

void irq_shutdown(void)
{
	write32(HW_PPCIRQMASK, 0);
	write32(HW_PPCIRQFLAG, 0xFFFFFFFF);
	irq_disable();
}

void irq_handler(void)
{
	int i;
	u32 enabled = read32(BW_PI_IRQMASK);
	u32 flags = read32(BW_PI_IRQFLAG);

	flags &= enabled;

	for(i = 0; i < BW_PI_IRQ_MAX; i++) {
		if(flags & IRQF(i)) { 
			if(irq_bw_pi_get_handler(i)) {
				(irq_bw_pi_get_handler(i))(i);
			}
			write32(BW_PI_IRQFLAG, IRQF(i));
		}
	}
}

void irq_bw_enable(u32 irq)
{
	set32(BW_PI_IRQMASK, IRQF(irq));
	write32(BW_PI_IRQFLAG, IRQF(irq));
	bw_enabled_irq |= IRQF(irq);
}

void irq_bw_disable(u32 irq)
{
	write32(BW_PI_IRQFLAG, IRQF(irq));
	clear32(BW_PI_IRQMASK, IRQF(irq));
	bw_enabled_irq &= ~IRQF(irq);
}

void irq_hw_enable(u32 irq)
{
	set32(HW_PPCIRQMASK, IRQF(irq));
	write32(HW_PPCIRQFLAG, IRQF(irq));
	hw_enabled_irq |= IRQF(irq);
}

void irq_hw_disable(u32 irq)
{
	write32(HW_PPCIRQFLAG, IRQF(irq));
	clear32(HW_PPCIRQMASK, IRQF(irq));
	hw_enabled_irq &= ~IRQF(irq);
}

void irq_enable()
{
	register u32 _val = 0;
	asm volatile(\
		"mfmsr	%0			\n" \
		"ori	%0, %0, 0x8000		\n" \
		"mtmsr	%0			\n" \
		: "=&r"((_val)) \
		:   "0"((_val)));
	ppcsync();
	asm volatile("mfmsr %0" : "=r"(msrvalue) : );
	write32(BW_PI_IRQFLAG, 0xFFFFFFFF);
	write32(HW_PPCIRQFLAG, 0xFFFFFFFF);
	irqs_on = 1;
}	

u32 irq_disable()
{
	u32 was_on = 0;
	register u32 _disable_mask = 0;
	write32(BW_PI_IRQFLAG, 0xFFFFFFFF);
	write32(HW_PPCIRQFLAG, 0xFFFFFFFF);
	asm volatile(\
		"mfmsr	%0			\n" \
		"rlwinm	%1, %0, 0, 17, 15	\n" \
		"mtmsr	%1			\n" \
		"extrwi	%0, %0, 1, 16		\n" \
		: "=&r" ((was_on)), "=&r" ((_disable_mask)) \
		: "0" ((was_on)), "1" ((_disable_mask)));
	ppcsync();
	asm volatile("mfmsr %0" : "=r"(msrvalue) : );
	irqs_on = 0;
	return was_on;
}

void irq_restore(u32 was_on)
{
	irqs_on = 2;
	if(was_on)
		irq_enable();
	irq_enable();		/* Looks like we need to do this hack... >.> */
}

