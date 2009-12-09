MAKING_LIBBROADWAY = 1
include ../common/broadway.mk

DEFINES = -DLACKS_SYS_TYPES_H -DLACKS_ERRNO_H -DLACKS_STDLIB_H -DLACKS_STRING_H -DLACKS_STRINGS_H -DLACKS_UNISTD_H -DCAN_HAZ_IRQ
INCLUDES = -Iinclude

TARGET = libbroadway.a

OBJS = string.o sync.o time.o printf.o exception.o exception_2200.o malloc.o gecko.o ipc.o mini_ipc.o irq.o sha1.o
NOLINKOBJS = crt0.o realmode.o

include ../common/common.mk

install: $(TARGET)
	@echo "  COPY      $(TARGET)"
	@cp $(TARGET) $(WIIDEV)/lib
	@echo "  COPY      crt0.o"
	@cp crt0.o $(WIIDEV)/powerpc-elf/lib/
	@echo "  COPY      realmode.o"
	@cp realmode.o $(WIIDEV)/powerpc-elf/lib/
	@mkdir -p $(WIIDEV)/include/broadway
	@echo "  COPY      bootmii_ppc.h"
	@cp include/broadway/bootmii_ppc.h $(WIIDEV)/include/broadway/
	@echo "  COPY      hollywood.h"
	@cp include/broadway/hollywood.h $(WIIDEV)/include/broadway/
	@echo "  COPY      ipc.h"
	@cp include/broadway/ipc.h $(WIIDEV)/include/broadway/
	@echo "  COPY      irq.h"
	@cp include/broadway/irq.h $(WIIDEV)/include/broadway/
	@echo "  COPY      mini_ipc.h"
	@cp include/broadway/mini_ipc.h $(WIIDEV)/include/broadway/
	@echo "  COPY      broadway.h"
	@cp include/broadway.h $(WIIDEV)/include/
	@echo "  COPY      malloc.h"
	@cp include/malloc.h $(WIIDEV)/include/
	@echo "  COPY      printf.h"
	@cp include/printf.h $(WIIDEV)/include/
	@echo "  COPY      string.h"
	@cp include/string.h $(WIIDEV)/include/
	@echo "  COPY      sha1.h"
	@cp include/sha1.h $(WIIDEV)/include/
	@echo "  COPY      types.h"
	@cp include/types.h $(WIIDEV)/include/
	@echo "  COPY      common.mk"
	@cp ../common/common.mk $(WIIDEV)/
	@echo "  COPY      starlet.mk"
	@cp ../common/starlet.mk $(WIIDEV)/
	@echo "  COPY      broadway.mk"
	@cp ../common/broadway.mk $(WIIDEV)/

