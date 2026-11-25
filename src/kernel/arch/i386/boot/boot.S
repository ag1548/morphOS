/* Copyright (C) 1999, 2001, 2010  Free Software Foundation, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see "http://www.gnu.org/licenses/>"
 */

#define ASM_FILE        1
#include "multiboot.h"

/* The size of our stack (16KB). */
#define STACK_SIZE                      0x4000

/* The flags for the Multiboot header. */
#define MULTIBOOT_HEADER_FLAGS          MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO

.section .multiboot
.align 4

        /* magic */
        .long   MULTIBOOT_HEADER_MAGIC
        /* flags */
        .long   MULTIBOOT_HEADER_FLAGS
        /* checksum */
        .long   -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

        .long   0 /* header_addr */
        .long   0 /* load_addr */
        .long   0 /* load_end_addr */
        .long   0 /* bss_end_addr */
        .long   0 /* entry_addr */

        /* These values arent' currently used, because no "| MULTIBOOT_VIDEO_MODE" in MULTIBOOT_HEADER_FLAGS above */
        .long 0    /* mode_type */
        .long 1024 /* width */
        .long 768  /* height */
        .long 32   /* depth */

.section .vm_structs, "aw", @nobits
.align 4096
virtual_memory_data_structures:
.skip 4096 * 13 /* 1 page directory pointer table; 4 page directories; 8 page tables for mapping bottom 16 MB */

.section .text
.align 4

        .comm   stack, STACK_SIZE, 16

        .globl  start, _start
start:
_start:

        /* At this point: */
        /* - EAX has magic value 0x2BADB002 */
        /* - EBX has physical address of multiboot_info_struct */

        /* Initialize the stack pointer. */
        movl    $(stack + STACK_SIZE), %esp

        /* Reset EFLAGS. */
        pushl   $0
        popf

        /* Put the respective values in the symbols */
        mov     %eax, multiboot_magic
        mov     %ebx, multiboot_info_ptr
        movl    $virtual_memory_data_structures, virtual_memory_data_structures_ptr

        cli /* Clear interrupt flag */
        cld /* Clear direction flag */

        /* Now enter the C main function... */
        call    cmain

        /* Halt. */
        pushl   $halt_message
        call    printf

        cli

loop:   hlt
        jmp     loop

halt_message:
        .asciz  "Halted. boot.S\n"
