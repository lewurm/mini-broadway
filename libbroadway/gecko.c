/*
	BootMii - a Free Software replacement for the Nintendo/BroadOn bootloader.
	Requires mini.

Copyright (c) 2008		Nuke - <wiinuke@gmail.com>
Copyright (C) 2008, 2009	Hector Martin "marcan" <marcan@marcansoft.com>
Copyright (C) 2009		Andre Heider "dhewg" <dhewg@wiibrew.org>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#include <broadway.h>
#include <printf.h>

static int gecko_console_enabled = 0;

static u32 _gecko_command(u32 command) {
	u32 i;
	// Memory Card Port B (Channel 1, Device 0, Frequency 3 (32Mhz Clock))
	write32(EXI1_CSR, 0xd0);
	write32(EXI1_DATA, command);
	write32(EXI1_CR, 0x19);
	i = 1000;
	while ((read32(EXI1_CR) & 1) && (i--));
	i = read32(EXI1_DATA);
	write32(EXI1_CSR, 0);
	return i;
}

static u32 _gecko_getid(void)
{
	u32 i;
	// Memory Card Port B (Channel 1, Device 0, Frequency 3 (32Mhz Clock))
	write32(EXI1_CSR, 0xd0);
	write32(EXI1_DATA, 0);
	write32(EXI1_CR, 0x19);
	while (read32(EXI1_CR) & 1);
	write32(EXI1_CR, 0x39);
	while (read32(EXI1_CR) & 1);
	i = read32(EXI1_DATA);
	write32(EXI1_CSR, 0);
	return i;
}

static u32 _gecko_sendbyte(char sendbyte) {
	u32 i = 0;
	i = _gecko_command(0xB0000000 | (sendbyte<<20));
	if (i&0x04000000)
		return 1; // Return 1 if byte was sent
	return 0;
}

#if 0
static u32 _gecko_recvbyte(char *recvbyte) {
	u32 i = 0;
	*recvbyte = 0;
	i = _gecko_command(0xA0000000);
	if (i&0x08000000) {
		// Return 1 if byte was received
		*recvbyte = (i>>16)&0xff;
		return 1;
	}
	return 0;
}

static u32 _gecko_checksend(void) {
	u32 i = 0;
	i = _gecko_command(0xC0000000);
	if (i&0x04000000)
		return 1; // Return 1 if safe to send
	return 0;
}

static u32 _gecko_checkrecv(void) {
	u32 i = 0;
	i = _gecko_command(0xD0000000);
	if (i&0x04000000)
		return 1; // Return 1 if safe to recv
	return 0;
}

static void gecko_flush(void) {
	char tmp;
	while(_gecko_recvbyte(&tmp));
}
#endif

static int gecko_isalive(void) {
	u32 i;

	i = _gecko_getid();
	if (i != 0x00000000)
		return 0;

	i = _gecko_command(0x90000000);
	if ((i & 0xFFFF0000) != 0x04700000)
		return 0;

	return 1;
}

#if 0
static int gecko_recvbuffer(void *buffer, u32 size) {
	u32 left = size;
	char *ptr = (char*)buffer;

	while(left>0) {
		if(!_gecko_recvbyte(ptr))
			break;
		ptr++;
		left--;
	}
	return (size - left);
}
#endif

static int gecko_sendbuffer(const void *buffer, u32 size) {
	u32 left = size;
	char *ptr = (char*)buffer;

	while(left>0) {
		if(!_gecko_sendbyte(*ptr))
			break;
		ptr++;
		left--;
	}
	return (size - left);
}

#if 0
static int gecko_recvbuffer_safe(void *buffer, u32 size) {
	u32 left = size;
	char *ptr = (char*)buffer;
	
	while(left>0) {
		if(_gecko_checkrecv()) {
			if(!_gecko_recvbyte(ptr))
				break;
			ptr++;
			left--;
		}
	}
	return (size - left);
}

static int gecko_sendbuffer_safe(const void *buffer, u32 size) {
	u32 left = size;
	char *ptr = (char*)buffer;
	
	while(left>0) {
		if(_gecko_checksend()) {
			if(!_gecko_sendbyte(*ptr))
				break;
			ptr++;
			left--;
		}
	}
	return (size - left);
}
#endif

void gecko_init(void)
{
	// unlock EXI
	write32(0x0d00643c, 0);

	write32(EXI0_CSR, 0);
	write32(EXI1_CSR, 0);
	write32(EXI2_CSR, 0);
	write32(EXI0_CSR, 0x2000);
	write32(EXI0_CSR, 3<<10);
	write32(EXI1_CSR, 3<<10);

	if (!gecko_isalive())
		return;

	gecko_console_enabled = 1;
}

int printf(const char *fmt, ...) {
	if (!gecko_console_enabled)
		return 0;

	udelay(100); // evil hack :)

	va_list args;
	char buffer[1024];
	int i;

	va_start(args, fmt);
	i = vsprintf(buffer, fmt, args);
	va_end(args);

	return gecko_sendbuffer(buffer, i);
}

