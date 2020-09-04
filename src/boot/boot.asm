; Some multiboot header constants
MBALIGN equ 1 << 0
MEMINFO equ 1 << 1
FLAGS equ MBALIGN | MEMINFO
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
	dd	MAGIC
	dd FLAGS
	dd CHECKSUM

section .data
align 4096

section .bss
align 16
global stack_bottom
stack_bottom:
resb 16384
global stack_top
stack_top:
section .text
global _start:function (_start.end - _start)
_start:
		mov esp, stack_top
		extern kernel_start
		call kernel_start
		cli
		hlt
.end:
