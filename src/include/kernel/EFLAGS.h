#pragma once

/*

===============
EFLAGS REGISTER
===============

Intel Manual 3.4.3 - Vol. 1 3-15

*/

#define CF_BIT   0b00000000000000000000000000000001
#define PF_BIT   0b00000000000000000000000000000100
#define AF_BIT   0b00000000000000000000000000010000
#define ZF_BIT   0b00000000000000000000000001000000
#define SF_BIT   0b00000000000000000000000010000000
#define TF_BIT   0b00000000000000000000000100000000
#define IF_BIT   0b00000000000000000000001000000000
#define DF_BIT   0b00000000000000000000010000000000
#define OF_BIT   0b00000000000000000000100000000000
#define IOPL_BIT 0b00000000000000000011000000000000
#define NT_BIT   0b00000000000000000100000000000000
#define RF_BIT   0b00000000000000010000000000000000
#define VM_BIT   0b00000000000000100000000000000000
#define AC_BIT   0b00000000000001000000000000000000
#define VIF_BIT  0b00000000000010000000000000000000
#define VIP_BIT  0b00000000000100000000000000000000
#define ID_BIT   0b00000000001000000000000000000000

struct EFLAGSRegister {
    unsigned int value;
    EFLAGSRegister(unsigned int value)
        : value(value) {}
};
typedef struct EFLAGSRegister EFLAGSRegister;
struct EFLAGSRegisterDump {

    bool CF_carry_flag;
    bool PF_parity_flag;
    bool AF_aux_carry_flag;
    bool ZF_zero_flag;
    bool SF_sign_flag;
    bool TF_trap_flag;
    bool IF_interrupt_enable_flag;
    bool DF_direction_flag;
    bool OF_overflow_flag;
    bool IOPL_io_privilege_level;
    bool NT_nested_task;
    bool RF_resume_flag;
    bool VM_virtual_8086_mode;
    bool AC_alignment_check_access_control;
    bool VIF_virtual_interrupt_flag;
    bool VIP_virtual_interrupt_pending;
    bool ID_id_flag;

    EFLAGSRegisterDump(EFLAGSRegister reg)
        : CF_carry_flag(reg.value & CF_BIT)
        , PF_parity_flag(reg.value & PF_BIT)
        , AF_aux_carry_flag(reg.value & AF_BIT)
        , ZF_zero_flag(reg.value & ZF_BIT)
        , SF_sign_flag(reg.value & SF_BIT)
        , TF_trap_flag(reg.value & TF_BIT)
        , IF_interrupt_enable_flag(reg.value & IF_BIT)
        , DF_direction_flag(reg.value & DF_BIT)
        , OF_overflow_flag(reg.value & OF_BIT)
        , IOPL_io_privilege_level(reg.value & IOPL_BIT)
        , NT_nested_task(reg.value & NT_BIT)
        , RF_resume_flag(reg.value & RF_BIT)
        , VM_virtual_8086_mode(reg.value & VM_BIT)
        , AC_alignment_check_access_control(reg.value & AC_BIT)
        , VIF_virtual_interrupt_flag(reg.value & VIF_BIT)
        , VIP_virtual_interrupt_pending(reg.value & VIP_BIT)
        , ID_id_flag(reg.value & ID_BIT)
        {}
};

void print_EFLAGSRegister_dump(EFLAGSRegisterDump);
