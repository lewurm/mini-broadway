/*
	BootMii - a Free Software replacement for the Nintendo/BroadOn bootloader.
	Requires mini.

Copyright (C) 2008		Segher Boessenkool <segher@kernel.crashing.org>
Copyright (C) 2010		Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#ifndef __PPC_H__
#define __PPC_H__

#include <types.h>
#include <printf.h>

#define OK	0
#define EFAIL	1

#define MEM2_BSS	__attribute__ ((section (".bss.mem2")))
#define MEM2_DATA	__attribute__ ((section (".data.mem2")))
#define MEM2_RODATA	__attribute__ ((section (".rodata.mem2")))
#define ALIGNED(x)	__attribute__ ((aligned(x)))

#define STACK_ALIGN(type, name, cnt, alignment)         \
	u8 _al__##name[((sizeof(type)*(cnt)) + (alignment) + \
	(((sizeof(type)*(cnt))%(alignment)) > 0 ? ((alignment) - \
	((sizeof(type)*(cnt))%(alignment))) : 0))]; \
	type *name = (type*)(((u32)(_al__##name)) + ((alignment) - (( \
	(u32)(_al__##name))&((alignment)-1))))

#define VIRT_TO_PHYS(x)		((u32)(x) & 0x3FFFFFFF)
#define PHYS_TO_VIRT(x)		((u32)(x) | 0x80000000)
#define PHYS_TO_VIRT_NOCACHE(x)	((u32)(x) | 0xC0000000)

/* Basic I/O. */
static inline u32 read32(u32 addr)
{
	u32 x;
	asm volatile("lwz %0,0(%1) ; sync" : "=r"(x) : "b"(PHYS_TO_VIRT_NOCACHE(addr)));
	return x;
}

static inline void write32(u32 addr, u32 x)
{
	asm("stw %0,0(%1) ; eieio" : : "r"(x), "b"(PHYS_TO_VIRT_NOCACHE(addr)));
}

static inline void set32(u32 addr, u32 set)
{
	write32(addr, read32(addr) | set);
}

static inline void clear32(u32 addr, u32 clear)
{
	write32(addr, read32(addr) & (~clear));
}

static inline void mask32(u32 addr, u32 clear, u32 set)
{
	write32(addr, (read32(addr) & (~clear)) | set);
}

static inline u16 read16(u32 addr)
{
	u16 x;
	asm volatile("lhz %0,0(%1) ; sync" : "=r"(x) : "b"(PHYS_TO_VIRT_NOCACHE(addr)));
	return x;
}

static inline void write16(u32 addr, u16 x)
{
	asm("sth %0,0(%1) ; eieio" : : "r"(x), "b"(PHYS_TO_VIRT_NOCACHE(addr)));
}

static inline void set16(u32 addr, u16 set)
{
	write16(addr, read16(addr) | set);
}

static inline void clear16(u32 addr, u16 clear)
{
	write16(addr, read16(addr) & (~clear));
}

static inline void mask16(u32 addr, u16 clear, u16 set)
{
	write16(addr, (read16(addr) & (~clear)) | set);
}

static inline u8 read8(u32 addr)
{
	u8 x;
	asm volatile("lbz %0,0(%1) ; sync" : "=r"(x) : "b"(PHYS_TO_VIRT_NOCACHE(addr)));
	return x;
}

static inline void write8(u32 addr, u8 x)
{
	asm("stb %0,0(%1) ; eieio" : : "r"(x), "b"(PHYS_TO_VIRT_NOCACHE(addr)));
}

static inline void set8(u32 addr, u8 set)
{
	write8(addr, read8(addr) | set);
}

static inline void clear8(u32 addr, u8 clear)
{
	write8(addr, read8(addr) & (~clear));
}

static inline void mask8(u32 addr, u8 clear, u8 set)
{
	write8(addr, (read8(addr) & (~clear)) | set);
}

/* Address mapping. */
static inline u32 virt_to_phys(const void *p)
{
	return (u32)(VIRT_TO_PHYS(p));
}

static inline void *phys_to_virt(u32 x)
{
	return (void *)(PHYS_TO_VIRT(x));
}

static inline void *phys_to_virt_nocache(u32 x)
{
	return (void *)(PHYS_TO_VIRT_NOCACHE(x));
}

/* Cache synchronisation. */
void sync_before_read(void *p, u32 len);
void sync_after_write(const void *p, u32 len);
void sync_before_exec(const void *p, u32 len);
void ppcsync();

/* Time. */
void udelay(u32 us);
u64 getticks();
#define usleep(n)	udelay(n)
#define mftb()		getticks()

/* Special purpose registers. */
#define mtspr(n, x) asm("mtspr %1,%0" : : "r"(x), "i"(n))
#define mfspr(n) ({ \
	u32 x; asm volatile("mfspr %0,%1" : "=r"(x) : "i"(n)); x; \
})

/* Exceptions. */
void exception_init(void);

/* Console. */
void gecko_init(void);
int printf(const char *fmt, ...);
void hexdump(void *d, int len);

/* Debug: blink the tray led. */
static inline void blink(void)
{
	write32(0x0d8000c0, read32(0x0d8000c0) ^ 0x20);
}

#endif

