#include "ControlRegisters.h"

// extern "C" void printf(const char *format, ...);
void debug_out(const char *const str);

void print_CR0_dump(ControlRegister0Dump dump) {

    // printf("===\n");
    // printf("CR0\n");
    // printf("===\n");
    debug_out("===\n");
    debug_out("CR0\n");
    debug_out("===\n");

    if (dump.PE_protection_enable) {
        // printf("PE_protection_enable is set\n");
        debug_out("PE_protection_enable is set\n");
    }
    if (dump.MP_monitor_coprocessor) {
        // printf("MP_monitor_coprocessor is set\n");
        debug_out("MP_monitor_coprocessor is set\n");
    }
    if (dump.EM_emulation) {
        // printf("EM_emulation is set\n");
        debug_out("EM_emulation is set\n");
    }
    if (dump.TS_task_switched) {
        // printf("TS_task_switched is set\n");
        debug_out("TS_task_switched is set\n");
    }
    if (dump.ET_extension_type) {
        // printf("ET_extension_type is set\n");
        debug_out("ET_extension_type is set\n");
    }
    if (dump.NE_numeric_error) {
        // printf("NE_numeric_error is set\n");
        debug_out("NE_numeric_error is set\n");
    }
    if (dump.WP_write_protect) {
        // printf("WP_write_protect is set\n");
        debug_out("WP_write_protect is set\n");
    }
    if (dump.AM_alignment_mask) {
        // printf("AM_alignment_mask is set\n");
        debug_out("AM_alignment_mask is set\n");
    }
    if (dump.NW_not_write_through) {
        // printf("NW_not_write_through is set\n");
        debug_out("NW_not_write_through is set\n");
    }
    if (dump.CD_cache_disable) {
        // printf("CD_cache_disable is set\n");
        debug_out("CD_cache_disable is set\n");
    }
    if (dump.PG_paging) {
        // printf("PG_paging is set\n");
        debug_out("PG_paging is set\n");
    }

    // printf("\n");
}

void print_CR2_dump(ControlRegister2 reg) {
    // printf("===============================\n");
    // printf("CR2 - Page Fault Linear Address\n");
    // printf("===============================\n");
    debug_out("===============================\n");
    debug_out("CR2 - Page Fault Linear Address\n");
    debug_out("===============================\n");

    // printf("%x\n", reg.value);
    // printf("\n");
}

void print_CR3_dump(ControlRegister3Dump dump) {

    // printf("===\n");
    // printf("CR3\n");
    // printf("===\n");
    debug_out("===\n");
    debug_out("CR3\n");
    debug_out("===\n");

    if (dump.PWT_page_level_write_through) {
        // printf("PWT_page_level_write_through is set\n");
        debug_out("PWT_page_level_write_through is set\n");
    }
    if (dump.PCD_page_level_cache_disable) {
        // printf("PCD_page_level_cache_disable is set\n");
        debug_out("PCD_page_level_cache_disable is set\n");
    }

    // printf("%x\n", dump.PDB_page_directory_base);
    // printf("\n");
}

