/* Constants for multiboot header */
.set ALIGN, 1<<0 /*Align loaded modules*/
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)
.set VM_BASE, 0xC0000000
.set PDE_INDEX, VM_BASE >> 2

/*Declare a multiboot header*/
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long 0, 0, 0, 0, 0
.long 0 # 0 = set graphics mode
.long 1024, 768, 32 # Width, height, depth

.section .bss
.align 16
.global stack_bottom, stack_top
stack_bottom:
.skip 16384
stack_top:

//Linker script specifies _start as entry to kernel
.section .text
.global _start
.type _start, @function
_start:
    //We are now into 32-bit protected mode
    //Every man for himself
    //Setup a stack by setting esp register
    mov $stack_top, %esp
    //Enter high-level kernel
    call kernel_start
    //If nothing left, loop
    cli
    hlt
.hang:    
    jmp .hang
//Set size of start symbol
.size _start, . - _start
