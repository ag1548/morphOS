#pragma once

#define PS2_DATA_PORT                   0x60
#define PS2_STATUS_PORT                 0x64

#define PS2_BYTE_READY                  1
#define KEYBOARD__IS_NOT_MOUSE_DATA_BIT 0x00

namespace Keyboard {

void handle_interrupt();
void init();

} // namespace Keyboard
