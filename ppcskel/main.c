/*
        BootMii - a Free Software replacement for the Nintendo/BroadOn bootloader.
        Requires mini.

Copyright (C) 2008, 2009        Haxx Enterprises <bushing@gmail.com>
Copyright (C) 2009              Andre Heider "dhewg" <dhewg@wiibrew.org>
Copyright (C) 2008, 2009        Hector Martin "marcan" <marcan@marcansoft.com>
Copyright (C) 2008, 2009        Sven Peter <svenpeter@gmail.com>
Copyright (C) 2009              John Kelley <wiidev@kelley.ca>
Copyright (C) 2009-2010		Alex Marshall <trap15@raidenii.net>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#include <broadway.h>
#include <string.h>
#include <malloc.h>
#include <printf.h>
#include <sha1.h>
#include <diskmii/nandfs.h>
#include <diskmii.h>
#include <diskmii/diskio.h>
#include <cereal.h>
#include <video_low.h>
#include <input.h>
#include <console.h>

#define DELAY_TIME		(500000)
#define IRQ_STABLE_TIME		(1000000)

#define MINIMUM_MINI_VERSION	(0x00010004)

char spinchrs[5] = "|/-\\";
otp_t otp;
seeprom_t seeprom;
int omgmagicalfairyponies = 1;

static void dsp_reset(void)
{
	mask16(0x0C00500A, 0x01F8, 0x0010);
	write16(0x0C005036, 0);
}

static char ascii(char s)
{
	if(s < 0x20) return '.';
	if(s > 0x7E) return '.';
	return s;
}

void hexdump(void *d, int len)
{
	u8 *data;
	int i, off;
	data = (u8*)d;
	for(off = 0; off < len; off += 16) {
		printf("%08x  ",off);
		for(i = 0; i < 16; i++)
			if((i + off) >= len) printf("   ");
			else printf("%02x ", data[off + i]);

		printf(" ");
		for(i = 0; i < 16; i++)
			if((i + off) >= len) printf(" ");
			else printf("%c", ascii(data[off + i]));
		printf("\n");
	}
}
	
void testOTP(void)
{
	printf("reading OTP...\n");
	getotp(&otp);
	printf("read OTP!\n");
	printf("OTP:\n");
	hexdump(&otp, sizeof(otp));

	printf("reading SEEPROM...\n");
	getseeprom(&seeprom);
	printf("read SEEPROM!\n");
	printf("SEEPROM:\n");
	hexdump(&seeprom, sizeof(seeprom));
}

int main(void)
{
	u16 ret;
	int vmode = -1;
	exception_init();
	dsp_reset();

	irq_initialize();
	
	ipc_initialize();
	ipc_slowping();

	gecko_init();
	input_init();
	init_fb(vmode);

	VIDEO_Init(vmode);
	VIDEO_SetFrameBuffer(get_xfb());
	VISetupEncoder();

	gfx_printf("\n\n");
	gfx_printf("Booting!\n");
	usleep(DELAY_TIME);
	u32 version = ipc_getvers();
	u16 mini_version_major = version >> 16 & 0xFFFF;
	u16 mini_version_minor = version & 0xFFFF;
	gfx_printf("Mini version: %d.%0d\n", mini_version_major, mini_version_minor);
	
	if(version < MINIMUM_MINI_VERSION) {
		gfx_printf("Sorry, this version of MINI (armboot.bin)\n"
			   "is too old, please update to at least %d.%0d.\n", 
			   (MINIMUM_MINI_VERSION >> 16), (MINIMUM_MINI_VERSION & 0xFFFF));
		/* Here's a better idea:
		 * Return because our crt0 can handle it :)
		 */
		return 1;
	}
	
	gfx_printf("Turning on Hardware Broadway IRQ!\n");
	usleep(DELAY_TIME);
	irq_bw_enable(IRQ_BW_HW);
	gfx_printf("Switching on some IRQs!\n");
	usleep(DELAY_TIME);
	irq_hw_enable(IRQ_HW_PPCIPC);
	irq_hw_enable(IRQ_HW_RESET);
#if 0
	gfx_printf("Initing libcereal!\n");
	usleep(DELAY_TIME);
	if((ret = cereal_init())) {
		gfx_printf("Unable to init libcereal %d! bailing!\n", ret);
		return 1;
	}
	gfx_printf("Success.\n");
	gfx_printf("Initing USB IRQs...\n");
	usleep(DELAY_TIME);
	/* external ohci */
	irq_hw_enable(IRQ_HW_OHCI0);
	/* internal ohci */
	//irq_hw_enable(IRQ_HW_OHCI1);
	
	gfx_printf("Initing libcereal USB driver...\n");
	usleep(DELAY_TIME);
	/* external ohci */
	cereal_install(IRQ_HW_OHCI0);

	/* internal ohci */
	//cereal_install(IRQ_HW_OHCI1);

	gfx_printf("Loading HID Keyboard driver...\n");
	usleep(DELAY_TIME);
	/* load HID keyboard driver */
#endif
	
	/* wait for usb keyboard plugged in */
	if(omgmagicalfairyponies/* Keyboard plugged in */) {
		gfx_printf("plug in a usb keyboard");
	}
	while(omgmagicalfairyponies/* Keyboard plugged in */) {
//		printf("Spinning...\n");
		usleep(20);
		if(reset_pressed) {
			omgmagicalfairyponies = 0;
			gfx_printf("Reset hit...\n");
			usleep(DELAY_TIME * 20);
			gfx_printf("Load Sysmenu...\n");
			boot2_run(1,2);
			return 1;
		}
	}

	gfx_printf("hello keyboard :)");
	
	/* Do Keyboard shit here */
	
	gfx_printf("===============================\n");

	gfx_printf("bye, world!\n");

#if 0
	gfx_printf("Deiniting libcereal USB driver...\n");
	usleep(DELAY_TIME);
	/* external ohci */
	cereal_uninstall(IRQ_HW_OHCI0);
	/* internal ohci */
	//cereal_uninstall(IRQ_HW_OHCI1);
	
	cereal_quit();
#endif
	
	return 0;
}

