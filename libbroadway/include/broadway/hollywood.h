/*
	mini - a Free Software replacement for the Nintendo/BroadOn IOS.
	Hollywood register definitions

Copyright (C) 2008, 2009	Haxx Enterprises <bushing@gmail.com>
Copyright (C) 2008, 2009	Sven Peter <svenpeter@gmail.com>
Copyright (C) 2008, 2009	Hector Martin "marcan" <marcan@marcansoft.com>
Copyright (C) 2008, 2009	John Kelley <wiidev@kelley.ca>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#ifndef __HOLLYWOOD_H__
#define __HOLLYWOOD_H__

/* Hollywood Registers */

#define		HW_PPC_REG_BASE			0xD000000
#define		HW_REG_BASE			0xD800000

// The PPC can only see the first three IPC registers
#define		HW_IPC_PPCMSG			(HW_REG_BASE + 0x000)
#define		HW_IPC_PPCCTRL			(HW_REG_BASE + 0x004)
#define		HW_IPC_ARMMSG			(HW_REG_BASE + 0x008)
#define		HW_IPC_ARMCTRL			(HW_REG_BASE + 0x00C)

#define		HW_TIMER			(HW_REG_BASE + 0x010)
#define		HW_ALARM			(HW_REG_BASE + 0x014)

#define		HW_PPCIRQFLAG			(HW_REG_BASE + 0x030)
#define		HW_PPCIRQMASK			(HW_REG_BASE + 0x034)

#define		HW_ARMIRQFLAG			(HW_REG_BASE + 0x038)
#define		HW_ARMIRQMASK			(HW_REG_BASE + 0x03C)

#define		HW_MEMMIRR			(HW_REG_BASE + 0x060)

// something to do with PPCBOOT
// and legacy DI it seems ?!?
#define		HW_EXICTRL			(HW_REG_BASE + 0x070)
#define		EXICTRL_ENABLE_EXI		1

// PPC side of GPIO1 (Starlet can access this too)
// Output state
#define		HW_GPIO1BOUT			(HW_REG_BASE + 0x0C0)
// Direction (1=output)
#define		HW_GPIO1BDIR			(HW_REG_BASE + 0x0C4)
// Input state
#define		HW_GPIO1BIN			(HW_REG_BASE + 0x0C8)
// Interrupt level
#define		HW_GPIO1BINTLVL			(HW_REG_BASE + 0x0CC)
// Interrupt flags (write 1 to clear)
#define		HW_GPIO1BINTFLAG		(HW_REG_BASE + 0x0D0)
// Interrupt propagation enable
// Do these interrupts go anywhere???
#define		HW_GPIO1BINTENABLE		(HW_REG_BASE + 0x0D4)
//??? seems to be a mirror of inputs at some point... power-up state?
#define		HW_GPIO1BINMIR			(HW_REG_BASE + 0x0D8)
// 0xFFFFFF by default, if cleared disables respective outputs. Top bits non-settable.
#define		HW_GPIO1ENABLE			(HW_REG_BASE + 0x0DC)

#define		HW_GPIO1_SLOT			0x000020
#define		HW_GPIO1_DEBUG			0xFF0000
#define		HW_GPIO1_DEBUG_SH		16

// Starlet side of GPIO1
// Output state
#define		HW_GPIO1OUT			(HW_REG_BASE + 0x0E0)
// Direction (1=output)
#define		HW_GPIO1DIR			(HW_REG_BASE + 0x0E4)
// Input state
#define		HW_GPIO1IN			(HW_REG_BASE + 0x0E8)
// Interrupt level
#define		HW_GPIO1INTLVL			(HW_REG_BASE + 0x0EC)
// Interrupt flags (write 1 to clear)
#define		HW_GPIO1INTFLAG			(HW_REG_BASE + 0x0F0)
// Interrupt propagation enable (interrupts go to main interrupt 0x800)
#define		HW_GPIO1INTENABLE		(HW_REG_BASE + 0x0F4)
//??? seems to be a mirror of inputs at some point... power-up state?
#define		HW_GPIO1INMIR			(HW_REG_BASE + 0x0F8)
// Owner of each GPIO bit. If 1, GPIO1B registers assume control. If 0, GPIO1 registers assume control.
#define		HW_GPIO1OWNER			(HW_REG_BASE + 0x0FC)

// ????
#define		HW_DIFLAGS			(HW_REG_BASE + 0x180)
#define		DIFLAGS_BOOT_CODE		0x100000

// maybe a GPIO???
#define		HW_RESETS			(HW_REG_BASE + 0x194)

#define		HW_CLOCKS			(HW_REG_BASE + 0x1B4)

#define		HW_GPIO2OUT			(HW_REG_BASE + 0x1C8)
#define		HW_GPIO2DIR			(HW_REG_BASE + 0x1CC)
#define		HW_GPIO2IN			(HW_REG_BASE + 0x1D0)

#define		HW_OTPCMD			(HW_REG_BASE + 0x1EC)
#define		HW_OTPDATA			(HW_REG_BASE + 0x1F0)
#define		HW_VERSION			(HW_REG_BASE + 0x214)

/* NAND Registers */

#define		NAND_REG_BASE			0xD010000

#define		NAND_CMD			(NAND_REG_BASE + 0x000)
#define		NAND_STATUS			NAND_CMD
#define		NAND_CONF			(NAND_REG_BASE + 0x004)
#define		NAND_ADDR0			(NAND_REG_BASE + 0x008)
#define		NAND_ADDR1			(NAND_REG_BASE + 0x00C)
#define		NAND_DATA			(NAND_REG_BASE + 0x010)
#define		NAND_ECC			(NAND_REG_BASE + 0x014)
#define		NAND_UNK1			(NAND_REG_BASE + 0x018)
#define		NAND_UNK2			(NAND_REG_BASE + 0x01C)

