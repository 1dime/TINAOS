BIN=`pwd`/libs/cross-compiler/compiler/bin/
TARGET=i686-elf
GCC=$(BIN)$(TARGET)-gcc
AS=$(BIN)$(TARGET)-as
NASM=nasm

#Install all requirements
.PHONY: install
install:
	sudo apt install nasm build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo qemu-kvm qemu-system-i386 xorriso curl
#Are we making the cross compiler?
.PHONY: cross-compiler
cross-compiler:
	cd libs/cross-compiler && make all
.PHONY: tinaos
tinaos:
	echo "Building TINAOS bootstrap..."
	mkdir -p "output"
	$(NASM) -felf32 src/boot/boot.asm -o output/boot.o
	$(GCC) -c src/kernel/kernel.c -Isrc/include -I`pwd`/libs/cross-compiler/compiler/bin/usr/local/i686-tinaos/include -I`pwd`/libs/cross-compiler/compiler/bin/usr/local/i686-tinaos/libs -o output/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wformat=0
	$(GCC) -T src/boot/linker.ld -o output/TINAOS.bin -ffreestanding -O2 -nostdlib  output/boot.o output/kernel.o -lgcc 
	mkdir -p output/isodir/boot/grub
	cp src/boot/grub.cfg output/isodir/boot/grub/grub.cfg
	cp output/TINAOS.bin output/isodir/boot/TINAOS.bin
	grub-mkrescue -o output/TINAOS.iso output/isodir
.PHONY: qemu
qemu:
	make tinaos
	qemu-system-i386  -cdrom output/TINAOS.iso -serial stdio -m 1024
.PHONY: all
all:
	make install
	make cross-compiler
	make tinaos