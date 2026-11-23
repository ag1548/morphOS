/*

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

*/

#include "GDT.h"

#include "common.h"
#include "types.h"

namespace GDT {

// NULL, Code, and Segment in Ring 0 for now...
static DescriptorPointer s_gdtr;
static Descriptor        s_gdt_descriptors[3];

static void create_global_descriptor(const size_t index, const unsigned int low, const unsigned int high) {
    s_gdt_descriptors[index].low  = low;
    s_gdt_descriptors[index].high = high;
}
static void create_null_descriptor() {
    create_global_descriptor(0, 0, 0);
}
static void create_code_descriptor() {
    create_global_descriptor(1, 0x0000ffff, 0x00CF9A00);
}
static void create_data_descriptor() {
    create_global_descriptor(2, 0x0000ffff, 0x00CF9200);
}

static void set_global_descriptor_table_register_values() {
    s_gdtr.size    = sizeof(s_gdt_descriptors) - 1;
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

void init() {

    debug_out("GDT loading...!\n");

    set_global_descriptor_table_register_values();

    // FIXME: Create classes instead of dealing with raw hex values
    create_null_descriptor();
    create_code_descriptor();
    create_data_descriptor();

    load_and_flush_gdt();

    debug_out("GDT loaded!\n");
}

} // namespace GDT
