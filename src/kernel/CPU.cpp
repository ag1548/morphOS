#include "CPU.h"

#include "ControlRegisters.h"
#include "PIC.h"
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

===
GDT
===

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

===
IDT
===

"Gate" (Interrupt) Descriptor
=============================
Offset - the address of the entry point of the Interrupt Service Routine
P - Present bit (must be set to 1)
DPL - Descriptor Privilege Level (see Rings above)
Type - A 4-bit value which defines the type of gate this Interrupt Descriptor represents.
    0b0101 or 0x5: Task Gate, note that in this case, the Offset value is unused and should be set to zero.
    0b0110 or 0x6: 16-bit Interrupt Gate
    0b0111 or 0x7: 16-bit Trap Gate
    0b1110 or 0xE: 32-bit Interrupt Gate
    0b1111 or 0xF: 32-bit Trap Gate
Selector - A Segment Selector with multiple fields which must point to a valid code segment in your GDT.

 2                           .5           1             2                    2
┌────────────┬──┬────────┬──┬────────────┬─────────────┬────────────────────┬────────────────────┐
│63        48│47│46    45│44│43        40│39         32│31                16│15                 0│
│    Offset  │P │   DPL  │  │   Type     │ Reserved    │       Selector     │      Offset        │
│31        16│  │1      0│  │3          0│             │15                 0│15                 0│
└────────────┴──┴────────┴──┴────────────┴─────────────┴────────────────────┴────────────────────┘
 16           1  2        1  4            8             16                   16

                     DPL
  Offset 16-31 ---┐ P|| Type Reserved Selector--------┐ Offset 0-16 ----┐
  |               | ||| |  | |      | |               | |               |
  v               v vvv V  v V      v v               v v               v
0b00000000_00000000_000x0000_xxxxxxxx_00000000_00000000_00000000_00000000

  High
  |           Low
  v           v
0x00_00_00_00_00_00_00_00

*/

extern "C" void printf(const char *format, ...);
void            debug_out(const char *format, ...);

static_assert(sizeof(unsigned long long) == 8);

// GDT
// NULL, Code, and Segment in Ring 0 for now...
static DescriptorPointer s_gdtr;
static Descriptor        s_gdt_descriptors[3];

// IDT
static DescriptorPointer s_idtr;
static Descriptor        s_idt_descriptors[256];

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

void gdt_init() {
    set_global_descriptor_table_register_values();

    create_null_descriptor();
    create_code_descriptor();
    create_data_descriptor();

    load_and_flush_gdt();
}

static void set_interrupt_descriptor_table_register_values() {
    s_idtr.size    = sizeof(s_idt_descriptors) - 1;
    s_idtr.address = static_cast<void *>(s_idt_descriptors);
}

[[noreturn]] void hang() {
    asm volatile("cli" ::: "memory");
    for (;;) {
        asm volatile("hlt" ::: "memory");
    }
}

static void unimplemented_interrupt_handler() {
    printf("Unhandled IRQ.\n");
    debug_out("Unhandled IRQ.\n");
    hang();
}

static void unimplemented_interrupt_handler_test() {
    printf("Unhandled IRQ TEST\n");
    debug_out("Unhandled IRQ TEST\n");
    hang();
}

void assert_gate_type(const unsigned int type_) {
    const bool is_32bit_interrupt_gate = type_ == 0xE;
    const bool is_32bit_trap_gate      = type_ == 0xF;
    if (!is_32bit_interrupt_gate && !is_32bit_trap_gate) {
        printf("Unknown gate type!\n");
        debug_out("Unknown gate type!\n");
        hang();
    }
}

void construct_idt_descriptor(unsigned int &low, unsigned int &high, const unsigned int type_, const unsigned int f) {

    assert_gate_type(type_);

    unsigned long long result = 0ULL;

    unsigned long long present_bit  = 1ULL << 47;
    result                         |= present_bit;

    // DPL will remain 00 for now...

    unsigned long long type  = static_cast<unsigned long long>(type_ & 0xF) << 40;
    result                  |= type;

    // GDT offset of 8 for the code segment
    unsigned long long selector  = 0x00080000ULL;
    result                      |= selector;

    unsigned long long offset   = f & 0xFFFF0000; // Top 2 bytes
    offset                    <<= 32;             // Top 2 bytes
    offset                     |= f & 0xFFFF;     // Bottom 2 bytes
    result                     |= offset;

    low  = result & 0xFFFFFFFF;
    high = (result >> 32) & 0xFFFFFFFF;
}

static void install_interrupt_descriptor(const size_t index, const unsigned int low, const unsigned int high) {
    s_idt_descriptors[index].low  = low;
    s_idt_descriptors[index].high = high;
}

