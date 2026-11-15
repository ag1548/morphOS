#include "Keyboard.h"

#include "PIC.h"

namespace Keyboard {

void init() {

    PIC::clear_interrupt_mask(PIC_KEYBOARD);
}

} // namespace Keyboard
