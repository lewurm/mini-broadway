/*
	BootMii - a Free Software replacement for the Nintendo/BroadOn bootloader.
	Requires mini.
	
Copyright (C) 2008		Segher Boessenkool <segher@kernel.crashing.org>
Copyright (C) 2010		Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

.globl exception_2200_start, exception_2200_end
.globl exception_restorer
.globl exception_finish

exception_2200_start:
	# store all interesting regs
	mfcr	0
	stw	0, 0x2080(0)
	mfxer	0
	stw	0, 0x2084(0)
	mflr	0
	stw	0, 0x2088(0)
	mfctr	0
	stw	0, 0x208c(0)
	mfsrr0	0
	stw	0, 0x2090(0)
	mfsrr1	0
	stw	0, 0x2094(0)
	mfdar	0
	stw	0, 0x2098(0)
	mfdsisr	0
	stw	0, 0x209c(0)

	# Switch on FP, DR, IR
	mfmsr	0
	ori	0, 0, 0x2030
	mtsrr1	0

	# Set up TOC and "kernel stack"
	lis	2, 0x8000
	lis	1,    kernel_stack_end@h
	ori	1, 1, kernel_stack_end@l
	# Go to C handler
	lis	0,    exception_handler@h
	ori	0, 0, exception_handler@l
	mtsrr0	0
	rfi
exception_2200_end:

exception_restorer:
	lwz	0, 0x2094(0)
	mtsrr1	0
	lwz	0, 0x2090(0)
	mtsrr0	0
	lwz	0, 0x208c(0)
	mtctr	0
	lwz	0, 0x2088(0)
	mtlr	0
	lwz	0, 0x2084(0)
	mtxer	0
	lwz	0, 0x2080(0)
	mtcr	0

	lmw	1, 0x2004(0)
	lwz	0, 0x2000(0)
	rfi

exception_finish:
	# Load up exception_restorer to be called
	lis	0,    exception_restorer@h
	ori	0, 0, exception_restorer@l
	mtsrr0	0
	# Turn off EE, DR
	mfmsr	0
	ori	0, 0, 0x8010
	xori	0, 0, 0x8010
	mtsrr1	0
	rfi

kernel_stack:
	.space	0x8000	# Just to be safe
kernel_stack_end:
	.space	0x1000	# Just to be safe