static void initialize_all_interrupts_to_null() {
    unsigned int       low                       = 0;
    unsigned int       high                      = 0;
    const unsigned int type_32bit_interrupt_gate = 0xE;
    const unsigned int f                         = reinterpret_cast<unsigned int>(&unimplemented_interrupt_handler);
    construct_idt_descriptor(low, high, type_32bit_interrupt_gate, f);
    for (unsigned int i = 0; i < 256; i++) {
        install_interrupt_descriptor(i, low, high);
    }
}

[[maybe_unused]] static void install_test_handler() {
    unsigned int       low                       = 0;
    unsigned int       high                      = 0;
    const unsigned int type_32bit_interrupt_gate = 0xE;
    const unsigned int f                         = reinterpret_cast<unsigned int>(&unimplemented_interrupt_handler_test);
    construct_idt_descriptor(low, high, type_32bit_interrupt_gate, f);
    install_interrupt_descriptor(0x20, low, high);
}

[[noreturn]] static void print_control_registers_and_hang() {

    unsigned int cr0;
    unsigned int cr2;
    unsigned int cr3;
    unsigned int cr4;

    asm volatile("mov %%cr0, %%eax" : "=a"(cr0));
    asm volatile("mov %%cr2, %%eax" : "=a"(cr2));
    asm volatile("mov %%cr3, %%eax" : "=a"(cr3));
    asm volatile("mov %%cr4, %%eax" : "=a"(cr4));

    printf("CR0=%x CR2=%x CR3=%x CR4=%x\n", cr0, cr2, cr3, cr4);
    printf("\n");
    print_CR0_dump(ControlRegister0Dump(ControlRegister0(cr0)));
    print_CR2_dump(ControlRegister2(cr2));
    print_CR3_dump(ControlRegister3Dump(ControlRegister3(cr3)));
    print_CR4_dump(ControlRegister4Dump(ControlRegister4(cr4)));

    hang();
}

static void _exception0_divide_error() {
    printf("INT_0x0: Divide Error\n");
    debug_out("INT_0x0: Divide Error\n");
    print_control_registers_and_hang();
}

static void _exception1_debug_exception() {
    printf("INT_0x1: Debug Exception\n");
    debug_out("INT_0x1: Debug Exception\n");
    print_control_registers_and_hang();
}

static void _exception2_nmi_interrupt() {
    printf("INT_0x2: NMI Interrupt\n");
    debug_out("INT_0x2: NMI Interrupt\n");
    print_control_registers_and_hang();
}

static void _exception3_breakpoint() {
    printf("INT_0x3: Breakpoint\n");
    debug_out("INT_0x3: Breakpoint\n");
    print_control_registers_and_hang();
}

static void _exception4_overflow() {
    printf("INT_0x4: Overflow\n");
    debug_out("INT_0x4: Overflow\n");
    print_control_registers_and_hang();
}

static void _exception5_bound_range_exceeded() {
    printf("INT_0x5: Bound range exceeded\n");
    debug_out("INT_0x5: Bound range exceeded\n");
    print_control_registers_and_hang();
}

static void _exception6_invalid_opcode() {
    printf("INT_0x6: Invalid opcode\n");
    debug_out("INT_0x6: Invalid opcode\n");
    print_control_registers_and_hang();
}

static void _exception7_device_not_available() {
    printf("INT_0x7: Device not available\n");
    debug_out("INT_0x7: Device not available\n");
    print_control_registers_and_hang();
}

static void _exception8_double_fault() {
    printf("INT_0x8: Double fault\n");
    debug_out("INT_0x8: Double fault\n");
    print_control_registers_and_hang();
}

static void _exception9_coprocessor_segment_overrun() {
    printf("INT_0x9: Coprocessor segment overrun\n");
    debug_out("INT_0x9: Coprocessor segment overrun\n");
    print_control_registers_and_hang();
}

static void _exceptionA_invalid_tss() {
    printf("INT_0xA: Invalid TSS\n");
    debug_out("INT_0xA: Invalid TSS\n");
    print_control_registers_and_hang();
}

static void _exceptionB_segment_not_present() {
    printf("INT_0xB: Segment not present\n");
    debug_out("INT_0xB: Segment not present\n");
    print_control_registers_and_hang();
}

static void _exceptionC_stack_segment_fault() {
    printf("INT_0xC: Stack segment fault\n");
    debug_out("INT_0xC: Stack segment fault\n");
    print_control_registers_and_hang();
}

static void _exceptionD_general_protection() {
    printf("INT_0xD: General protection (exception)\n");
    debug_out("INT_0xD: General protection (exception)\n");
    print_control_registers_and_hang();
}

static void _exceptionE_page_fault() {
    printf("INT_0xE: Page fault\n");
    debug_out("INT_0xE: Page fault\n");
    print_control_registers_and_hang();
}

