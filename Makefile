BIN=`pwd`/libs/cross-compiler/compiler/bin/
TARGET=i686-elf
GCC=$(BIN)$(TARGET)-gcc
AS=$(BIN)$(TARGET)-as
NASM=$(BIN)$(TARGET)-nasm

#Install all requirements
install:
	sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo qemu-kvm qemu-system-i386 xorriso curl
#Are we making the cross compiler?
cross-compiler:
	cd libs/cross-compiler && make all
tinaos:
	echo "Building TINAOS bootstrap..."
	mkdir -p "output"
	$(AS) src/boot/boot.s -o output/boot.o
	$(GCC) -c src/kernel/kernel.c -Isrc/include -Isrc/include/newlib/include -Isrc/include/newlib/lib -o output/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	$(GCC) -T src/boot/linker.ld -o output/TINAOS.bin -ffreestanding -O2 -nostdlib  output/boot.o output/kernel.o -lgcc
	mkdir -p output/isodir/boot/grub
	cp src/boot/grub.cfg output/isodir/boot/grub/grub.cfg
	cp output/TINAOS.bin output/isodir/boot/TINAOS.bin
	grub-mkrescue -o output/TINAOS.iso output/isodir
usb:
	make tinaos
	sudo dd if=output/TINAOS.iso of=$(USB) && sync
qemu:
	make tinaos
	qemu-system-i386  -cdrom output/TINAOS.iso -serial stdio -m 1024
all:
	make install
	make cross-compiler
	make tinaos