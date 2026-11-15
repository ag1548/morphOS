/*

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

#include "IDT.h"

#include "CPU.h"
#include "IO.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "PIC.h"
#include "types.h"

extern "C" void printf(const char *format, ...);
void            debug_out(const char *format, ...);

namespace IDT {

static DescriptorPointer s_idtr;
static Descriptor        s_idt_descriptors[256];

static void set_interrupt_descriptor_table_register_values() {
    s_idtr.size    = sizeof(s_idt_descriptors) - 1;
    s_idtr.address = static_cast<void *>(s_idt_descriptors);
}

static void unimplemented_interrupt_handler() {
    printf("Unhandled IRQ.\n");
    debug_out("Unhandled IRQ.\n");
    CPU::hang();
}

static void unimplemented_interrupt_handler_test() {
    printf("Unhandled IRQ TEST\n");
    debug_out("Unhandled IRQ TEST\n");
    CPU::hang();
}

void assert_gate_type(const unsigned int type_) {
    const bool is_32bit_interrupt_gate = type_ == 0xE;
    const bool is_32bit_trap_gate      = type_ == 0xF;
    if (!is_32bit_interrupt_gate && !is_32bit_trap_gate) {
        printf("Unknown gate type!\n");
        debug_out("Unknown gate type!\n");
        CPU::hang();
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
    install_interrupt_descriptor(0xa0, low, high);
    // install_interrupt_descriptor(0xa1, low, high);
    // install_interrupt_descriptor(0xa2, low, high);
    // install_interrupt_descriptor(0xa3, low, high);
    // install_interrupt_descriptor(0xa4, low, high);
    // install_interrupt_descriptor(0xa5, low, high);
    // install_interrupt_descriptor(0xa6, low, high);
    // install_interrupt_descriptor(0xa7, low, high);
    // install_interrupt_descriptor(0xa8, low, high);
    // install_interrupt_descriptor(0xa9, low, high);
    // install_interrupt_descriptor(0xaa, low, high);
    // install_interrupt_descriptor(0xab, low, high);
    // install_interrupt_descriptor(0xac, low, high);
    // install_interrupt_descriptor(0xad, low, high);
    // install_interrupt_descriptor(0xae, low, high);
    // install_interrupt_descriptor(0xaf, low, high);
}

extern "C" void _exception0_divide_error_entry();
asm(
    ".global _exception0_divide_error_entry\n"
    "_exception0_divide_error_entry:\n\t"
    "call _exception0_divide_error\n\t"
    "iret\n\t");
extern "C" void _exception0_divide_error() {
    printf("INT_0x0: Divide Error\n");
    debug_out("INT_0x0: Divide Error\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exception1_debug_exception_entry();
asm(
    ".global _exception1_debug_exception_entry\n"
    "_exception1_debug_exception_entry:\n\t"
    "call _exception1_debug_exception\n\t"
    "iret\n\t");
extern "C" void _exception1_debug_exception() {
    printf("INT_0x1: Debug Exception\n");
    debug_out("INT_0x1: Debug Exception\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exception2_nmi_interrupt_entry();
asm(
    ".global _exception2_nmi_interrupt_entry\n"
    "_exception2_nmi_interrupt_entry:\n\t"
    "call _exception2_nmi_interrupt\n\t"
    "iret\n\t");
extern "C" void _exception2_nmi_interrupt() {
    printf("INT_0x2: NMI Interrupt\n");
    debug_out("INT_0x2: NMI Interrupt\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exception3_breakpoint_entry();
asm(
    ".global _exception3_breakpoint_entry\n"
    "_exception3_breakpoint_entry:\n\t"
    "call _exception3_breakpoint\n\t"
    "iret\n\t");
extern "C" void _exception3_breakpoint() {
    printf("INT_0x3: Breakpoint\n");
    debug_out("INT_0x3: Breakpoint\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exception4_overflow_entry();
asm(
    ".global _exception4_overflow_entry\n"
    "_exception4_overflow_entry:\n\t"
    "call _exception4_overflow\n\t"
    "iret\n\t");
extern "C" void _exception4_overflow() {
    printf("INT_0x4: Overflow\n");
    debug_out("INT_0x4: Overflow\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exception5_bound_range_exceeded_entry();
asm(
    ".global _exception5_bound_range_exceeded_entry\n"
    "_exception5_bound_range_exceeded_entry:\n\t"
    "call _exception5_bound_range_exceeded\n\t"
    "iret\n\t");
extern "C" void _exception5_bound_range_exceeded() {
    printf("INT_0x5: Bound range exceeded\n");
    debug_out("INT_0x5: Bound range exceeded\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exception6_invalid_opcode_entry();
asm(
    ".global _exception6_invalid_opcode_entry\n"
    "_exception6_invalid_opcode_entry:\n\t"
    "call _exception6_invalid_opcode\n\t"
    "iret\n\t");
extern "C" void _exception6_invalid_opcode() {
    printf("INT_0x6: Invalid opcode\n");
    debug_out("INT_0x6: Invalid opcode\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exception7_device_not_available_entry();
asm(
    ".global _exception7_device_not_available_entry\n"
    "_exception7_device_not_available_entry:\n\t"
    "call _exception7_device_not_available\n\t"
    "iret\n\t");
extern "C" void _exception7_device_not_available() {
    printf("INT_0x7: Device not available\n");
    debug_out("INT_0x7: Device not available\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exception8_double_fault_entry();
asm(
    ".global _exception8_double_fault_entry\n"
    "_exception8_double_fault_entry:\n\t"
    "call _exception8_double_fault\n\t"
    "iret\n\t");
extern "C" void _exception8_double_fault() {
    printf("INT_0x8: Double fault\n");
    debug_out("INT_0x8: Double fault\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exception9_coprocessor_segment_overrun_entry();
asm(
    ".global _exception9_coprocessor_segment_overrun_entry\n"
    "_exception9_coprocessor_segment_overrun_entry:\n\t"
    "call _exception9_coprocessor_segment_overrun\n\t"
    "iret\n\t");
extern "C" void _exception9_coprocessor_segment_overrun() {
    printf("INT_0x9: Coprocessor segment overrun\n");
    debug_out("INT_0x9: Coprocessor segment overrun\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exceptionA_invalid_tss_entry();
asm(
    ".global _exceptionA_invalid_tss_entry\n"
    "_exceptionA_invalid_tss_entry:\n\t"
    "call _exceptionA_invalid_tss\n\t"
    "iret\n\t");
extern "C" void _exceptionA_invalid_tss() {
    printf("INT_0xA: Invalid TSS\n");
    debug_out("INT_0xA: Invalid TSS\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exceptionB_segment_not_present_entry();
asm(
    ".global _exceptionB_segment_not_present_entry\n"
    "_exceptionB_segment_not_present_entry:\n\t"
    "call _exceptionB_segment_not_present\n\t"
    "iret\n\t");
extern "C" void _exceptionB_segment_not_present() {
    printf("INT_0xB: Segment not present\n");
    debug_out("INT_0xB: Segment not present\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exceptionC_stack_segment_fault_entry();
asm(
    ".global _exceptionC_stack_segment_fault_entry\n"
    "_exceptionC_stack_segment_fault_entry:\n\t"
    "call _exceptionC_stack_segment_fault\n\t"
    "iret\n\t");
extern "C" void _exceptionC_stack_segment_fault() {
    printf("INT_0xC: Stack segment fault\n");
    debug_out("INT_0xC: Stack segment fault\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exceptionD_general_protection_entry();
asm(
    ".global _exceptionD_general_protection_entry\n"
    "_exceptionD_general_protection_entry:\n\t"
    "call _exceptionD_general_protection\n\t"
    "iret\n\t");
extern "C" void _exceptionD_general_protection() {
    printf("INT_0xD: General protection (exception)\n");
    debug_out("INT_0xD: General protection (exception)\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exceptionE_page_fault_entry();
asm(
    ".global _exceptionE_page_fault_entry\n"
    "_exceptionE_page_fault_entry:\n\t"
    "call _exceptionE_page_fault\n\t"
    "iret\n\t");
extern "C" void _exceptionE_page_fault() {
    printf("INT_0xE: Page fault\n");
    debug_out("INT_0xE: Page fault\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exceptionF_intel_reserved_entry();
asm(
    ".global _exceptionF_intel_reserved_entry\n"
    "_exceptionF_intel_reserved_entry:\n\t"
    "call _exceptionF_intel_reserved\n\t"
    "iret\n\t");
extern "C" void _exceptionF_intel_reserved() {
    printf("INT_0xF: Intel reserved exception\n");
    debug_out("INT_0xF: Intel reserved exception\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exception10_x87_fpu_floating_point_error_entry();
asm(
    ".global _exception10_x87_fpu_floating_point_error_entry\n"
    "_exception10_x87_fpu_floating_point_error_entry:\n\t"
    "call _exception10_x87_fpu_floating_point_error\n\t"
    "iret\n\t");
extern "C" void _exception10_x87_fpu_floating_point_error() {
    printf("INT_0x10: FPU Floating Point Error\n");
    debug_out("INT_0x10: FPU Floating Point Error\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exception11_alignment_check_entry();
asm(
    ".global _exception11_alignment_check_entry\n"
    "_exception11_alignment_check_entry:\n\t"
    "call _exception11_alignment_check\n\t"
    "iret\n\t");
extern "C" void _exception11_alignment_check() {
    printf("INT_0x11: Align check (exception)\n");
    debug_out("INT_0x11: Align check (exception)\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exception12_machine_check_entry();
asm(
    ".global _exception12_machine_check_entry\n"
    "_exception12_machine_check_entry:\n\t"
    "call _exception12_machine_check\n\t"
    "iret\n\t");
extern "C" void _exception12_machine_check() {
    printf("INT_0x12: Machine check (exception)\n");
    debug_out("INT_0x12: Machine check (exception)\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exception13_simd_floating_point_exception_entry();
asm(
    ".global _exception13_simd_floating_point_exception_entry\n"
    "_exception13_simd_floating_point_exception_entry:\n\t"
    "call _exception13_simd_floating_point_exception\n\t"
    "iret\n\t");
extern "C" void _exception13_simd_floating_point_exception() {
    printf("INT_0x13: SIMD floating-point exception\n");
    debug_out("INT_0x13: SIMD floating-point exception\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exception14_virtualization_exception_entry();
asm(
    ".global _exception14_virtualization_exception_entry\n"
    "_exception14_virtualization_exception_entry:\n\t"
    "call _exception14_virtualization_exception\n\t"
    "iret\n\t");
extern "C" void _exception14_virtualization_exception() {
    printf("INT_0x14: Virtualization exception\n");
    debug_out("INT_0x14: Virtualization exception\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exception15_control_protection_exception_entry();
asm(
    ".global _exception15_control_protection_exception_entry\n"
    "_exception15_control_protection_exception_entry:\n\t"
    "call _exception15_control_protection_exception\n\t"
    "iret\n\t");
extern "C" void _exception15_control_protection_exception() {
    printf("INT_0x14: Control protection exception\n");
    debug_out("INT_0x14: Control protection exception\n");
    CPU::print_control_registers_and_hang();
}

extern "C" void _exception20_timer_tick_entry();
asm(
    ".global _exception20_timer_tick_entry\n"
    "_exception20_timer_tick_entry:\n\t"
    "call _exception20_timer_tick\n\t"
    "iret\n\t");
static unsigned int value = 0;
extern "C" void     _exception20_timer_tick() {
    value += 1;
    if ((value % 1000) == 999) {
        // printf("INT_0x20: PIT - Timer tick %x\n", value);
        // debug_out("INT_0x20: PIT - Timer tick %x\n", value);
        printf("INT_0x20: PIT - Timer tick\n");
        debug_out("INT_0x20: PIT - Timer tick\n");
        value = 0;
    }
    // CPU::print_control_registers_and_hang();
    PIC::send_end_of_interrupt(PIC_TIMER_TICK);
}

extern "C" void _exception21_keyboard_entry();
asm(
    ".global _exception21_keyboard_entry\n"
    "_exception21_keyboard_entry:\n\t"
    "call _exception21_keyboard\n\t"
    "iret\n\t");
extern "C" void _exception21_keyboard() {
    Keyboard::handle_interrupt();
    PIC::send_end_of_interrupt(PIC_KEYBOARD);
}

extern "C" void _exception32_mouse_entry();
asm(
    ".global _exception32_mouse_entry\n"
    "_exception32_mouse_entry:\n\t"
    "call _exception32_mouse\n\t"
    "iret\n\t");
extern "C" void _exception32_mouse() {
    Mouse::handle_interrupt();
    PIC::send_end_of_interrupt(PIC_MOUSE);
}

extern "C" void _exception27_spurious_entry();
asm(
    ".global _exception27_spurious_entry\n"
    "_exception27_spurious_entry:\n\t"
    "call _exception27_spurious\n\t"
    "iret\n\t");
extern "C" void _exception27_spurious() {
    // FIXME: Check ISR to determine if real.
    printf("INT_0x27: SPURIOUS?\n");
    debug_out("INT_0x27: SPURIOUS?\n");
    CPU::print_control_registers_and_hang();
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
    install_descriptor(reinterpret_cast<unsigned int>(&_exception0_divide_error_entry), 0x00);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception1_debug_exception_entry), 0x01);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception2_nmi_interrupt_entry), 0x02);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception3_breakpoint_entry), 0x03);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception4_overflow_entry), 0x04);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception5_bound_range_exceeded_entry), 0x05);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception6_invalid_opcode_entry), 0x06);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception7_device_not_available_entry), 0x07);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception8_double_fault_entry), 0x08);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception9_coprocessor_segment_overrun_entry), 0x09);
    install_descriptor(reinterpret_cast<unsigned int>(&_exceptionA_invalid_tss_entry), 0x0A);
    install_descriptor(reinterpret_cast<unsigned int>(&_exceptionB_segment_not_present_entry), 0x0B);
    install_descriptor(reinterpret_cast<unsigned int>(&_exceptionC_stack_segment_fault_entry), 0x0C);
    install_descriptor(reinterpret_cast<unsigned int>(&_exceptionD_general_protection_entry), 0x0D);
    install_descriptor(reinterpret_cast<unsigned int>(&_exceptionE_page_fault_entry), 0x0E);
    install_descriptor(reinterpret_cast<unsigned int>(&_exceptionF_intel_reserved_entry), 0x0F);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception10_x87_fpu_floating_point_error_entry), 0x10);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception11_alignment_check_entry), 0x11);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception12_machine_check_entry), 0x12);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception13_simd_floating_point_exception_entry), 0x13);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception14_virtualization_exception_entry), 0x14);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception15_control_protection_exception_entry), 0x15);

    // PIC - PIC IRQs 0..0xF -> INT 0x20..0x2F
    install_descriptor(reinterpret_cast<unsigned int>(&_exception20_timer_tick_entry), IRQ_TIMER_TICK);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception21_keyboard_entry), IRQ_KEYBOARD);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception27_spurious_entry), IRQ_PRL1_OR_SPURIOUS);
    install_descriptor(reinterpret_cast<unsigned int>(&_exception32_mouse_entry), IRQ_MOUSE);
}

static void load_idt() {
    asm volatile(
        "lidt (%0)\n" // Give the CPU the address of the IDT pointer
        :
        : "r"(&s_idtr)
        : "memory");
}

void init() {

    debug_out("IDT loading...!\n");

    set_interrupt_descriptor_table_register_values();

    initialize_all_interrupts_to_null();
    // install_test_handler();
    install_interrupts();

    load_idt();

    debug_out("IDT loaded!\n");
}

} // namespace IDT
