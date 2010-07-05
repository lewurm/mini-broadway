/*
	libbroadway - A general purpose library to control the Wii.
	IRQ support

Copyright (C) 2009		Bernhard Urban <lewurm@gmx.net>
Copyright (C) 2009		Sebastian Falbesoner <sebastian.falbesoner@gmail.com>
Copyright (C) 2009-2010		Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#ifndef __IRQ_H__
#define __IRQ_H__

#include <types.h>

/* Broadway Processor Interface Registers */
#define HW_BWIRQFLAG	(0x0C003000)
#define HW_BWIRQMASK	(0x0C003004)

#define IRQ_HW_TIMER	0
#define IRQ_HW_NAND	1
#define IRQ_HW_AES	2
#define IRQ_HW_SHA1	3
#define IRQ_HW_EHCI	4
#define IRQ_HW_OHCI0	5
#define IRQ_HW_OHCI1	6
#define IRQ_HW_SDHC	7
#define IRQ_HW_WIFI	8
#define IRQ_HW_GPIO1B	10
#define IRQ_HW_GPIO1	11
#define IRQ_HW_RESET	17
#define IRQ_HW_PPCIPC	30
#define IRQ_HW_IPC	31
/* Not a real IRQ, just a little helpful something */
#define IRQ_HW_MAX	32

// http://hitmen.c02.at/files/yagcd/yagcd/chap5.html#sec5.4
#define IRQ_BW_RESET		1
#define IRQ_BW_DI		2
#define IRQ_BW_SI		3
#define IRQ_BW_EXI		4
#define IRQ_BW_AI 	 	5
#define IRQ_BW_DSP 	 	6
#define IRQ_BW_MEM 	 	7
#define IRQ_BW_VI 	 	8
#define IRQ_BW_PE_TOKEN		9
#define IRQ_BW_PE_FINISH	10
#define IRQ_BW_CP 	 	11
#define IRQ_BW_DEBUG		12
#define IRQ_BW_HSP 	 	13
#define IRQ_BW_HW 		14 /* Hollywood IRQs */
#define IRQ_BW_RESET_SW 	16
/* Not a real IRQ, just a little helpful something */
#define IRQ_BW_MAX		32

#define IRQF(n)			(1 << n)
#define IRQF_HW_TIMER		(IRQF(IRQ_HW_TIMER))
#define IRQF_HW_NAND		(IRQF(IRQ_HW_NAND))
#define IRQF_HW_AES		(IRQF(IRQ_HW_AES))
#define IRQF_HW_SHA1		(IRQF(IRQ_HW_SHA1))
#define IRQF_HW_EHCI		(IRQF(IRQ_HW_EHCI))
#define IRQF_HW_OHCI0		(IRQF(IRQ_HW_OHCI0))
#define IRQF_HW_OHCI1		(IRQF(IRQ_HW_OHCI1))
#define IRQF_HW_SDHC		(IRQF(IRQ_HW_SDHC))
#define IRQF_HW_WIFI		(IRQF(IRQ_HW_WIFI))
#define IRQF_HW_GPIO1B		(IRQF(IRQ_HW_GPIO1B))
#define IRQF_HW_GPIO1		(IRQF(IRQ_HW_GPIO1))
#define IRQF_HW_RESET		(IRQF(IRQ_HW_RESET))
#define IRQF_HW_PPCIPC		(IRQF(IRQ_HW_PPCIPC))
#define IRQF_HW_IPC		(IRQF(IRQ_HW_IPC))

#define IRQF_BW_RESET		(IRQF(IRQ_BW_RESET))
#define IRQF_BW_DI		(IRQF(IRQ_BW_DI))
#define IRQF_BW_SI		(IRQF(IRQ_BW_SI))
#define IRQF_BW_EXI		(IRQF(IRQ_BW_EXI))
#define IRQF_BW_AI		(IRQF(IRQ_BW_AI))
#define IRQF_BW_DSP		(IRQF(IRQ_BW_DSP))
#define IRQF_BW_MEM		(IRQF(IRQ_BW_MEM))
#define IRQF_BW_VI		(IRQF(IRQ_BW_VI))
#define IRQF_BW_PE_TOKEN	(IRQF(IRQ_BW_PE_TOKEN))
#define IRQF_BW_PE_FINISH	(IRQF(IRQ_BW_PE_FINISH))
#define IRQF_BW_CP		(IRQF(IRQ_BW_CP))
#define IRQF_BW_DEBUG		(IRQF(IRQ_BW_DEBUG))
#define IRQF_BW_HSP		(IRQF(IRQ_BW_HSP))
#define IRQF_BW_HW		(IRQF(IRQ_BW_HW))
#define IRQF_BW_RESET_SW	(IRQF(IRQ_BW_RESET_SW))

#define IRQF_HW_ALL	(IRQF_HW_TIMER  | IRQF_HW_NAND   | IRQF_HW_AES   | IRQF_HW_SHA1  | \
			 IRQF_HW_EHCI   | IRQF_HW_OHCI0  | IRQF_HW_OHCI1 | IRQF_HW_SDHC  | \
			 IRQF_HW_WIFI   | IRQF_HW_GPIO1B | IRQF_HW_GPIO1 | IRQF_HW_RESET | \
			 IRQF_HW_PPCIPC | IRQF_HW_IPC )

#define IRQF_BW_ALL	(IRQF_BW_RESET    | IRQF_BW_DI        | IRQF_BW_SI       | IRQF_BW_EXI   | \
			 IRQF_BW_AI       | IRQF_BW_DSP       | IRQF_BW_MEM      | IRQF_BW_VI    | \
			 IRQF_BW_PE_TOKEN | IRQF_BW_PE_FINISH | IRQF_BW_CP       | IRQF_BW_DEBUG | \
			 IRQF_BW_HSP      | IRQF_BW_HW        | IRQF_BW_RESET_SW )

extern u32 reset_pressed;

typedef struct {
	int	(*exec)(u32 irq, void* data);
	void*	data;
} irq_handler_t;

void irq_initialize(void);
void irq_shutdown(void);

void irq_handler(void);

void irq_bw_enable(u32 irq);
void irq_bw_disable(u32 irq);
void irq_hw_enable(u32 irq);
void irq_hw_disable(u32 irq);

void irq_enable(void);
u32  irq_disable(void);
void irq_restore(u32 was_on);

/* To unregister an IRQ, pass NULL as exec. */
int		irq_hw_register_handler(u32 irqn, int (*exec)(u32 irq, void* data), void* data);
irq_handler_t	irq_hw_get_handler(u32 irqn);
int		irq_bw_register_handler(u32 irqn, int (*exec)(u32 irq, void* data), void* data);
irq_handler_t	irq_bw_get_handler(u32 irqn);

/* TODO: port to ppc 
static inline void irq_wait(void)
{
	u32 data = 0;
	__asm__ volatile ( "mcr\tp15, 0, %0, c7, c0, 4" : : "r" (data) );
}
*/
#endif


