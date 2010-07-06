ifeq ($(strip $(WIIDEV)),)
$(error "Set WIIDEV in your environment.")
endif

include $(WIIDEV)/broadway.mk

DEFINES += 
INCLUDES += -Iinclude

CFLAGS += -Wno-unused-parameter 

#debug flags; DU = Debug USB
#CFLAGS += -D_DU_OHCI_F      # fire @ u/h/ohci.c
CFLAGS  += -D_DU_OHCI_F_HALT # halted @ u/h/ohci.c
CFLAGS  += -D_DU_OHCI_Q      # enqueue @ u/h/ohci.c
CFLAGS  += -D_DU_OHCI_RH     # roothub @ u/h/ohci.c
CFLAGS  += -D_DU_CORE        # @ u/c/core.c
CFLAGS  += -D_DU_CORE_ADD    # @ u/c/core.c
CFLAGS  += -D_DU_USB         # @ u/c/usb.c

TARGET = libhextwelve.a

OBJS = host/ohci.o core/core.o core/usb.o lib/list.o drivers/class/hid.o hextwelve.o

include $(WIIDEV)/common.mk

.PHONY: install
install: $(TARGET)
	@echo "  COPY      $(TARGET)"
	@cp $(TARGET) $(WIIDEV)/lib
	@mkdir -p $(WIIDEV)/include/hextwelve
	@mkdir -p $(WIIDEV)/include/hextwelve/core
	@mkdir -p $(WIIDEV)/include/hextwelve/drivers
	@mkdir -p $(WIIDEV)/include/hextwelve/drivers/class
	@mkdir -p $(WIIDEV)/include/hextwelve/drivers/mon
	@mkdir -p $(WIIDEV)/include/hextwelve/host
	@mkdir -p $(WIIDEV)/include/hextwelve/lib
	@mkdir -p $(WIIDEV)/include/hextwelve/usbspec
	@echo "  COPY      core.h"
	@cp include/hextwelve/core/core.h $(WIIDEV)/include/hextwelve/core/
	@echo "  COPY      usb.h"
	@cp include/hextwelve/core/usb.h $(WIIDEV)/include/hextwelve/core/
	@echo "  COPY      hid.h"
	@cp include/hextwelve/drivers/class/hid.h $(WIIDEV)/include/hextwelve/drivers/class/
	@echo "  COPY      hub.h"
	@cp include/hextwelve/drivers/class/hub.h $(WIIDEV)/include/hextwelve/drivers/class/
	@echo "  COPY      storage.h"
	@cp include/hextwelve/drivers/class/storage.h $(WIIDEV)/include/hextwelve/drivers/class/
	@echo "  COPY      mon.h"
	@cp include/hextwelve/drivers/mon/mon.h $(WIIDEV)/include/hextwelve/drivers/mon/
	@echo "  COPY      host.h"
	@cp include/hextwelve/host/host.h $(WIIDEV)/include/hextwelve/host/
	@echo "  COPY      ohci.h"
	@cp include/hextwelve/host/ohci.h $(WIIDEV)/include/hextwelve/host/
	@echo "  COPY      list.h"
	@cp include/hextwelve/lib/list.h $(WIIDEV)/include/hextwelve/lib/
	@echo "  COPY      usb11spec.h"
	@cp include/hextwelve/usbspec/usb11spec.h $(WIIDEV)/include/hextwelve/usbspec/
	@echo "  COPY      hextwelve.h"
	@cp include/hextwelve.h $(WIIDEV)/include/
