include ../common/broadway.mk

DEFINES = -DLACKS_SYS_TYPES_H -DLACKS_ERRNO_H -DLACKS_STDLIB_H -DLACKS_STRING_H -DLACKS_STRINGS_H -DLACKS_UNISTD_H -DCAN_HAZ_IRQ
LIBS = -lgcc

TARGET = libbroadway.a

OBJS = realmode.o string.o sync.o time.o printf.o exception.o exception_2200.o malloc.o gecko.o ipc.o mini_ipc.o irq.o
NOLINKOBJS = crt0.o

include ../common/common.mk

install: $(TARGET)
	@echo "  COPY      $(TARGET)"
	@cp $(TARGET) $(WIIDEV)/lib
	@echo "  COPY      crt0.o"
	@cp crt0.o $(WIIDEV)/powerpc-elf/lib/
	@mkdir -p $(WIIDEV)/include/broadway
	@echo "  COPY      bootmii_ppc.h"
	@cp bootmii_ppc.h $(WIIDEV)/include/broadway/
	@echo "  COPY      hollywood.h"
	@cp hollywood.h $(WIIDEV)/include/broadway/
	@echo "  COPY      ipc.h"
	@cp ipc.h $(WIIDEV)/include/broadway/
	@echo "  COPY      irq.h"
	@cp irq.h $(WIIDEV)/include/broadway/
	@echo "  COPY      mini_ipc.h"
	@cp mini_ipc.h $(WIIDEV)/include/broadway/
	@echo "  COPY      malloc.h"
	@cp malloc.h $(WIIDEV)/include/
	@echo "  COPY      printf.h"
	@cp printf.h $(WIIDEV)/include/
	@echo "  COPY      string.h"
	@cp string.h $(WIIDEV)/include/
	@echo "  COPY      types.h"
	@cp types.h $(WIIDEV)/include/

.PHONY: upload

