ifeq ($(strip $(WIIDEV)),)
$(error "Set WIIDEV in your environment.")
endif

include $(WIIDEV)/broadway.mk

DEFINES += 
INCLUDES += -Iinclude

CFLAGS += -Wno-unused-parameter 

TARGET = libcereal.a

OBJS = cereal.o host.o ohci.o ehci.o

include $(WIIDEV)/common.mk

.PHONY: install
install: $(TARGET)
	@echo "  COPY      $(TARGET)"
	@cp $(TARGET) $(WIIDEV)/lib
	@mkdir -p $(WIIDEV)/include/cereal
	@echo "  COPY      host.h"
	@cp include/cereal/host.h $(WIIDEV)/include/cereal/
	@echo "  COPY      ohci.h"
	@cp include/cereal/ohci.h $(WIIDEV)/include/cereal/
	@echo "  COPY      ehci.h"
	@cp include/cereal/ehci.h $(WIIDEV)/include/cereal/
	@echo "  COPY      cereal.h"
	@cp include/cereal.h $(WIIDEV)/include/
