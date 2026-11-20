#include "ControlRegisters.h"

#define kprintf(...)            \
    do {                        \
        printf(__VA_ARGS__);    \
        debug_out(__VA_ARGS__); \
    } while (0)

extern "C" void printf(const char *format, ...);
void debug_out(const char *format, ...);

void print_CR0_dump(ControlRegister0Dump dump) {

    if (!dump.any()) {
        return;
    }

    kprintf("===\n");
    kprintf("CR0\n");
    kprintf("===\n");

    if (dump.PE_protection_enable) {
        kprintf("PE_protection_enable is set\n");
    }
    if (dump.MP_monitor_coprocessor) {
        kprintf("MP_monitor_coprocessor is set\n");
    }
    if (dump.EM_emulation) {
        kprintf("EM_emulation is set\n");
    }
    if (dump.TS_task_switched) {
        kprintf("TS_task_switched is set\n");
    }
    if (dump.ET_extension_type) {
        kprintf("ET_extension_type is set\n");
    }
    if (dump.NE_numeric_error) {
        kprintf("NE_numeric_error is set\n");
    }
    if (dump.WP_write_protect) {
        kprintf("WP_write_protect is set\n");
    }
    if (dump.AM_alignment_mask) {
        kprintf("AM_alignment_mask is set\n");
    }
    if (dump.NW_not_write_through) {
        kprintf("NW_not_write_through is set\n");
    }
    if (dump.CD_cache_disable) {
        kprintf("CD_cache_disable is set\n");
    }
    if (dump.PG_paging) {
        kprintf("PG_paging is set\n");
    }

    kprintf("\n");
}

void print_CR2_dump(ControlRegister2 reg) {

    if (!reg.value) {
        return;
    }

    kprintf("===============================\n");
    kprintf("CR2 - Page Fault Linear Address\n");
    kprintf("===============================\n");

    kprintf("%x\n", reg.value);
    kprintf("\n");
}

void print_CR3_dump(ControlRegister3Dump dump) {

    if (!dump.any()) {
        return;
    }

    kprintf("===\n");
    kprintf("CR3\n");
    kprintf("===\n");

    if (dump.PWT_page_level_write_through) {
        kprintf("PWT_page_level_write_through is set\n");
    }
    if (dump.PCD_page_level_cache_disable) {
        kprintf("PCD_page_level_cache_disable is set\n");
    }

    kprintf("%x\n", dump.PDB_page_directory_base);
    kprintf("\n");
}

void print_CR4_dump(ControlRegister4Dump dump) {

    if (!dump.any()) {
        return;
    }

    kprintf("===\n");
    kprintf("CR4\n");
    kprintf("===\n");

    if (dump.VME_virtual_8086_mode_ext) {
        kprintf("VME_virtual_8086_mode_ext is set\n");
    }
    if (dump.PVI_protected_mode_virtual_interrupts) {
        kprintf("PVI_protected_mode_virtual_interrupts is set\n");
    }
    if (dump.TSD_time_stamp_disable) {
        kprintf("TSD_time_stamp_disable is set\n");
    }
    if (dump.DE_debugging_ext) {
        kprintf("DE_debugging_ext is set\n");
    }
    if (dump.PSE_page_size_ext) {
        kprintf("PSE_page_size_ext is set\n");
    }
    if (dump.PAE_physical_address_ext) {
        kprintf("PAE_physical_address_ext is set\n");
    }
    if (dump.MCE_machine_check_enable) {
        kprintf("MCE_machine_check_enable is set\n");
    }
    if (dump.PGE_page_global_enable) {
        kprintf("PGE_page_global_enable is set\n");
    }
    if (dump.PCE_performance_monitoring_counter_enable) {
        kprintf("PCE_performance_monitoring_counter_enable is set\n");
    }
    if (dump.OSFXSR_os_support_for_fxsave_fxstor) {
        kprintf("OSFXSR_os_support_for_fxsave_fxstor is set\n");
    }
    if (dump.OSXMMEXCPT_os_support_for_unmasked_simd_fp_exc) {
        kprintf("OSXMMEXCPT_os_support_for_unmasked_simd_fp_exc is set\n");
    }
    if (dump.UMIP_user_mode_instruction_prevention) {
        kprintf("UMIP_user_mode_instruction_prevention is set\n");
    }
    if (dump.LA57_57_bit_linear_addresses) {
        kprintf("LA57_57_bit_linear_addresses is set\n");
    }
    if (dump.VMXE_vmx_enable) {
        kprintf("VMXE_vmx_enable is set\n");
    }
    if (dump.SMXE_smx_enable) {
        kprintf("SMXE_smx_enable is set\n");
    }
    if (dump.FSGSBASE_fsgsbase_enable) {
        kprintf("FSGSBASE_fsgsbase_enable is set\n");
    }
    if (dump.OSXSAVE_xsave_and_proc_extended_states_enable) {
        kprintf("OSXSAVE_xsave_and_proc_extended_states_enable is set\n");
    }
    if (dump.KL_key_locker_enable) {
        kprintf("KL_key_locker_enable is set\n");
    }
    if (dump.SMEP_supervisor_mode_execution_enable) {
        kprintf("SMEP_supervisor_mode_execution_enable is set\n");
    }
    if (dump.SMAP_supervisor_mode_access_prevention_enable) {
        kprintf("SMAP_supervisor_mode_access_prevention_enable is set\n");
    }
    if (dump.PKE_protection_key_enable) {
        kprintf("PKE_protection_key_enable is set\n");
    }
    if (dump.CET_control_flow_enforce_tech) {
        kprintf("CET_control_flow_enforce_tech is set\n");
    }
    if (dump.PKS_protection_key_supervisor_mode_enable) {
        kprintf("PKS_protection_key_supervisor_mode_enable is set\n");
    }
    if (dump.UINTR_user_interrupts_enable) {
        kprintf("UINTR_user_interrupts_enable is set\n");
    }
    if (dump.LAM_SUP_supervisor_linear_address_masking_enable) {
        kprintf("LAM_SUP_supervisor_linear_address_masking_enable is set\n");
    }

    kprintf("\n");
}
