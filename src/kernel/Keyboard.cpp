#include "Keyboard.h"

#include "IO.h"
#include "Mouse.h"
#include "PIC.h"

extern "C" void printf(const char *format, ...);
void            debug_out(const char *format, ...);

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
        printf("Keyboard Byte Received! %x\n", data);
        debug_out("Keyboard Byte Received! %x\n", data);
    }
}

void init() {
    printf("Keyboard initialized!\n");
    debug_out("Keyboard initialized!\n");
    PIC::clear_interrupt_mask(PIC_KEYBOARD);
}

} // namespace Keyboard
