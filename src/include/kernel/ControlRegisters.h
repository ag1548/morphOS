#pragma once

/*

=================
CONTROL REGISTERS
=================

Intel Manual 2-14 Vol. 3A

CR1, CR5, CR6, and CR7 are reserved. CR8 is only used in 64-bit mode.

*/

/*

CR1
===

Contains system control flags that control operating mode and states of the processor.

*/
#define PE_BIT 0b00000000000000000000000000000001
#define MP_BIT 0b00000000000000000000000000000010
#define EM_BIT 0b00000000000000000000000000000100
#define TS_BIT 0b00000000000000000000000000001000
#define ET_BIT 0b00000000000000000000000000010000
#define NE_BIT 0b00000000000000000000000000100000
#define WP_BIT 0b00000000000000010000000000000000
#define AM_BIT 0b00000000000001000000000000000000
#define NW_BIT 0b00100000000000000000000000000000
#define CD_BIT 0b01000000000000000000000000000000
#define PG_BIT 0b10000000000000000000000000000000
struct ControlRegister0 {
    unsigned int value;
    ControlRegister0(unsigned int value)
        : value(value) {}
};
typedef struct ControlRegister0 ControlRegister0;
struct ControlRegister0Dump {
    bool PE_protection_enable;
    bool MP_monitor_coprocessor;
    bool EM_emulation;
    bool TS_task_switched;
    bool ET_extension_type;
    bool NE_numeric_error;
    bool WP_write_protect;
    bool AM_alignment_mask;
    bool NW_not_write_through;
    bool CD_cache_disable;
    bool PG_paging;

    ControlRegister0Dump(ControlRegister0 reg)
        : PE_protection_enable(reg.value & PE_BIT)
        , MP_monitor_coprocessor(reg.value & MP_BIT)
        , EM_emulation(reg.value & EM_BIT)
        , TS_task_switched(reg.value & TS_BIT)
        , ET_extension_type(reg.value & ET_BIT)
        , NE_numeric_error(reg.value & NE_BIT)
        , WP_write_protect(reg.value & WP_BIT)
        , AM_alignment_mask(reg.value & AM_BIT)
        , NW_not_write_through(reg.value & NW_BIT)
        , CD_cache_disable(reg.value & CD_BIT)
        , PG_paging(reg.value & PG_BIT) {}
};

/*

CR2
===

Contains the page-fault linear address (the linear address that caused a page fault).

*/
struct ControlRegister2 {
    unsigned int value;
    ControlRegister2(unsigned int value)
        : value(value) {}
};
typedef struct ControlRegister2 ControlRegister2;

/*

CR3
===

Contains the physical address of the base of the paging-structure hierarchy and two flags (PCD and PWT).

*/
#define PWT_BIT  0b00000000000000000000000000001000
#define PCD_BIT  0b00000000000000000000000000010000
#define PDB_BITS 0b11111111111111111111000000000000
struct ControlRegister3 {
    unsigned int value;
    ControlRegister3(unsigned int value)
        : value(value) {}
};
typedef struct ControlRegister3 ControlRegister3;
struct ControlRegister3Dump {
    bool         PWT_page_level_write_through;
    bool         PCD_page_level_cache_disable;
    unsigned int PDB_page_directory_base;

    ControlRegister3Dump(ControlRegister3 reg)
        : PWT_page_level_write_through(reg.value & PWT_BIT)
        , PCD_page_level_cache_disable(reg.value & PCD_BIT)
        , PDB_page_directory_base((reg.value & PDB_BITS) >> 12) {}
};

