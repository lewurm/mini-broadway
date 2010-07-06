# crt0.s file for the GameCube V1.0 by Costis (costis@gbaemu.com)!

# [trap15]	[2009/12/07]	[20:00PST]
# [*]	Re-did all the indentation and tabbing
# [*]	Added some .equs to make things a bit more readable.
#
# [trap15]	[2010/07/03]	[20:00PST]
# [*]	Added some more .equs for readability.
# [*]	Removed some ''s so that shitty editors (like the one I use :P) dont screw up
# [*]	Changed the numeric register names (e.g., lis 2, 0x8000) to named registers (e.g., lis r2, 0x8000)

	.globl _start

	.equ	r0,	0
	.equ	r1,	1
	.equ	r2,	2
	.equ	r3,	3
	.equ	r4,	4
	.equ	r5,	5
	.equ	r6,	6
	.equ	r7,	7
	.equ	r8,	8
	.equ	r9,	9
	.equ	r10,	10
	.equ	r11,	11
	.equ	r12,	12
	.equ	r13,	13
	.equ	r14,	14
	.equ	r15,	15
	.equ	r16,	16
	.equ	r17,	17
	.equ	r18,	18
	.equ	r19,	19
	.equ	r20,	20
	.equ	r21,	21
	.equ	r22,	22
	.equ	r23,	23
	.equ	r24,	24
	.equ	r25,	25
	.equ	r26,	26
	.equ	r27,	27
	.equ	r28,	28
	.equ	r29,	29
	.equ	r30,	30
	.equ	r31,	31

	.equ	f0,	0
	.equ	f1,	1
	.equ	f2,	2
	.equ	f3,	3
	.equ	f4,	4
	.equ	f5,	5
	.equ	f6,	6
	.equ	f7,	7
	.equ	f8,	8
	.equ	f9,	9
	.equ	f10,	10
	.equ	f11,	11
	.equ	f12,	12
	.equ	f13,	13
	.equ	f14,	14
	.equ	f15,	15
	.equ	f16,	16
	.equ	f17,	17
	.equ	f18,	18
	.equ	f19,	19
	.equ	f20,	20
	.equ	f21,	21
	.equ	f22,	22
	.equ	f23,	23
	.equ	f24,	24
	.equ	f25,	25
	.equ	f26,	26
	.equ	f27,	27
	.equ	f28,	28
	.equ	f29,	29
	.equ	f30,	30
	.equ	f31,	31
	
	.equ	SPR_HID2,		920
	.equ	SPR_HID0,		1008
	.equ	SPR_L2CR,		1017

	.equ	BIT_INSTCACHE,		0x0800
	.equ	BIT_DATACACHE,		0x4000
	.equ	BIT_FPENABLE,		0x2000
	.equ	BIT_DATAADDRTRANS,	0x0010
	.equ	BIT_INSTADDRTRANS,	0x0020

_start:
	# Clear all GPRs
	.irp i, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
	li	\i, 0
	.endr
	
	lis	r1,     _stack_bot@h
	ori	r1, r1, _stack_bot@l
	stwu	r0, -64(r1)
	
	# Write Stack Pointer to 0x80000034
	lis	r2, 0x8000
	stw	r1, 0x34(r2)
	
#	# Set the Small Data (Read\Write) base register.
#	lis	r13,      _SDA_BASE_@h
#	ori	r13, r13, _SDA_BASE_@l
	
	bl       InitHardware	# Initialize the GameCube Hardware (Floating Point Registers, Caches, etc.) 
	bl       SystemInit	# Initialize more cache aspects, clear a few SPRs, and disable interrupts.
	
	# Clear BSS
	lis	r3,     __bss_start@h
	ori	r3, r3, __bss_start@l
	li	r4, 0
	lis	r5,     __bss_end@h
	ori	r5, r5, __bss_end@l
	sub	r5, r5, r3
	bl	memset
	
	# Branch to the user code!
mainlooping:
	bl	main
	# If the main function returns, then just loop endlessly.
	
	# trap15s note:
	# Maybe we could do something else here ;)
	# Possibly have a "return to loader" deal?
	b	mainlooping
	b	.

InitHardware:
	mflr	r31		# Store the link register in r31
	
	bl	PSInit		# Initialize Paired Singles
	bl	FPRInit		# Initialize the FPRs
	bl	CacheInit	# Initialize the system caches
	
	mtlr	r31		# Retreive the link register from r31
	blr

PSInit:
	mfspr	r3, SPR_HID2
	oris	r3, r3, 0xA000
	mtspr	SPR_HID2, 3
	
	# Set the Instruction Cache invalidation bit in HID0
	mfspr	r3, SPR_HID0
	ori	r3, r3, BIT_INSTCACHE
	mtspr	SPR_HID0, r3
	
	sync
	
	# Clear various Special Purpose Registers
	li	r3, 0
	mtspr	912, r3
	mtspr	913, r3
	mtspr	914, r3
	mtspr	915, r3
	mtspr	916, r3
	mtspr	917, r3
	mtspr	918, r3
	mtspr	919, r3
	
	# Return 
	blr

FPRInit:
	# Enable the Floating Point Registers
	mfmsr	r3
	ori	r3, r3, BIT_FPENABLE
	mtmsr	r3
	
	# Clear all of the FPRs to 0
	lis	r3,     zfloat@h
	ori	r3, r3, zfloat@l
	lfd	f0, 0(r3)
	.irp i, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
	fmr	\i, f0
	.endr
	mtfsf	0xFF, f0
	
	# Return
	blr

