#include "EFLAGS.h"

extern "C" void printf(const char *format, ...);
void            debug_out(const char *const str);


void print_EFLAGSRegister_dump(EFLAGSRegisterDump dump) {

    // printf("======\n");
    // printf("EFLAGS\n");
    // printf("======\n");
    debug_out("======\n");
    debug_out("EFLAGS\n");
    debug_out("======\n");

    if (dump.CF_carry_flag) {
        // printf("CF_carry_flag is set\n");
        debug_out("CF_carry_flag is set\n");
    }
    if (dump.PF_parity_flag) {
        // printf("PF_parity_flag is set\n");
        debug_out("PF_parity_flag is set\n");
    }
    if (dump.AF_aux_carry_flag) {
        // printf("AF_aux_carry_flag is set\n");
        debug_out("AF_aux_carry_flag is set\n");
    }
    if (dump.ZF_zero_flag) {
        // printf("ZF_zero_flag is set\n");
        debug_out("ZF_zero_flag is set\n");
    }
    if (dump.SF_sign_flag) {
        // printf("SF_sign_flag is set\n");
        debug_out("SF_sign_flag is set\n");
    }
    if (dump.TF_trap_flag) {
        // printf("TF_trap_flag is set\n");
        debug_out("TF_trap_flag is set\n");
    }
    if (dump.IF_interrupt_enable_flag) {
        // printf("IF_interrupt_enable_flag is set\n");
        debug_out("IF_interrupt_enable_flag is set\n");
    }
    if (dump.DF_direction_flag) {
        // printf("DF_direction_flag is set\n");
        debug_out("DF_direction_flag is set\n");
    }
    if (dump.OF_overflow_flag) {
        // printf("OF_overflow_flag is set\n");
        debug_out("OF_overflow_flag is set\n");
    }
    if (dump.IOPL_io_privilege_level) {
        // printf("IOPL_io_privilege_level is set\n");
        debug_out("IOPL_io_privilege_level is set\n");
    }
    if (dump.NT_nested_task) {
        // printf("NT_nested_task is set\n");
        debug_out("NT_nested_task is set\n");
    }
    if (dump.RF_resume_flag) {
        // printf("RF_resume_flag is set\n");
        debug_out("RF_resume_flag is set\n");
    }
    if (dump.VM_virtual_8086_mode) {
        // printf("VM_virtual_8086_mode is set\n");
        debug_out("VM_virtual_8086_mode is set\n");
    }
    if (dump.AC_alignment_check_access_control) {
        // printf("AC_alignment_check_access_control is set\n");
        debug_out("AC_alignment_check_access_control is set\n");
    }
    if (dump.VIF_virtual_interrupt_flag) {
        // printf("VIF_virtual_interrupt_flag is set\n");
        debug_out("VIF_virtual_interrupt_flag is set\n");
    }
    if (dump.VIP_virtual_interrupt_pending) {
        // printf("VIP_virtual_interrupt_pending is set\n");
        debug_out("VIP_virtual_interrupt_pending is set\n");
    }
    if (dump.ID_id_flag) {
        // printf("ID_id_flag is set\n");
        debug_out("ID_id_flag is set\n");
    }

    // printf("\n");
}
