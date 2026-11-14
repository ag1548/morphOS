#pragma once

[[noreturn]] void hang();
[[noreturn]] void print_control_registers_and_hang();

struct RegisterDump {
    unsigned int edi;
    unsigned int esi;
    unsigned int ebp;
    unsigned int esp;
    unsigned int ebx;
    unsigned int edx;
    unsigned int ecx;
    unsigned int eax;
};
typedef struct RegisterDump RegisterDump;
