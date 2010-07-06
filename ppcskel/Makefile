ifeq ($(strip $(WIIDEV)),)
$(error "Set WIIDEV in your environment.")
endif

include $(WIIDEV)/broadway.mk

DEFINES += -DLACKS_SYS_TYPES_H -DLACKS_ERRNO_H -DLACKS_STDLIB_H -DLACKS_STRING_H -DLACKS_STRINGS_H -DLACKS_UNISTD_H -DCAN_HAZ_IRQ
LIBS += -lcereal -ldiskmii -lbroadway

TARGET = ppcboot.elf

OBJS = main.o

include $(WIIDEV)/common.mk


.PHONY: upload

