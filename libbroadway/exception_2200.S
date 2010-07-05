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
	stw	0, 0x20a0(0)
	ori	0, 0, 0x2030
	mtsrr1	0

	# Go to C handler
	lis	0,    exception_handler@h
	ori	0, 0, exception_handler@l
	mtsrr0	0
	rfi
exception_2200_end:

exception_restorer:
	li	1, 0
	oris	1, 1, 0x8000
	lwz	0, 0x2080(1)
	mtcr	0
	lwz	0, 0x2084(1)
	mtxer	0
#	lwz	0, 0x2088(1)
#	mtlr	0
	lwz	0, 0x208C(1)
	mtctr	0
	lwz	0, 0x2090(1)
	mtlr	0
#	mtsrr0	0
	lwz	0, 0x2098(1)
	mtdar	0
	lwz	0, 0x209C(1)
	mtdsisr	0
	lwz	0, 0x20A0(1)
	lwz	3, 0x2094(1)
	mtsrr1	3
#	andi.	3, 3, 0xFF73
#	andi.	0, 0, 0x008C
#	or	0, 0, 4
#	mtmsr	0
	lmw	2, 0x2008(1)
	lwz	0, 0x2000(1)
	lwz	1, 0x2004(1)
	blr