/* AES Registers */

#define		AES_REG_BASE			0xD020000

#define		AES_CMD				(AES_REG_BASE + 0x000)
#define		AES_SRC				(AES_REG_BASE + 0x004)
#define		AES_DEST			(AES_REG_BASE + 0x008)
#define		AES_KEY				(AES_REG_BASE + 0x00C)
#define		AES_IV				(AES_REG_BASE + 0x010)

/* SHA-1 Registers */

#define		SHA_REG_BASE			0xD030000

#define		SHA_CMD				(SHA_REG_BASE + 0x000)
#define		SHA_SRC				(SHA_REG_BASE + 0x004)
#define		SHA_H0				(SHA_REG_BASE + 0x008)
#define		SHA_H1				(SHA_REG_BASE + 0x00C)
#define		SHA_H2				(SHA_REG_BASE + 0x010)
#define		SHA_H3				(SHA_REG_BASE + 0x014)
#define		SHA_H4				(SHA_REG_BASE + 0x018)

/* SD Host Controller Registers */

#define		SDHC_REG_BASE			0xD070000

/* OHCI0 Registers */
#define 	OHCI0_REG_BASE			0xD050000

/* OHCI1 Registers */
#define 	OHCI1_REG_BASE			0xD060000

#define 	OHCI_HC_REVISION		0x00
#define 	OHCI_HC_CONTROL			0x04
#define 	OHCI_HC_COMMAND_STATUS		0x08
#define 	OHCI_HC_INT_STATUS		0x0C

#define 	OHCI_HC_INT_ENABLE		0x10
#define 	OHCI_HC_INT_DISABLE		0x14
#define 	OHCI_HC_HCCA 			0x18
#define 	OHCI_HC_PERIOD_CURRENT_ED 	0x1C

#define 	OHCI_HC_CTRL_HEAD_ED 		0x20
#define 	OHCI_HC_CTRL_CURRENT_ED 	0x24
#define 	OHCI_HC_BULK_HEAD_ED 		0x28
#define 	OHCI_HC_BULK_CURRENT_ED 	0x2C

#define 	OHCI_HC_DONE_HEAD 		0x30
#define 	OHCI_HC_FM_INTERVAL 	 	0x34
#define 	OHCI_HC_FM_REMAINING 	 	0x38
#define 	OHCI_HC_FM_NUMBER		0x3C

#define 	OHCI_HC_PERIODIC_START 		0x40
#define 	OHCI_HC_LS_THRESHOLD 	 	0x44
#define 	OHCI_HC_RH_DESCRIPTOR_A		0x48
#define 	OHCI_HC_RH_DESCRIPTOR_B  	0x4C

#define 	OHCI_HC_RH_STATUS 		0x50
#define 	OHCI_HC_RH_PORT_STATUS_1 	0x54
#define 	OHCI_HC_RH_PORT_STATUS_2 	0x58

/* EHCI Registers */
#define 	EHCI_REG_BASE			0xD040000

/* stolen from mikep2 patched linux kernel: drivers/usb/host/ohci-mipc.c */
#define		EHCI_CTL			(EHCI_REG_BASE + 0xCC)
/* oh0 interrupt enable */
#define		EHCI_CTL_OH0INTE		(1 << 11)
/* oh1 interrupt enable */
#define		EHCI_CTL_OH1INTE		(1 << 12)

/* EXI Registers */
#define		EXI_REG_BASE			0xD806800

#define		EXI_BOOT_BASE			(EXI_REG_BASE + 0x040)

/* EXI0 Registers */
#define		EXI0_REG_BASE			(EXI_REG_BASE + 0x000)

#define		EXI0_CSR			(EXI0_REG_BASE + 0x000)
#define		EXI0_MAR			(EXI0_REG_BASE + 0x004)
#define		EXI0_LENGTH			(EXI0_REG_BASE + 0x008)
#define		EXI0_CR				(EXI0_REG_BASE + 0x00C)
#define		EXI0_DATA			(EXI0_REG_BASE + 0x010)

/* EXI1 Registers */
#define		EXI1_REG_BASE			(EXI_REG_BASE + 0x014)

#define		EXI1_CSR			(EXI1_REG_BASE + 0x000)
#define		EXI1_MAR			(EXI1_REG_BASE + 0x004)
#define		EXI1_LENGTH			(EXI1_REG_BASE + 0x008)
#define		EXI1_CR				(EXI1_REG_BASE + 0x00C)
#define		EXI1_DATA			(EXI1_REG_BASE + 0x010)

/* EXIT2 Registers */
#define		EXI2_REG_BASE			(EXI_REG_BASE + 0x028)

#define		EXI2_CSR			(EXI2_REG_BASE + 0x000)
#define		EXI2_MAR			(EXI2_REG_BASE + 0x004)
#define		EXI2_LENGTH			(EXI2_REG_BASE + 0x008)
#define		EXI2_CR				(EXI2_REG_BASE + 0x00C)
#define		EXI2_DATA			(EXI2_REG_BASE + 0x010)


/* MEMORY CONTROLLER Registers */

#define		MEM_REG_BASE			0xD8B4000
#define		MEM_PROT			(MEM_REG_BASE + 0x20A)
#define		MEM_PROT_START			(MEM_REG_BASE + 0x20C)
#define		MEM_PROT_END			(MEM_REG_BASE + 0x20E)
#define		MEM_FLUSHREQ			(MEM_REG_BASE + 0x228)
#define		MEM_FLUSHACK			(MEM_REG_BASE + 0x22A)

#endif
