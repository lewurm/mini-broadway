include ../common/broadway.mk

DEFINES = -DLACKS_SYS_TYPES_H -DLACKS_ERRNO_H -DLACKS_STDLIB_H -DLACKS_STRING_H -DLACKS_STRINGS_H -DLACKS_UNISTD_H -DCAN_HAZ_IRQ
LDSCRIPT = mini.ld
LIBS = -lgcc -lbroadway

TARGET = ppcboot.elf

OBJS = main.o input.o video_low.o nandfs.o ff.o diskio.o fat.o font.o console.o sha1.o

include usb/Makefile

include ../common/common.mk

upload: $(TARGET)
	@$(WIIDEV)/bin/bootmii -p $<

.PHONY: upload

