ASSEMBLE = ../
MAKE = $(TOOL_PATH)make -r
NASM = $(ASSEMBLE)nasm.exe
DD = $(TOOL_PATH)dd.exe
DEL = del
COPY = copy

defult :
	$(MAKE) mdr

rosboot.bin: rosboot.asm Makefile
	$(NASM) rosboot.asm -o rosboot.bin

roshead.bin: roshead.asm Makefile
	$(NASM) roshead.asm -o roshead.bin

sysfunc.o: sysfunc.asm Makefile
	$(NASM) -f elf32 sysfunc.asm -o sysfunc.o

rosinit.o: rosinit.c Makefile
	gcc -o rosinit.o -c rosinit.c

sysinit.bin: rosinit.o sysfunc.o Makefile

	ld -e _MAIN -Ttext 0x00101000 rosinit.o sysfunc.o -o sysinit.o 
	objcopy -O binary sysinit.o sysinit.bin

clean:
	-$(DEL) *.o