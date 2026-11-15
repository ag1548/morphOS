#pragma once

namespace CPU {

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

inline void clear_interrupt_flag() {
    asm volatile("cli" ::: "memory");
}

inline void set_interrupt_flag() {
    asm volatile("sti" ::: "memory");
}

inline void idle_loop() {
    for (;;) {
        asm volatile("hlt" ::: "memory");
    }
}

} // namespace CPU
