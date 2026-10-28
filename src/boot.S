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
#define MULTIBOOT_HEADER_FLAGS          MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_VIDEO_MODE

        .text

        .globl  start, _start
start:
_start:
        jmp     multiboot_entry

        /* Align 32 bits boundary. */
        .align  4

        /* Multiboot header. */
multiboot_header:
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

        .long 0    /* mode_type */
        .long 1024 /* width */
        .long 768  /* height */
        .long 32   /* depth */

multiboot_entry:
        /* Initialize the stack pointer. */
        movl    $(stack + STACK_SIZE), %esp

        /* Reset EFLAGS. */
        pushl   $0
        popf

        /* Push the pointer to the Multiboot information structure. */
        pushl   %ebx
        /* Push the magic value. */
        pushl   %eax

        /* Now enter the C main function... */
        call    cmain

        /* Halt. */
        pushl   $halt_message
        call    printf

loop:   hlt
        jmp     loop

halt_message:
        .asciz  "Halted."

        .comm   stack, STACK_SIZE
