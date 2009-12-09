ifeq ($(strip $(WIIDEV)),)
$(error "Set WIIDEV in your environment.")
endif

include $(WIIDEV)/broadway.mk

DEFINES += -DLACKS_SYS_TYPES_H -DLACKS_ERRNO_H -DLACKS_STDLIB_H -DLACKS_STRING_H -DLACKS_STRINGS_H -DLACKS_UNISTD_H -DCAN_HAZ_IRQ
LDSCRIPT = mini.ld
LIBS += -lgcc -lhextwelve -ldiskmii -lbroadway

TARGET = ppcboot.elf

OBJS = main.o input.o video_low.o font.o console.o

include $(WIIDEV)/common.mk

upload: $(TARGET)
	@$(WIIDEV)/bin/bootmii -p $<

.PHONY: upload

