ifeq ($(strip $(WIIDEV)),)
$(error "Set WIIDEV in your environment.")
endif

PREFIX	 = $(WIIDEV)/bin/powerpc-elf-

CFLAGS	+= -mcpu=750 -mpaired -m32 -mhard-float -mno-eabi -mno-sdata
CFLAGS	+= -ffreestanding -ffunction-sections
CFLAGS	+= -Wall -Wextra -Os -pipe -DBM_BROADWAY -DBOOTMII
ASFLAGS	+=
LDFLAGS	+= -mcpu=750 -m32 -n -nostartfiles -nodefaultlibs -Wl,-gc-sections

ifeq ($(MAKING_LIBBROADWAY),)
LDFLAGS += $(WIIDEV)/powerpc-elf/lib/realmode.o
endif
