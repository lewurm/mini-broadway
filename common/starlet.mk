ifeq ($(strip $(WIIDEV)),)
$(error "Set WIIDEV in your environment.")
endif

PREFIX	 = $(WIIDEV)/bin/armeb-eabi-

ARCH     = arm
CFLAGS	+= -mbig-endian -mcpu=arm926ej-s
CFLAGS	+= -fomit-frame-pointer -ffunction-sections
CFLAGS	+= -Wall -Wextra -Os -pipe -DBM_STARLET -DBOOTMII
ASFLAGS	+=
LDFLAGS	+= -mbig-endian -n -nostartfiles -nodefaultlibs -Wl,-gc-sections
LDSCRIPT = $(WIIDEV)/mini.ld
LIBS = -lgcc
