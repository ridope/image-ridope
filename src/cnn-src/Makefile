BUILD_DIR?=../build/

include $(BUILD_DIR)/software/include/generated/variables.mak
include $(SOC_DIRECTORY)/software/common.mak

DEPFLAGS+=-DPICOLIBC_FLOAT_PRINTF_SCANF

OBJDIR = ../obj

CNN_RIDOPE_DIR = ../lib/cnn_ridope
LIBDIR = ../lib/ufft
OBJECTS = $(OBJDIR)/comm_ridope.o $(OBJDIR)/isr.o $(OBJDIR)/main.o $(OBJDIR)/crt0.o $(OBJDIR)/activation_functions.o $(OBJDIR)/conv2d_layer.o $(OBJDIR)/dense_layer.o $(OBJDIR)/max_pool_layer.o $(OBJDIR)/utils.o
OBJECTS += $(OBJDIR)/layer_1_bias.o $(OBJDIR)/layer_3_bias.o $(OBJDIR)/layer_5_bias.o
OBJECTS += $(OBJDIR)/layer_1_weights.o $(OBJDIR)/layer_3_weights.o $(OBJDIR)/layer_5_weights.o

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
	$(CX) $(LDFLAGS) \
		-T linker.ld \
		-N -o $@ \
		$(OBJECTS) \
		$(PACKAGES:%=-L$(BUILD_DIR)/software/%) \
		$(LIBS:lib%=-l%) \
		-lgcc 
	chmod -x $@

$(OBJDIR)/main.o: main.cpp
	$(compilexx)

$(OBJDIR)/crt0.o: $(CPU_DIRECTORY)/crt0.S
	$(assemble)

$(OBJDIR)/%.o: %.cpp
	$(compilexx)

$(OBJDIR)/%.o: %.c
	$(compile)

$(OBJDIR)/%.o: $(LIBDIR)/%.c
	$(compile)

$(OBJDIR)/%.o: $(CNN_RIDOPE_DIR)/%.cpp
	$(compilexx)

$(OBJDIR)/%.o: $(CNN_RIDOPE_DIR)/data/%.c
	$(compile)

$(OBJDIR)/%.o: %.S
	$(assemble)

clean:
	$(RM) $(OBJECTS) $(OBJECTS:.o=.d) demo.elf demo.bin .*~ *~

.PHONY: all main.o clean load
