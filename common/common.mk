AR = $(PREFIX)ar
AS = $(PREFIX)as
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
LD = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
RANLIB = $(PREFIX)ranlib
STRIP = $(PREFIX)strip
ASFLAGS += -D_LANGUAGE_ASSEMBLY

BIN2S = $(DEVKITPPC)/bin/bin2s

ifeq ($(NOMAPFILE),)
LDFLAGS += -Wl,-Map,$(TARGET).map
endif

ifneq ($(LDSCRIPT),)
LDFLAGS += -Wl,-T$(LDSCRIPT)
endif

DEPDIR = .deps

CFLAGS	+= $(INCLUDES)

ifeq ($(MAKING_LIBBROADWAY),)
CFLAGS	+= -I$(WIIDEV)/include
LDFLAGS	+= -L$(WIIDEV)/lib
endif

.PHONY: all clean upload

all: $(TARGET)

%.elf: $(OBJS) $(NOLINKOBJS)
	@echo "  LINK      $@"
	@$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

%.a: $(OBJS) $(NOLINKOBJS)
	@echo "  ARCHIVE   $@"
	@$(AR) rcs "$@" $(OBJS)

ifneq ($(LDSCRIPT),)
$(TARGET): $(LDSCRIPT)
endif

%.o: %.c
	@echo "  COMPILE   $<"
	@mkdir -p $(DEPDIR)
	@$(CC) $(CFLAGS) $(DEFINES) -Wp,-MMD,$(DEPDIR)/$(*F).d,-MQ,"$@",-MP -c $< -o $@

%.o: %.s
	@echo "  ASSEMBLE  $<"
	@$(CC) $(CFLAGS) $(DEFINES) $(ASFLAGS) -c $< -o $@

%.o: %.S
	@echo "  ASSEMBLE  $<"
	@$(CC) $(CFLAGS) $(DEFINES) $(ASFLAGS) -c $< -o $@

clean:
	rm -rf $(DEPDIR)
	rm -f $(TARGET) $(TARGET).map $(OBJS) $(NOLINKOBJS)

ifeq ($(ARCH),ppc)
upload: $(TARGET)
	@$(WIIDEV)/bin/bootmii -p $<
endif
ifeq ($(ARCH),arm)
upload: $(TARGET)
	@$(WIIDEV)/bin/bootmii -a $<
endif

define bin2o
	@echo "  BIN2S     $(notdir $<)"
	@$(BIN2S) -a 32 $< | $(AS) -o $(@)
	@echo "extern const u8" `(echo $(<F) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`"_end[];" > `(echo $(<F) | tr . _)`.h
	@echo "extern const u8" `(echo $(<F) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`"[];" >> `(echo $(<F) | tr . _)`.h
	@echo "extern const u32" `(echo $(<F) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`_size";" >> `(echo $(<F) | tr . _)`.h
endef

-include $(DEPDIR)/*

