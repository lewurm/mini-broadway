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
#include <hextwelve.h>
#include <hextwelve/core/core.h>
#include <hextwelve/drivers/class/hid.h>
#include <video_low.h>
#include <input.h>
#include <console.h>

#define DELAY_TIME		(500000)
#define IRQ_STABLE_TIME		(1000000)

#define MINIMUM_MINI_VERSION	(0x00010002)

char spinchrs[5] = "|/-\\";
otp_t otp;
seeprom_t seeprom;

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
	u32 was_on;
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
	irq_bw_enable(BW_PI_IRQ_HW);
	gfx_printf("Switching on some IRQs!\n");
	usleep(DELAY_TIME);
	irq_hw_enable(IRQ_PPCIPC);
	irq_hw_enable(IRQ_RESET);
	gfx_printf("Initing hextwelve!\n");
	usleep(DELAY_TIME);
	if((ret = hextwelve_init())) {
		gfx_printf("Unable to init hextwelve %d! bailing!\n", ret);
		return 1;
	}
	gfx_printf("Success.\n");
	gfx_printf("Initing hextwelve IRQs...\n");
	usleep(DELAY_TIME);
	/* external ohci */
	irq_hw_enable(IRQ_OHCI0);
	/* internal ohci */
	//irq_hw_enable(IRQ_OHCI1);
	
	gfx_printf("Initing hextwelve USB driver...\n");
	usleep(DELAY_TIME);
	/* external ohci */
	usb_init(OHCI0_REG_BASE);

	/* internal ohci */
	//usb_init(OHCI1_REG_BASE);

	gfx_printf("Loading hextwelve HID Keyboard driver...\n");
	usleep(DELAY_TIME);
	/* load HID keyboard driver */
	usb_hidkb_init();

	gfx_printf("Waiting for IRQs to stabilize...\n");
	usleep(IRQ_STABLE_TIME);
wait_kb:
	/* wait for usb keyboard plugged in */
	if(!usb_hidkb_inuse()) {
		gfx_printf("plug in a usb keyboard");
	}
	while(!usb_hidkb_inuse()) {
		if(reset_pressed) {
			gfx_printf("Reset hit...\n");
			usleep(DELAY_TIME * 20);
			gfx_printf("Load Sysmenu...\n");
			boot2_run(1,2);
			return 1;
		}
	}

	gfx_printf("hello keyboard :)");

#define FONT_WIDTH  13
#define FONT_HEIGHT 15
#define STDOUT_BORDER_LEFT 20
#define STDOUT_BORDER_RIGHT 650
#define STDOUT_BORDER_TOP 20
#define STDOUT_BORDER_BOTTOM 550
#define TABSIZE 4
	/* you are welcome to make this nice :) */
	char str[7];
	u16 i, j, y=STDOUT_BORDER_TOP, x=STDOUT_BORDER_LEFT;
	u16 old_x, old_y;
	struct kbrep *k, *old=NULL;

	while(usb_hidkb_inuse()) {
		memset(str, '\0', 7);
		k = usb_hidkb_getChars();
		j=0;
		old_x = x; /* save actual x and y position for printing after the loop */
		old_y = y;
		for(i=0; k->keys[i]>0; i++) {

			/* dropping char's if necessary */
			if(old) {
				for(j=0; j < 6; j++) {
					if(old->keys[j] == k->keys[i]) {
						ret = 1;
						break;
					}
				}
			}
			if(ret) {
				ret = 0;
				continue;
			}
			j = 0;

			unsigned char key = usb_hidkb_get_char_from_keycode(k->keys[i],
					(k->mod & MOD_lshift) || (k->mod & MOD_rshift));
			/* no key or key not relevant? next, please. */
			if (key == 0)
				continue;

			/* RETURN pressed? */
			if (key == '\n') {
				x = STDOUT_BORDER_LEFT;
				y += FONT_HEIGHT;
				printf("\n");
			/* TAB pressed? */
			} else if (key == '\t') {
				x += (TABSIZE*FONT_WIDTH);
				printf("\t");

			/* BACKSPACE pressed? */
			} else if (key == '\r') {
				/* TODO */

			/* now we have only printable characters left */
			} else {
				x += FONT_WIDTH;
				str[j] = key;
				j++;
			}

			/* line full? break! */
			if(x > (STDOUT_BORDER_RIGHT-FONT_WIDTH)) {
				x = STDOUT_BORDER_LEFT;
				y += FONT_HEIGHT;
			}
			/* screen full? start again at top */
			if(y > (STDOUT_BORDER_BOTTOM-FONT_HEIGHT)) {
				y = STDOUT_BORDER_TOP;
			}
		}
		if(old) {
			free(old);
		}
		old = k;
		if(j > 0) { /* when there was any printable stuff, show it */
			print_str_noscroll(old_x, old_y, str);
			printf("%s", str);
		}
	}

	goto wait_kb;

	gfx_printf("===============================\n");

	gfx_printf("bye, world!\n");

	hextwelve_quit();
	return 0;
}

