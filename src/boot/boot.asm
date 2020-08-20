; Some multiboot header constants
MBALIGN equ 1 << 0
MEMINFO equ 1 << 1
FLAGS equ MBALIGN | MEMINFO
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)
VM_BASE     equ 0xC0000000
PDE_INDEX   equ (VM_BASE >> 22)
PSE_BIT     equ 0x00000010
PG_BIT      equ 0x80000000

section .multiboot
align 4
	dd	MAGIC
	dd FLAGS
	dd CHECKSUM

section .data
align 4096
tmp_page_dir:
	dd 0x00000083
	times(PDE_INDEX - 1) dd 0
	dd 0x00000083
	times(1024 - PDE_INDEX - 1) dd 0

section .bss
align 16
extern stack_bottom
stack_bottom:
resb 16384
extern stack_top
stack_top:
section .text
global _start:function (_start.end - _start)
_start:
		mov esp, stack_top
		extern kernel_start
		call kernel_start
		cli
.hang:	hlt
		jmp .hang
.end: