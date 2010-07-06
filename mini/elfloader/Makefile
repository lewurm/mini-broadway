ifeq ($(strip $(WIIDEV)),)
$(error "Set WIIDEV in your environment.")
endif

include $(WIIDEV)/starlet.mk

CFLAGS += -fpic
LDSCRIPT = stub.ld
LIBS = -lgcc

TARGET = elfloader.elf
TARGET_BIN = elfloader.bin
OBJS = start.o stub.o ../string.o utils.o

include $(WIIDEV)/common.mk

all: $(TARGET_BIN)

$(TARGET_BIN): $(TARGET)
	@echo  "  OBJCPY    $@"
	@$(OBJCOPY) -O binary $< $@

clean: myclean

myclean:
	-rm -f $(TARGET_BIN)

