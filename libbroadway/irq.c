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

static irq_handler_t	irq_hw_handler_table[IRQ_HW_MAX];
static irq_handler_t	irq_bw_handler_table[IRQ_BW_MAX];

static int irqs_on = 0;
static u32 bw_enabled_irq = 0;
static u32 hw_enabled_irq = 0;
static u32 msrvalue = 0;
static u32 irq_bw_fired = 0;
static u32 irq_bw_enabled = 0;
u32 reset_pressed = 0;


int irq_hw_register_handler(u32 irqn, int (*exec)(u32 irq, void* data), void* data)
{
	irq_hw_handler_table[irqn % IRQ_HW_MAX].exec = exec;
	irq_hw_handler_table[irqn % IRQ_HW_MAX].data = data;
	return 1;
}

irq_handler_t irq_hw_get_handler(u32 irqn)
{
	return irq_hw_handler_table[irqn % IRQ_HW_MAX];
}

int irq_bw_register_handler(u32 irqn, int (*exec)(u32 irq, void* data), void* data)
{
	irq_bw_handler_table[irqn % IRQ_BW_MAX].exec = exec;
	irq_bw_handler_table[irqn % IRQ_BW_MAX].data = data;
	return 1;
}

irq_handler_t irq_bw_get_handler(u32 irqn)
{
	return irq_bw_handler_table[irqn % IRQ_BW_MAX];
}

int _irq_handler_timer(u32 irq, void* data)
{
	(void)irq;
	(void)data;
	return 1;
}

int _irq_handler_nand(u32 irq, void* data)
{
	/* Hmmm... should be done by MINI? */
	/* Let's shut it up... */
	write32(NAND_CMD, 0x7FFFFFFF);
	(void)irq;
	(void)data;
	return 1;
}

int _irq_handler_gpio1b(u32 irq, void* data)
{
	/* Hmmm... should be done by MINI? */
	/* Let's shut it up... */
	write32(HW_GPIO1BINTFLAG, 0xFFFFFF);
	(void)irq;
	(void)data;
	return 1;
}

int _irq_handler_gpio1(u32 irq, void* data)
{
	/* Hmmm... should be done by MINI? */
	/* Let's shut it up... */
	write32(HW_GPIO1INTFLAG, 0xFFFFFF);
	(void)irq;
	(void)data;
	return 1;
}

int _irq_handler_reset(u32 irq, void* data)
{
	reset_pressed = 1;
	(void)irq;
	(void)data;
	return 1;
}

int _irq_handler_ipc(u32 irq, void* data)
{
	/* Not necessary here? */
	/* ipc_irq(); */
	(void)irq;
	(void)data;
	return 1;
}

int _irq_handler_aes(u32 irq, void* data)
{
	(void)irq;
	(void)data;
	return 1;
}

int _irq_handler_sdhc(u32 irq, void* data)
{
	/* sdhc_irq(); */
	(void)irq;
	(void)data;
	return 1;
}

int _irq_bw_handler_hardware(u32 irq, void* data)
{
	int i;
	irq_handler_t handler;
	u32 enabled = read32(HW_PPCIRQMASK);
	u32 flags = read32(HW_PPCIRQFLAG);
	u32 useflags = flags & enabled;
	
	gfx_printf(" BW Fired: %08X *HW Fired: %08X\n", irq_bw_fired,   flags);
	gfx_printf(" BW Mask:  %08X *HW Mask:  %08X\n", irq_bw_enabled, enabled);
	if(useflags == 0) /* Spurious interrupt */
		return;
	for(i = 0; i < IRQ_HW_MAX; i++) {
		printf("Interrupt check: %d.\n", i);
		if(useflags & IRQF(i)) { 
			printf("Did fire!\n");
			handler = irq_hw_get_handler(i);
			if(handler.exec) {
				printf("We got a handler at 0x%08X\n", handler.exec);
				(handler.exec)(i, handler.data);
				useflags &= ~IRQF(i);
			}
			write32(HW_PPCIRQFLAG, IRQF(i));
		}
	}
	
	if(useflags) {
		printf("HWIRQ: unhandled 0x%08x\n", useflags);
		write32(HW_PPCIRQFLAG, useflags);
	}
	(void)irq;
	(void)data;
	return 1;
}

void _irq_init_defaults()
{
	irq_bw_register_handler(IRQ_BW_HW,		_irq_bw_handler_hardware,	NULL);

	irq_hw_register_handler(IRQ_HW_TIMER,		_irq_handler_timer,		NULL);
	irq_hw_register_handler(IRQ_HW_NAND,		_irq_handler_nand,		NULL);
	irq_hw_register_handler(IRQ_HW_GPIO1B,		_irq_handler_gpio1b,		NULL);
	irq_hw_register_handler(IRQ_HW_GPIO1,		_irq_handler_gpio1,		NULL);
	irq_hw_register_handler(IRQ_HW_RESET,		_irq_handler_reset,		NULL);
	irq_hw_register_handler(IRQ_HW_IPC,		_irq_handler_ipc,		NULL);
	irq_hw_register_handler(IRQ_HW_AES,		_irq_handler_aes,		NULL);
	irq_hw_register_handler(IRQ_HW_SDHC,		_irq_handler_sdhc,		NULL);
}

