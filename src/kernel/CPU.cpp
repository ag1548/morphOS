#include "CPU.h"

#include "types.h"

/*

CPU
===
EAX=00000000 EBX=00000000 ECX=00000000 EDX=00000663
ESI=00000000 EDI=00000000 EBP=00000000 ESP=00000000
EIP=0000fff0 EFL=00000002 [-------] CPL=0 II=0 A20=1 SMM=0 HLT=0
ES =0000 00000000 0000ffff 00009300
CS =f000 ffff0000 0000ffff 00009b00
SS =0000 00000000 0000ffff 00009300
DS =0000 00000000 0000ffff 00009300
FS =0000 00000000 0000ffff 00009300
GS =0000 00000000 0000ffff 00009300
LDT=0000 00000000 0000ffff 00008200
TR =0000 00000000 0000ffff 00008b00
GDT=     00000000 0000ffff
IDT=     00000000 0000ffff
CR0=60000010 CR2=00000000 CR3=00000000 CR4=00000000
DR0=00000000 DR1=00000000 DR2=00000000 DR3=00000000
DR6=ffff0ff0 DR7=00000400
CCS=00000000 CCD=00000000 CCO=DYNAMIC
EFER=0000000000000000
FCW=037f FSW=0000 [ST=0] FTW=00 MXCSR=00001f80
FPR0=0000000000000000 0000 FPR1=0000000000000000 0000
FPR2=0000000000000000 0000 FPR3=0000000000000000 0000
FPR4=0000000000000000 0000 FPR5=0000000000000000 0000
FPR6=0000000000000000 0000 FPR7=0000000000000000 0000
XMM00=0000000000000000 0000000000000000 XMM01=0000000000000000 0000000000000000
XMM02=0000000000000000 0000000000000000 XMM03=0000000000000000 0000000000000000
XMM04=0000000000000000 0000000000000000 XMM05=0000000000000000 0000000000000000
XMM06=0000000000000000 0000000000000000 XMM07=0000000000000000 0000000000000000

Rings
=====
Ring 0 - Kernel
Ring 1 - Device Drivers
Ring 2 - Shared Libs
Ring 3 - User Applications

FLAGS - 4 bits
==============
0 Granularity: Granularity flag, indicates the size the Limit value is scaled by
  - 0 - the Limit is in 1 Byte blocks (byte granularity)
  - 1 - the Limit is in 4 KiB blocks (page granularity)
0 "DB"/"Data/Bit" Size flag (A GDT can have both 16-bit and 32-bit selectors at once)
  - 0 - the descriptor defines a 16-bit protected mode segment
  - 1 - the descriptor defines a 32-bit protected mode segment
0 Long-mode code flag
  - 1 - the descriptor defines a 64-bit code segment. When set, DB should always be clear.
  - 0 - For any other type of segment (other code types or any data segment), it should be clear (0).
0 - Reserved

ACCESS - 8 bits
===============
0 Present: Must be `1` for any valid segment
00 Descriptor Privilege Level
  - 00 - most privilege
  - 01
  - 10
  - 11 - least privilege
0 Descriptor type:
  - 0 - system ("task state") segment, where each _task_ can have a hardware-controlled segment
  - 1 - code/data segment
0 Executable:
  - 0 - data
  - 1 - code
0 Direction/Conforming:
  - data selectors = direction bit
    - 0 grows up
    - 1 grows down
  - code selectors = conforming bit
    - 0 - code can only be executed by ring set in DPL
    - 1 - code can only be executed by an equal or lower-privilege ring
0 Readable
  - code selectors = readable bit; write access is never allowed for code segments
  - data selectors = writeable bit; read access is always allowed for data segments
0 Accessed
  - Set by the CPU when the segment is accessed (but, "best left set to 1 unless otherwise needed")

Segment Descriptor
==================

 1            0.5      0.5      1            1             2                    2
┌────────────┬────────┬────────┬────────────┬─────────────┬────────────────────┬────────────────────┐
│63        56│55    52│51    48│47        40│39         32│31                16│15                 0│
│    Base    │ Flags  │ Limit  │   Access   │ Base        │       Base         │      Limit         │
│31        24│3      0│19    16│7          0│23         16│15                 0│15                 0│
└────────────┴────────┴────────┴────────────┴─────────────┴────────────────────┴────────────────────┘
 8            4        4        8            8             16                   16


NULL - 0x0
============================================================================
00000000 0000___0000 00000000 00000000 | 00000000_00000000 00000000_00000000
Base     Flags Limit Access   Base     | Base              Limit

CODE - 0x00CF9A00_0000FFFF
0x00     CF          9A       00       | 00       00       FF       FF
============================================================================
00000000 1100___1111 10011010 00000000 | 00000000_00000000 11111111_11111111
Base     Flags Limit Access   Base     | Base              Limit

Base  0x0
Limit 0x0FFFFF
Flags 0xC
  - 1 - 4 KiB blocks (page granularity)
  - 1 - 32-bit protected mode segment
  - 0 - Set when descriptor defines 64-bit code segment (if set, implies D/B _should_ be clear)
  - 0 - Reserved
Access
  - 1 - Present
  - 00 - Ring 0
  - 1 - data/code segment
  - 1 - executable
  - 0 - "only executable by Ring 0"
  - 1 - Read access allowed
  - 0 - "Accessed" bit; used by CPU

DATA - 0x00CF9200_0000FFFF
0x00     CF          92       00       | 00       00       FF       FF
============================================================================
00000000 1100___1111 10010010 00000000 | 00000000_00000000 11111111_11111111
Base     Flags Limit Access   Base     | Base              Limit

Base  0x0
Limit 0x0FFFFF
Flags 0xC
  - 1 - 4 KiB blocks (page granularity)
  - 1 - 32-bit protected mode segment
  - 0 - Set when descriptor defines 64-bit code segment (if set, implies D/B _should_ be clear)
  - 0 - Reserved
Access
  - 1 - Present
  - 00 - Ring 0
  - 1 - data/code segment
  - 0 - non-executable; defines data segment
  - 0 - segment grows upwards
  - 1 - Write access allowed
  - 0 - "Accessed" bit; used by CPU

static void load_and_flush_gdt() {
    asm volatile(
        "lgdt (%0)\n"           // Give the CPU the address of the GDT pointer
        "mov $0x10, %%ax\n"     // The Data segment descriptor is 16 bytes (0x10) from the GDT pointer
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        "ljmp $0x08, $.flush\n" // The Code segment descriptor is 8 bytes (0x08) from the GDT pointer; and can only be changed by a `ljmp` (not a mov)
        ".flush:\n"
        :
        : "r"(&s_gdtr)
        : "ax", "memory"
    );
}

*/