static void _exceptionF_intel_reserved() {
    printf("INT_0xF: Intel reserved exception\n");
    debug_out("INT_0xF: Intel reserved exception\n");
    print_control_registers_and_hang();
}

static void _exception10_x87_fpu_floating_point_error() {
    printf("INT_0x10: FPU Floating Point Error\n");
    debug_out("INT_0x10: FPU Floating Point Error\n");
    print_control_registers_and_hang();
}

static void _exception11_alignment_check() {
    printf("INT_0x11: Align check (exception)\n");
    debug_out("INT_0x11: Align check (exception)\n");
    print_control_registers_and_hang();
}

static void _exception12_machine_check() {
    printf("INT_0x12: Machine check (exception)\n");
    debug_out("INT_0x12: Machine check (exception)\n");
    print_control_registers_and_hang();
}

static void _exception13_simd_floating_point_exception() {
    printf("INT_0x13: SIMD floating-point exception\n");
    debug_out("INT_0x13: SIMD floating-point exception\n");
    print_control_registers_and_hang();
}

static void _exception14_virtualization_exception() {
    printf("INT_0x14: Virtualization exception\n");
    debug_out("INT_0x14: Virtualization exception\n");
    print_control_registers_and_hang();
}

static void _exception15_control_protection_exception() {
    printf("INT_0x14: Control protection exception\n");
    debug_out("INT_0x14: Control protection exception\n");
    print_control_registers_and_hang();
}

extern "C" void _handle_irq();
asm(
    ".global _handle_irq\n"
    "_handle_irq:\n\t"
    "call _exception20_timer_tick\n\t"
    "iret\n\t");

static unsigned int value = 0;
extern "C" void     _exception20_timer_tick() {
    value += 1;
    printf("INT_0x20: PIT - Timer tick %x\n", value);
    debug_out("INT_0x20: PIT - Timer tick %x\n", value);
    // print_control_registers_and_hang();
    PIC_sendEOI(PIC_TIMER_TICK);
}

static void _exception27_spurious() {
    // FIXME: Check ISR to determine if real.
    printf("INT_0x27: SPURIOUS?\n");
    debug_out("INT_0x27: SPURIOUS?\n");
    print_control_registers_and_hang();
}

/*

0x16
...    Reserved
0x1f

===

0x20
...    External Interrupts
0xff

*/

static void install_descriptor(const unsigned int f, const unsigned int index) {
    unsigned int       low                       = 0;
    unsigned int       high                      = 0;
    const unsigned int type_32bit_interrupt_gate = 0xE;
    construct_idt_descriptor(low, high, type_32bit_interrupt_gate, f);
    install_interrupt_descriptor(index, low, high);
}

static void install_interrupts() {

    // CPU
    install_descriptor(reinterpret_cast<unsigned int>(&_exception0_divide_error), 0x00);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception1_debug_exception), 0x01);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception2_nmi_interrupt), 0x02);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception3_breakpoint), 0x03);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception4_overflow), 0x04);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception5_bound_range_exceeded), 0x05);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception6_invalid_opcode), 0x06);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception7_device_not_available), 0x07);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception8_double_fault), 0x08);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception9_coprocessor_segment_overrun), 0x09);
    install_descriptor(reinterpret_cast<unsigned int>(&_exceptionA_invalid_tss), 0x0A);
    install_descriptor(reinterpret_cast<unsigned int>(&_exceptionB_segment_not_present), 0x0B);
    install_descriptor(reinterpret_cast<unsigned int>(&_exceptionC_stack_segment_fault), 0x0C);
    install_descriptor(reinterpret_cast<unsigned int>(&_exceptionD_general_protection), 0x0D);
    install_descriptor(reinterpret_cast<unsigned int>(&_exceptionE_page_fault), 0x0E);
    install_descriptor(reinterpret_cast<unsigned int>(&_exceptionF_intel_reserved), 0x0F);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception10_x87_fpu_floating_point_error), 0x10);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception11_alignment_check), 0x11);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception12_machine_check), 0x12);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception13_simd_floating_point_exception), 0x13);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception14_virtualization_exception), 0x14);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception15_control_protection_exception), 0x15);

    // PIC - PIC IRQs 0..0xF -> INT 0x20..0x2F
    install_descriptor(reinterpret_cast<unsigned int>(&_handle_irq), IRQ_TIMER_TICK);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception27_spurious), IRQ_PRL1_OR_SPURIOUS);
}

static void load_idt() {
    asm volatile(
        "lidt (%0)\n" // Give the CPU the address of the IDT pointer
        :
        : "r"(&s_idtr)
        : "memory");
}

void idt_init() {

    set_interrupt_descriptor_table_register_values();

    initialize_all_interrupts_to_null();
    // install_test_handler();
    install_interrupts();

    load_idt();
}
