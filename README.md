# image-ridope
On Prototyping image processing on RISC-V SoCs

# Pre-requisites

- Update Linker file

  If you're not using the linker provided in the [src/linker.ld](src/linker.ld), add the following two lines at the end of the file:
  ```
  PROVIDE(__heap_start = _end );
  PROVIDE(__heap_end = _start);
  ```
  
- Update Makefile

  If you're not using the Makefile provided in the [src/Makefile](src/Makefile), the Libcc library needs to be linked, to do that is necessary to use the following flag `-lgcc`:
  
  ```
    demo.elf: $(OBJECTS)
	  $(CC) $(LDFLAGS) \
		-T linker.ld \
		-N -o $@ \
		$(OBJECTS) \
		$(PACKAGES:%=-L$(BUILD_DIR)/software/%) \
		$(LIBS:lib%=-l%) \
		-lgcc
	chmod -x $@
  ```
  
- ***Optional***: Enable printf to work with float point numbers:

  If you're not using the Makefile provided in the [src/Makefile](src/Makefile), add the following line after the include section, `DEPFLAGS+=-DPICOLIBC_FLOAT_PRINTF_SCANF`:
  
  ```
  BUILD_DIR?=../build/

  include $(BUILD_DIR)/software/include/generated/variables.mak
  include $(SOC_DIRECTORY)/software/common.mak

  DEPFLAGS+=-DPICOLIBC_FLOAT_PRINTF_SCANF
  ```
  

 

