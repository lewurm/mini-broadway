/*
	BootMii - a Free Software replacement for the Nintendo/BroadOn bootloader.
	Requires mini.

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#ifndef __TYPES_H__
#define __TYPES_H__

typedef unsigned char			u8;
typedef unsigned short			u16;
typedef unsigned int			u32;
typedef unsigned long long		u64;

typedef signed char			s8;
typedef signed short			s16;
typedef signed int			s32;
typedef signed long long		s64;

typedef volatile unsigned char		vu8;
typedef volatile unsigned short		vu16;
typedef volatile unsigned int		vu32;
typedef volatile unsigned long long	vu64;

typedef volatile signed char		vs8;
typedef volatile signed short		vs16;
typedef volatile signed int		vs32;
typedef volatile signed long long	vs64;

typedef unsigned int			size_t;
typedef signed int			ssize_t;

#define NULL	((void *)0)

/* These types must be 16-bit, 32-bit or larger integer */
typedef s32				INT;
typedef u32				UINT;

/* These types must be 8-bit integer */
typedef s8				CHAR;
typedef u8				UCHAR;
typedef u8				BYTE;

/* These types must be 16-bit integer */
typedef s16				SHORT;
typedef u16				USHORT;
typedef u16				WORD;
typedef u16				WCHAR;

/* These types must be 32-bit integer */
typedef s32				LONG;
typedef u32				ULONG;
typedef u32				DWORD;

typedef s8				int8_t;
typedef s16				int16_t;
typedef s32				int32_t;
typedef s64				int64_t;
typedef u8				uint8_t;
typedef u16				uint16_t;
typedef u32				uint32_t;
typedef u64				uint64_t;

/* Boolean type */
typedef enum { FALSE = 0, TRUE } BOOL;

#endif