CacheInit:
	mflr	r0
	stw	r0,   4(r1)
	stwu	r1, -16(r1)
	stw	r31, 12(r1)
	stw	r30,  8(r1)

	mfspr	r3, SPR_HID0
	rlwinm	r0, r3, 0, 16, 16
	cmplwi	r0, 0x0000	# Check if the Instruction Cache has been enabled or not.
	bne	ICEnabled

	# If not, then enable it.
	isync
	mfspr	r3, SPR_HID0
	ori	r3, r3, BIT_INSTCACHE
	mtspr	SPR_HID0, r3

ICEnabled:
	mfspr	r3, SPR_HID0
	rlwinm	r0, r3, 0, 17, 17
	cmplwi	r0, 0x0000	# Check if the Data Cache has been enabled or not.
	bne	DCEnabled
	
	# If not, then enable it.
	sync
	mfspr	r3, SPR_HID0
	ori	r3, r3, BIT_DATACACHE
	mtspr	SPR_HID0, r3
	
DCEnabled:
	mfspr	r3, SPR_L2CR
	clrrwi	r0, r3, 31	# Clear all of the bits except 31
	cmplwi	r0, 0x0000
	bne	L2GISkip	# Skip the L2 Global Cache Invalidation process if it has already been done before.

	# Store the current state of the MSR in r30
	mfmsr	r3
	mr	r30, r3
	sync
	
	# Enable Instruction and Data Address Translation
	li	r3, BIT_INSTADDRTRANS|BIT_DATAADDRTRANS
	mtmsr	r3
	sync
	sync
	
	# Disable the L2 Global Cache.
	mfspr	r3, SPR_L2CR
	clrlwi	r3, r3, 1
	mtspr	SPR_L2CR, r3
	sync
	
	# Invalidate the L2 Global Cache.
	bl	L2GlobalInvalidate
	
	# Restore the previous state of the MSR from r30
	mr	r3, r30
	mtmsr	r3

	# Enable the L2 Global Cache and disable the L2 Data Only bit and the L2 Global Invalidate Bit.
	mfspr	r3, SPR_L2CR
	oris	r0, r3, 0x8000
	rlwinm	r3, r0, 0, 11, 9
	mtspr	SPR_L2CR, r3


L2GISkip:
	# Restore the non-volatile registers to their previous values and return.
	lwz	r0,  20(r1)
	lwz	r31, 12(r1)
	lwz	r30, 8(r1)
	addi	r1,  r1, 16
	mtlr	r0
	blr

L2GlobalInvalidate:
	mflr	r0
	stw	r0,   4(r1)
	stwu	r1, -16(r1)
	stw	r31, 12(r1)
	sync
	
	# Disable the L2 Cache.
	mfspr	r3, SPR_L2CR
	clrlwi	r3, r3, 1
	mtspr	SPR_L2CR, r3
	sync
	
	# Initiate the L2 Cache Global Invalidation process.
	mfspr	r3, SPR_L2CR
	oris	r3, r3, 0x0020
	mtspr	SPR_L2CR, r3
	
	# Wait until the L2 Cache Global Invalidation has been completed.
L2GICheckComplete:
	mfspr	r3, SPR_L2CR
	clrlwi	r0, r3, 31
	cmplwi	r0, 0x0000
	bne	L2GICheckComplete
	
	# Clear the L2 Data Only bit and the L2 Global Invalidate Bit.
	mfspr	r3, SPR_L2CR
	rlwinm	r3, r3, 0, 11, 9
	mtspr	SPR_L2CR, r3
	
	# Wait until the L2 Cache Global Invalidation status bit signifies that it is ready.
L2GDICheckComplete:
	mfspr	r3, SPR_L2CR
	clrlwi	r0, r3, 31
	cmplwi	r0, 0x0000
	bne	L2GDICheckComplete
	
	# Restore the non-volatile registers to their previous values and return.
	lwz	r0,  20(r1)
	lwz	r31, 12(r1)
	addi	r1,  r1, 16
	mtlr	r0
	blr

SystemInit:
	mflr	r0
	stw	r0,     4(r1)
	stwu	r1, -0x18(r1)
	stw	r31, 0x14(r1)
	stw	r30, 0x10(r1)
	stw	r29, 0x0C(r1)

	# Disable interrupts!
	mfmsr	r3
	rlwinm	r4, r3, 0, 17, 15
	rlwinm	r4, r4, 0, 26, 24
	mtmsr	r4

	# Clear various SPRs
	li	r3, 0
	mtspr	952, r3
	mtspr	956, r3
	mtspr	953, r3
	mtspr	954, r3
	mtspr	957, r3
	mtspr	958, r3

	# Disable Speculative Bus Accesses to non-guarded space from both caches.
	mfspr	r3, SPR_HID0
	ori	r3, r3, 0x0200
	mtspr	SPR_HID0, r3

	# Set the Non-IEEE mode in the FPSCR
	mtfsb1	r29
	
	mfspr	r3, SPR_HID2
	rlwinm	r3, r3, 0, 2, 0
	mtspr	SPR_HID2, r3

	# Restore the non-volatile registers to their previous values and return.
	lwz	r0,  0x1C(r1)
	lwz	r31, 0x14(r1)
	lwz	r30, 0x10(r1)
	lwz	r29, 0x0C(r1)
	addi	r1, r1, 0x18
	mtlr	r0
	blr

zfloat:
	.float	0
	.align	4

