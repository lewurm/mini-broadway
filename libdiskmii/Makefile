ifeq ($(strip $(WIIDEV)),)
$(error "Set WIIDEV in your environment.")
endif

include $(WIIDEV)/broadway.mk

DEFINES += 
INCLUDES += -Iinclude

TARGET = libdiskmii.a

OBJS = diskio.o diskmii.o syncobj.o ff.o unicode.o nandfs.o cache.o

include $(WIIDEV)/common.mk

install: $(TARGET)
	@echo "  COPY      $(TARGET)"
	@cp $(TARGET) $(WIIDEV)/lib
	@mkdir -p $(WIIDEV)/include/diskmii
	@echo "  COPY      diskio.h"
	@cp include/diskmii/diskio.h $(WIIDEV)/include/diskmii/
	@echo "  COPY      ff.h"
	@cp include/diskmii/ff.h $(WIIDEV)/include/diskmii/
	@echo "  COPY      ffconf.h"
	@cp include/diskmii/ffconf.h $(WIIDEV)/include/diskmii/
	@echo "  COPY      nandfs.h"
	@cp include/diskmii/nandfs.h $(WIIDEV)/include/diskmii/
	@echo "  COPY      cache.h"
	@cp include/diskmii/cache.h $(WIIDEV)/include/diskmii/
	@echo "  COPY      diskmii.h"
	@cp include/diskmii.h $(WIIDEV)/include/
