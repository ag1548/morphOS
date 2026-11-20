#include "EFLAGS.h"

#define kprintf(...)            \
    do {                        \
        printf(__VA_ARGS__);    \
        debug_out(__VA_ARGS__); \
    } while (0)

extern "C" void printf(const char *format, ...);
void            debug_out(const char *format, ...);

void print_EFLAGSRegister_dump(EFLAGSRegisterDump dump) {

    kprintf("======\n");
    kprintf("EFLAGS\n");
    kprintf("======\n");

    if (dump.CF_carry_flag) {
        kprintf("CF_carry_flag is set\n");
    }
    if (dump.PF_parity_flag) {
        kprintf("PF_parity_flag is set\n");
    }
    if (dump.AF_aux_carry_flag) {
        kprintf("AF_aux_carry_flag is set\n");
    }
    if (dump.ZF_zero_flag) {
        kprintf("ZF_zero_flag is set\n");
    }
    if (dump.SF_sign_flag) {
        kprintf("SF_sign_flag is set\n");
    }
    if (dump.TF_trap_flag) {
        kprintf("TF_trap_flag is set\n");
    }
    if (dump.IF_interrupt_enable_flag) {
        kprintf("IF_interrupt_enable_flag is set\n");
    }
    if (dump.DF_direction_flag) {
        kprintf("DF_direction_flag is set\n");
    }
    if (dump.OF_overflow_flag) {
        kprintf("OF_overflow_flag is set\n");
    }
    if (dump.IOPL_io_privilege_level) {
        kprintf("IOPL_io_privilege_level is set\n");
    }
    if (dump.NT_nested_task) {
        kprintf("NT_nested_task is set\n");
    }
    if (dump.RF_resume_flag) {
        kprintf("RF_resume_flag is set\n");
    }
    if (dump.VM_virtual_8086_mode) {
        kprintf("VM_virtual_8086_mode is set\n");
    }
    if (dump.AC_alignment_check_access_control) {
        kprintf("AC_alignment_check_access_control is set\n");
    }
    if (dump.VIF_virtual_interrupt_flag) {
        kprintf("VIF_virtual_interrupt_flag is set\n");
    }
    if (dump.VIP_virtual_interrupt_pending) {
        kprintf("VIP_virtual_interrupt_pending is set\n");
    }
    if (dump.ID_id_flag) {
        kprintf("ID_id_flag is set\n");
    }

    kprintf("\n");
}