/*

CR4
===

Contains a group of flags that enable several architectural extensions, and indicate operating system or executive support for specific processor capabilities.

*/
#define VME_BIT        0b00000000000000000000000000000001
#define PVI_BIT        0b00000000000000000000000000000010
#define TSD_BIT        0b00000000000000000000000000000100
#define DE_BIT         0b00000000000000000000000000001000
#define PSE_BIT        0b00000000000000000000000000010000
#define PAE_BIT        0b00000000000000000000000000100000
#define MCE_BIT        0b00000000000000000000000001000000
#define PGE_BIT        0b00000000000000000000000010000000
#define PCE_BIT        0b00000000000000000000000100000000
#define OSFXSR_BIT     0b00000000000000000000001000000000
#define OSXMMEXCPT_BIT 0b00000000000000000000010000000000
#define UMIP_BIT       0b00000000000000000000100000000000
#define LA57_BIT       0b00000000000000000001000000000000
#define VMXE_BIT       0b00000000000000000010000000000000
#define SMXE_BIT       0b00000000000000000100000000000000
#define FSGSBASE_BIT   0b00000000000000010000000000000000
#define OSXSAVE_BIT    0b00000000000000100000000000000000
#define KL_BIT         0b00000000000001000000000000000000
#define SMEP_BIT       0b00000000000010000000000000000000
#define SMAP_BIT       0b00000000000100000000000000000000
#define PKE_BIT        0b00000000001000000000000000000000
#define CET_BIT        0b00000000010000000000000000000000
#define PKS_BIT        0b00000000100000000000000000000000
#define UINTR_BIT      0b00000001000000000000000000000000
#define LAM_SUP_BIT    0b00001000000000000000000000000000
struct ControlRegister4 {
    unsigned int value;
    ControlRegister4(unsigned int value)
        : value(value) {}
};
typedef struct ControlRegister4 ControlRegister4;
struct ControlRegister4Dump {
    bool VME_virtual_8086_mode_ext;
    bool PVI_protected_mode_virtual_interrupts;
    bool TSD_time_stamp_disable;
    bool DE_debugging_ext;
    bool PSE_page_size_ext;
    bool PAE_physical_address_ext;
    bool MCE_machine_check_enable;
    bool PGE_page_global_enable;
    bool PCE_performance_monitoring_counter_enable;
    bool OSFXSR_os_support_for_fxsave_fxstor;
    bool OSXMMEXCPT_os_support_for_unmasked_simd_fp_exc;
    bool UMIP_user_mode_instruction_prevention;
    bool LA57_57_bit_linear_addresses;
    bool VMXE_vmx_enable;
    bool SMXE_smx_enable;
    bool FSGSBASE_fsgsbase_enable;
    bool OSXSAVE_xsave_and_proc_extended_states_enable;
    bool KL_key_locker_enable;
    bool SMEP_supervisor_mode_execution_enable;
    bool SMAP_supervisor_mode_access_prevention_enable;
    bool PKE_protection_key_enable;
    bool CET_control_flow_enforce_tech;
    bool PKS_protection_key_supervisor_mode_enable;
    bool UINTR_user_interrupts_enable;
    bool LAM_SUP_supervisor_linear_address_masking_enable;

    ControlRegister4Dump(ControlRegister4 reg)
        : VME_virtual_8086_mode_ext(reg.value & VME_BIT)
        , PVI_protected_mode_virtual_interrupts(reg.value & PVI_BIT)
        , TSD_time_stamp_disable(reg.value & TSD_BIT)
        , DE_debugging_ext(reg.value & DE_BIT)
        , PSE_page_size_ext(reg.value & PSE_BIT)
        , PAE_physical_address_ext(reg.value & PAE_BIT)
        , MCE_machine_check_enable(reg.value & MCE_BIT)
        , PGE_page_global_enable(reg.value & PGE_BIT)
        , PCE_performance_monitoring_counter_enable(reg.value & PCE_BIT)
        , OSFXSR_os_support_for_fxsave_fxstor(reg.value & OSFXSR_BIT)
        , OSXMMEXCPT_os_support_for_unmasked_simd_fp_exc(reg.value & OSXMMEXCPT_BIT)
        , UMIP_user_mode_instruction_prevention(reg.value & UMIP_BIT)
        , LA57_57_bit_linear_addresses(reg.value & LA57_BIT)
        , VMXE_vmx_enable(reg.value & VMXE_BIT)
        , SMXE_smx_enable(reg.value & SMXE_BIT)
        , FSGSBASE_fsgsbase_enable(reg.value & FSGSBASE_BIT)
        , OSXSAVE_xsave_and_proc_extended_states_enable(reg.value & OSXSAVE_BIT)
        , KL_key_locker_enable(reg.value & KL_BIT)
        , SMEP_supervisor_mode_execution_enable(reg.value & SMEP_BIT)
        , SMAP_supervisor_mode_access_prevention_enable(reg.value & SMAP_BIT)
        , PKE_protection_key_enable(reg.value & PKE_BIT)
        , CET_control_flow_enforce_tech(reg.value & CET_BIT)
        , PKS_protection_key_supervisor_mode_enable(reg.value & PKS_BIT)
        , UINTR_user_interrupts_enable(reg.value & UINTR_BIT)
        , LAM_SUP_supervisor_linear_address_masking_enable(reg.value & LAM_SUP_BIT) {}
};

void print_CR0_dump(ControlRegister0Dump);
void print_CR2_dump(ControlRegister2);
void print_CR3_dump(ControlRegister3Dump);
void print_CR4_dump(ControlRegister4Dump);