// NULL, Code, and Segment in Ring 0 for now...
static DescriptorPointer s_gdtr;
static Descriptor        s_gdt_descriptors[3];

static void create_descriptor(const size_t index, const unsigned int low, const unsigned int high) {
    s_gdt_descriptors[index].low  = low;
    s_gdt_descriptors[index].high = high;
}
static void create_null_descriptor() {
    create_descriptor(0, 0, 0);
}
static void create_code_descriptor() {
    create_descriptor(1, 0x0000ffff, 0x00CF9A00);
}
static void create_data_descriptor() {
    create_descriptor(2, 0x0000ffff, 0x00CF9200);
}

static void set_global_descriptor_table_register_values() {
    s_gdtr.limit   = sizeof(s_gdt_descriptors) - 1;
    s_gdtr.address = static_cast<void *>(s_gdt_descriptors);
}

static void load_gdt() {
    asm volatile(
        "lgdt (%0)\n" // Give the CPU the address of the GDT pointer
        :
        : "r"(&s_gdtr)
        : "memory");
}

static void load_data_segment_descriptors() {
    asm volatile(
        "mov $0x10, %%ax\n" // The Data segment descriptor is 16 bytes (0x10) from the GDT pointer
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        :
        :
        : "ax", "memory");
}

static void load_code_segment_descriptors() {
    asm volatile(
        "ljmp $0x08, $1f\n" // The Code segment descriptor is 8 bytes (0x08) from the GDT pointer; and can only be changed by a `ljmp` (not a mov)
        "1:\n"
        :
        :
        : "memory");
}

static void load_and_flush_gdt() {
    load_gdt();
    load_data_segment_descriptors();
    load_code_segment_descriptors();
}

void gdt_init() {
    create_null_descriptor();
    create_code_descriptor();
    create_data_descriptor();

    set_global_descriptor_table_register_values();

    load_and_flush_gdt();
}
