#pragma once

extern "C" void printf(const char *format, ...);

class InterruptDisabler {

    void cli() {
        asm volatile("cli" ::: "memory");
    }

    void sti() {
        asm volatile("sti" ::: "memory");
    }

    void disable_interrupts() {
        cli();
    }

    void enable_interrupts() {
        sti();
    }

public:
    InterruptDisabler() {
        printf("Disabling interrupts...\n");
        disable_interrupts();
        printf("Disabled interrupts!\n");
    }

    ~InterruptDisabler() {
        printf("Reenabling Interrupts...\n");
        enable_interrupts();
        printf("Reenabled Interrupts!\n");
    }
};
