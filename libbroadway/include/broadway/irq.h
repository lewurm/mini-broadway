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

#define IRQ_TIMER	0
#define IRQ_NAND	1
#define IRQ_AES		2
#define IRQ_SHA1	3
#define IRQ_EHCI	4
#define IRQ_OHCI0	5
#define IRQ_OHCI1	6
#define IRQ_SDHC	7
#define IRQ_WIFI	8
#define IRQ_GPIO1B	10
#define IRQ_GPIO1	11
#define IRQ_RESET	17
#define IRQ_PPCIPC	30
#define IRQ_IPC		31
/* Not a real IRQ, just a little helpful something */
#define IRQ_MAX		32

#define IRQF(n)		(1 << n)
#define IRQF_TIMER	(IRQF(IRQ_TIMER))
#define IRQF_NAND	(IRQF(IRQ_NAND))
#define IRQF_AES	(IRQF(IRQ_AES))
#define IRQF_SHA1	(IRQF(IRQ_SHA1))
#define IRQF_EHCI 	(IRQF(IRQ_EHCI))
#define IRQF_OHCI0 	(IRQF(IRQ_OHCI0))
#define IRQF_OHCI1 	(IRQF(IRQ_OHCI1))
#define IRQF_SDHC	(IRQF(IRQ_SDHC))
#define IRQF_WIFI	(IRQF(IRQ_WIFI))
#define IRQF_GPIO1B	(IRQF(IRQ_GPIO1B))
#define IRQF_GPIO1	(IRQF(IRQ_GPIO1))
#define IRQF_RESET	(IRQF(IRQ_RESET))
#define IRQF_PPCIPC	(IRQF(IRQ_PPCIPC))
#define IRQF_IPC	(IRQF(IRQ_IPC))

#define IRQF_ALL	(IRQF_TIMER  | IRQF_NAND   | IRQF_AES   | IRQF_SHA1  | \
			 IRQF_EHCI   | IRQF_OHCI0  | IRQF_OHCI1 | IRQF_SDHC  | \
			 IRQF_WIFI   | IRQF_GPIO1B | IRQF_GPIO1 | IRQF_RESET | \
			 IRQF_PPCIPC | IRQF_IPC )

/* Broadway Processor Interface Registers */
#define BW_PI_IRQFLAG (0x0C003000)
#define BW_PI_IRQMASK (0x0C003004)

// http://hitmen.c02.at/files/yagcd/yagcd/chap5.html#sec5.4
#define BW_PI_IRQ_RESET 	1
#define BW_PI_IRQ_DI 		2
#define BW_PI_IRQ_SI 		3
#define BW_PI_IRQ_EXI 		4
#define BW_PI_IRQ_AI 	 	5
#define BW_PI_IRQ_DSP 	 	6
#define BW_PI_IRQ_MEM 	 	7
#define BW_PI_IRQ_VI 	 	8
#define BW_PI_IRQ_PE_TOKEN 	9
#define BW_PI_IRQ_PE_FINISH 	10
#define BW_PI_IRQ_CP 	 	11
#define BW_PI_IRQ_DEBUG 	12
#define BW_PI_IRQ_HSP 	 	13
#define BW_PI_IRQ_HW 		14 /* hollywood pic */
#define BW_PI_IRQ_RESET_SW 	16
/* Not a real IRQ, just a little helpful something */
#define BW_PI_IRQ_MAX 		32

extern u32 reset_pressed;

typedef int (*irq_handler_t)(u32 irq);

void irq_initialize(void);
void irq_shutdown(void);

void irq_handler(void);

void irq_bw_enable(u32 irq);
void irq_bw_disable(u32 irq);
void irq_hw_enable(u32 irq);
void irq_hw_disable(u32 irq);

void irq_enable(void);
u32 irq_disable(void);
void irq_restore(u32 was_on);

int irq_register_handler(u32 irqn, irq_handler_t irqh);
irq_handler_t irq_get_handler(u32 irqn);
int irq_bw_pi_register_handler(u32 irqn, irq_handler_t irqh);
irq_handler_t irq_bw_pi_get_handler(u32 irqn);

/* TODO: port to ppc 
static inline void irq_wait(void)
{
	u32 data = 0;
	__asm__ volatile ( "mcr\tp15, 0, %0, c7, c0, 4" : : "r" (data) );
}
*/
#endif


