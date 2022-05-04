BUILD_DIR?=../build/

include $(BUILD_DIR)/software/include/generated/variables.mak
include $(SOC_DIRECTORY)/software/common.mak

DEPFLAGS+=-DPICOLIBC_FLOAT_PRINTF_SCANF
INCLUDES += -I/home/lucas/tflite-micro/
INCLUDES += -I/home/lucas/tflite-micro/tensorflow/lite/micro/tools/make/downloads/flatbuffers/include


OBJDIR = ./obj
TFT_MICRO_DIR = /home/lucas/tflite-micro/tensorflow/lite/micro/tools/make/gen/zephyr_vexriscv_x86_64_default/lib
#LIBDIR = ./lib/ufft
OBJECTS = $(OBJDIR)/comm_ridope.o $(OBJDIR)/isr.o $(OBJDIR)/main.o $(OBJDIR)/crt0.o

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
		-L$(TFT_MICRO_DIR) \
		$(LIBS:lib%=-l%) \
		-ltensorflow-microlite
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
