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
  
  Also, go into your litex installation folder, and change the following Makefile in line 46: \litex\litex\soc\software\libc\Makefile. Change the flag `-Dformat-default` to double.
  
  ```
  	meson $(PICOLIBC_DIRECTORY) \
		-Dmultilib=false \
		-Dpicocrt=false \
		-Datomic-ungetc=false \
		-Dthread-local-storage=false \
		-Dio-long-long=true \
		-Dformat-default=double \
		-Dincludedir=picolibc/$(TRIPLE)/include \
		-Dlibdir=picolibc/$(TRIPLE)/lib \
		--cross-file cross.txt
  ```
  
  
# Usage

 - Build and load the SOC
   - Go to the target directory
  
     `cd target/`
     
   - Build and load
    
     `sudo python3 ./soc_ridope.py --build --load`

  - Upload the C code

    The C code only waits for the communication, from the python script, to receive an Image and perform a FFT, everything else is ignored.
    
    - Go to the src directory
      
      `cd ../src/`
      
    - Build
      
      Change <absolute_path> to the absolute path for the cloned repository
      
      `make BUILD_DIR="<absolute_path>/target/build/terasic_de10lite/"`
      
    - Load

      Check if the board is available at /dev/ttyUSB0
      
      `sudo lxterm /dev/ttyUSB0 --kernel demo.bin`
      
       Wait few seconds and type reboot to upload the code, after succesfully uploading the code use Ctrl ^ C to exit.
       
  - Running the python script
    
    In the same directory as the previous step, run the python script:
    
    `sudo python3 ./send-img.py`
    
    - There are only two commands available 'send' and 'reboot'.
      - Send command:
        
        Make sure you have an image in `../img/image_NB32.bmp` and then type "send", the script will send the image to the softcore and receive the result of the FFT2D and save it as `../img/fft-image_NB32.png`
        
      - Reboot command:
        
        This command will reboot the softcore, but since there's no bin file being sent throught the UART, the softcore will be stuck in the bootloader program. So, you'll have to upload the C code again, explained in the last step.
       
       
 

