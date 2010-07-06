ifeq ($(strip $(WIIDEV)),)
$(error "Set WIIDEV in your environment.")
endif

include $(WIIDEV)/starlet.mk

CFLAGS += -DCAN_HAZ_IRQ -DCAN_HAZ_IPC
CFLAGS += -DMINI_VERSION_MAJOR=1 -DMINI_VERSION_MINOR=4

ELFLOADER = elfloader/elfloader.bin
MAKEBIN = python makebin.py

TARGET = armboot.elf
TARGET_BIN = armboot.bin
OBJS = start.o main.o ipc.o vsprintf.o string.o gecko.o memory.o memory_asm.o \
	utils_asm.o utils.o ff.o diskio.o sdhc.o powerpc_elf.o powerpc.o panic.o \
	irq.o irq_asm.o exception.o exception_asm.o seeprom.o crypto.o nand.o \
	boot2.o ldhack.o sdmmc.o

include $(WIIDEV)/common.mk

all: $(TARGET_BIN)

main.o: main.c git_version.h

$(TARGET_BIN): $(TARGET) $(ELFLOADER) 
	@echo  "MAKEBIN	$@"
	@$(MAKEBIN) $(ELFLOADER) $< $@

git_version.h: describesimple.sh
	@echo "  GITVER    $@"
	@echo 'const char git_version[] = "'`./describesimple.sh`'";' > git_version.h

clean: myclean

$(ELFLOADER):
	@$(MAKE) -C elfloader

.PHONY: myclean
myclean:
	-rm -f $(TARGET_BIN) git_version.h
	@$(MAKE) -C elfloader clean

