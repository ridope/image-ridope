BUILD_DIR?=../build/

include $(BUILD_DIR)/software/include/generated/variables.mak
include $(SOC_DIRECTORY)/software/common.mak

DEPFLAGS+=-DPICOLIBC_FLOAT_PRINTF_SCANF

OBJDIR = ./obj
LIBDIR = ./lib/ufft
OBJECTS   = $(OBJDIR)/ift-dif.o $(OBJDIR)/fft-dif.o $(OBJDIR)/isr.o $(OBJDIR)/main.o $(OBJDIR)/crt0.o 

ifdef WITH_CXX
	OBJECTS += hellocpp.o
endif

all: demo.bin

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
	chmod -x $@

demo.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) \
		-T linker.ld \
		-N -o $@ \
		$(OBJECTS) \
		$(PACKAGES:%=-L$(BUILD_DIR)/software/%) \
		$(LIBS:lib%=-l%) \
		-lgcc
	chmod -x $@

$(OBJDIR)/main.o: main.c
	$(compile)

$(OBJDIR)/crt0.o: $(CPU_DIRECTORY)/crt0.S
	$(assemble)

$(OBJDIR)/%.o: %.cpp
	$(compilexx)

$(OBJDIR)/%.o: %.c
	$(compile)

$(OBJDIR)/%.o: $(LIBDIR)/%.c
	$(compile)

$(OBJDIR)/%.o: %.S
	$(assemble)

clean:
	$(RM) $(OBJECTS) $(OBJECTS:.o=.d) demo.elf demo.bin .*~ *~

.PHONY: all main.o clean load