void print_CR4_dump(ControlRegister4Dump dump) {

    // printf("===\n");
    // printf("CR4\n");
    // printf("===\n");
    debug_out("===\n");
    debug_out("CR4\n");
    debug_out("===\n");

    if (dump.VME_virtual_8086_mode_ext) {
        // printf("VME_virtual_8086_mode_ext is set\n");
        debug_out("VME_virtual_8086_mode_ext is set\n");
    }
    if (dump.PVI_protected_mode_virtual_interrupts) {
        // printf("PVI_protected_mode_virtual_interrupts is set\n");
        debug_out("PVI_protected_mode_virtual_interrupts is set\n");
    }
    if (dump.TSD_time_stamp_disable) {
        // printf("TSD_time_stamp_disable is set\n");
        debug_out("TSD_time_stamp_disable is set\n");
    }
    if (dump.DE_debugging_ext) {
        // printf("DE_debugging_ext is set\n");
        debug_out("DE_debugging_ext is set\n");
    }
    if (dump.PSE_page_size_ext) {
        // printf("PSE_page_size_ext is set\n");
        debug_out("PSE_page_size_ext is set\n");
    }
    if (dump.PAE_physical_address_ext) {
        // printf("PAE_physical_address_ext is set\n");
        debug_out("PAE_physical_address_ext is set\n");
    }
    if (dump.MCE_machine_check_enable) {
        // printf("MCE_machine_check_enable is set\n");
        debug_out("MCE_machine_check_enable is set\n");
    }
    if (dump.PGE_page_global_enable) {
        // printf("PGE_page_global_enable is set\n");
        debug_out("PGE_page_global_enable is set\n");
    }
    if (dump.PCE_performance_monitoring_counter_enable) {
        // printf("PCE_performance_monitoring_counter_enable is set\n");
        debug_out("PCE_performance_monitoring_counter_enable is set\n");
    }
    if (dump.OSFXSR_os_support_for_fxsave_fxstor) {
        // printf("OSFXSR_os_support_for_fxsave_fxstor is set\n");
        debug_out("OSFXSR_os_support_for_fxsave_fxstor is set\n");
    }
    if (dump.OSXMMEXCPT_os_support_for_unmasked_simd_fp_exc) {
        // printf("OSXMMEXCPT_os_support_for_unmasked_simd_fp_exc is set\n");
        debug_out("OSXMMEXCPT_os_support_for_unmasked_simd_fp_exc is set\n");
    }
    if (dump.UMIP_user_mode_instruction_prevention) {
        // printf("UMIP_user_mode_instruction_prevention is set\n");
        debug_out("UMIP_user_mode_instruction_prevention is set\n");
    }
    if (dump.LA57_57_bit_linear_addresses) {
        // printf("LA57_57_bit_linear_addresses is set\n");
        debug_out("LA57_57_bit_linear_addresses is set\n");
    }
    if (dump.VMXE_vmx_enable) {
        // printf("VMXE_vmx_enable is set\n");
        debug_out("VMXE_vmx_enable is set\n");
    }
    if (dump.SMXE_smx_enable) {
        // printf("SMXE_smx_enable is set\n");
        debug_out("SMXE_smx_enable is set\n");
    }
    if (dump.FSGSBASE_fsgsbase_enable) {
        // printf("FSGSBASE_fsgsbase_enable is set\n");
        debug_out("FSGSBASE_fsgsbase_enable is set\n");
    }
    if (dump.OSXSAVE_xsave_and_proc_extended_states_enable) {
        // printf("OSXSAVE_xsave_and_proc_extended_states_enable is set\n");
        debug_out("OSXSAVE_xsave_and_proc_extended_states_enable is set\n");
    }
    if (dump.KL_key_locker_enable) {
        // printf("KL_key_locker_enable is set\n");
        debug_out("KL_key_locker_enable is set\n");
    }
    if (dump.SMEP_supervisor_mode_execution_enable) {
        // printf("SMEP_supervisor_mode_execution_enable is set\n");
        debug_out("SMEP_supervisor_mode_execution_enable is set\n");
    }
    if (dump.SMAP_supervisor_mode_access_prevention_enable) {
        // printf("SMAP_supervisor_mode_access_prevention_enable is set\n");
        debug_out("SMAP_supervisor_mode_access_prevention_enable is set\n");
    }
    if (dump.PKE_protection_key_enable) {
        // printf("PKE_protection_key_enable is set\n");
        debug_out("PKE_protection_key_enable is set\n");
    }
    if (dump.CET_control_flow_enforce_tech) {
        // printf("CET_control_flow_enforce_tech is set\n");
        debug_out("CET_control_flow_enforce_tech is set\n");
    }
    if (dump.PKS_protection_key_supervisor_mode_enable) {
        // printf("PKS_protection_key_supervisor_mode_enable is set\n");
        debug_out("PKS_protection_key_supervisor_mode_enable is set\n");
    }
    if (dump.UINTR_user_interrupts_enable) {
        // printf("UINTR_user_interrupts_enable is set\n");
        debug_out("UINTR_user_interrupts_enable is set\n");
    }
    if (dump.LAM_SUP_supervisor_linear_address_masking_enable) {
        // printf("LAM_SUP_supervisor_linear_address_masking_enable is set\n");
        debug_out("LAM_SUP_supervisor_linear_address_masking_enable is set\n");
    }

    // printf("\n");
}
