#include <Keyboard.h>

#include <arch/i386/PIC.h>
#include <common.h>
#include <IO.h>
#include <Mouse.h>

namespace Keyboard {

void handle_interrupt() {
    for (;;) {

        const unsigned char status           = IO::inb(PS2_STATUS_PORT);
        const bool          can_read         = status & PS2_BYTE_READY;
        const bool          is_keyboard_data = (status & MOUSE__IS_MOUSE_DATA_BIT) == KEYBOARD__IS_NOT_MOUSE_DATA_BIT;

        if (!can_read || !is_keyboard_data) {
            return;
        }

        unsigned char data = IO::inb(PS2_DATA_PORT);
        kprintf("Keyboard Byte Received! %x\n", data);
    }
}

void init() {
    kprintf("Keyboard initialized!\n");
    PIC::clear_interrupt_mask(PIC_KEYBOARD);
}

} // namespace Keyboard
