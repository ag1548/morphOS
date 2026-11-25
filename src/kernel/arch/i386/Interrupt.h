#pragma once

#include <common.h>

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
        kprintf("Disabling interrupts...\n");
        disable_interrupts();
        kprintf("Disabled interrupts!\n");
    }

    ~InterruptDisabler() {
        kprintf("Reenabling Interrupts...\n");
        enable_interrupts();
        kprintf("Reenabled Interrupts!\n");
    }
};