void irq_initialize(void)
{
	int i;
	/* Remove all the IRQs from the PPC and clear all current IRQs */
	write32(HW_PPCIRQMASK, 0);
	write32(HW_PPCIRQFLAG, 0xFFFFFFFF);
	hw_enabled_irq = 0;
	
	/* Remove all the IRQs from the Broadway and clear all current IRQs */
	write32(HW_BWIRQMASK, 0);
	write32(HW_BWIRQFLAG, 0xFFFFFFFF);
	bw_enabled_irq = 0;

	for(i = 0; i < IRQ_HW_MAX; i++) {
		irq_hw_handler_table[i].exec = NULL;
		irq_hw_handler_table[i].data = NULL;
	}
	for(i = 0; i < IRQ_BW_MAX; i++) {
		irq_bw_handler_table[i].exec = NULL;
		irq_bw_handler_table[i].data = NULL;
	}
	
	_irq_init_defaults();
	irq_enable();
}

void irq_shutdown(void)
{
	/* Remove all the IRQs from the PPC and clear all current IRQs */
	write32(HW_PPCIRQMASK, 0);
	write32(HW_PPCIRQFLAG, 0xFFFFFFFF);
	hw_enabled_irq = 0;
	
	/* Remove all the IRQs from the Broadway and clear all current IRQs */
	write32(HW_BWIRQMASK, 0);
	write32(HW_BWIRQFLAG, 0xFFFFFFFF);
	bw_enabled_irq = 0;
	
	irq_disable();
}

void irq_handler(void)
{
	int i;
	irq_handler_t handler;
	u32 enabled = read32(HW_BWIRQMASK);
	u32 flags = read32(HW_BWIRQFLAG);
	u32 useflags = flags & enabled;
	irq_bw_fired = flags;
	irq_bw_enabled = enabled;

	gfx_printf("*BW Fired: %08X  HW Fired: %08X\n", irq_bw_fired,   read32(HW_PPCIRQFLAG));
	gfx_printf("*BW Mask:  %08X  HW Mask:  %08X\n", irq_bw_enabled, read32(HW_PPCIRQMASK));
	if(useflags == 0) /* Spurious interrupt */
		return;
	for(i = 0; i < IRQ_BW_MAX; i++) {
		printf("Interrupt check: %d.\n", i);
		if(useflags & IRQF(i)) { 
			printf("Did fire!\n");
			handler = irq_bw_get_handler(i);
			if(handler.exec) {
				printf("We got a handler at 0x%08X\n", handler.exec);
				(handler.exec)(i, handler.data);
				useflags &= ~IRQF(i);
			}
			write32(HW_BWIRQFLAG, IRQF(i));
		}
	}
	if(useflags) {
		printf("BWIRQ: unhandled 0x%08x\n", useflags);
		write32(HW_BWIRQFLAG, useflags);
	}
	irq_bw_fired = 0;
}

void irq_bw_enable(u32 irq)
{
	set32(  HW_BWIRQMASK, IRQF(irq));
	write32(HW_BWIRQFLAG, IRQF(irq));
	bw_enabled_irq |= IRQF(irq);
}

void irq_bw_disable(u32 irq)
{
	write32(HW_BWIRQFLAG, IRQF(irq));
	clear32(HW_BWIRQMASK, IRQF(irq));
	bw_enabled_irq &= ~IRQF(irq);
}

void irq_hw_enable(u32 irq)
{
	/* Remove the IRQ from the ARM and give it to the PPC */
	clear32(HW_ARMIRQMASK, IRQF(irq));
	set32(  HW_PPCIRQMASK, IRQF(irq));
	
	write32(HW_PPCIRQFLAG, IRQF(irq));
	hw_enabled_irq |= IRQF(irq);
}

void irq_hw_disable(u32 irq)
{
	/* Remove the IRQ from the PPC and give it back to the ARM */
	clear32(HW_PPCIRQMASK, IRQF(irq));
	set32(  HW_ARMIRQMASK, IRQF(irq));
	
	write32(HW_PPCIRQFLAG, IRQF(irq));
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
	write32(HW_BWIRQFLAG, 0xFFFFFFFF);
	write32(HW_PPCIRQFLAG, 0xFFFFFFFF);
	irqs_on = 1;
}	

u32 irq_disable()
{
	u32 was_on = 0;
	register u32 _disable_mask = 0;
	write32(HW_BWIRQFLAG, 0xFFFFFFFF);
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

