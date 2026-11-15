#include "Mouse.h"

#include "CPU.h"
#include "IO.h"
#include "PIC.h"

extern "C" void printf(const char *format, ...);
void            debug_out(const char *format, ...);

namespace Mouse {

bool check_for_mouse() {

    write(PS2_STATUS_PORT, MOUSE_WRITE_DATA);
    write(PS2_DATA_PORT, MOUSE_REQ_SINGLE_PACKET);

    unsigned char maybe_ack = read();
    if (maybe_ack == PS2_ACK) {
        printf("Mouse detected!\n");
        debug_out("Mouse detected!\n");

        // Discard mouse data packet
        read();
        read();
        read();

        return true;

    } else {
        printf("No Mouse detected!\n");
        debug_out("No Mouse detected!\n");
        return false;
    }
}

void update_compaq_status() {
    // Get the status byte
    write(PS2_STATUS_PORT, MOUSE_GET_COMPAQ_STATUS_BYTE);
    unsigned char status = read();

    // Enable IRQ12
    status |= BIT_IRQ_12;

    // Disable Mouse Clock
    // status &= ~BIT_MOUSE_CLOCK;

    // Set the status byte
    write(PS2_STATUS_PORT, MOUSE_SET_COMPAQ_STATUS_BYTE);
    write(PS2_DATA_PORT, status);
}

void initialize_mouse() {

    update_compaq_status();

    write(PS2_STATUS_PORT, MOUSE_WRITE_DATA);
    write(PS2_DATA_PORT, MOUSE_CMD_SET_DEFAULTS);
    ack();

    write(PS2_STATUS_PORT, MOUSE_WRITE_DATA);
    write(PS2_DATA_PORT, MOUSE_CMD_ENABLE_PACKET_STREAMING);
    ack();

    write(PS2_STATUS_PORT, MOUSE_WRITE_DATA);
    write(PS2_DATA_PORT, MOUSE_CMD_GET_MOUSEID);
    ack();
    unsigned char id = read();
    printf("Mouse ID: %x\n", id);
    debug_out("Mouse ID: %x\n", id);
}

void ack() {
    unsigned char data = read();
    if (data != PS2_ACK) {
        printf("Mouse ACK missed!\n\n");
        debug_out("Mouse ACK missed!\n\n");
        CPU::hang();
    }
}

// All output to port 0x60 or 0x64 must be preceded by waiting for bit 1 (value=2) of port 0x64 to become clear.
void wait_to_write() {
    for (;;) {
        if (!(IO::inb(PS2_STATUS_PORT) & 2)) {
            return;
        }
    }
}

void write(unsigned short port, unsigned char data) {
    wait_to_write();
    IO::outb(port, data);
}

// Bytes cannot be read from port 0x60 until bit 0 (value=1) of port 0x64 is set.
void wait_to_read() {
    for (;;) {
        printf("Mouse - Waiting to read!\n\n");
        debug_out("Mouse - Waiting to read!\n\n");
        if (IO::inb(PS2_STATUS_PORT) & 1) {
            return;
        }
    }
}

unsigned char read() {
    wait_to_read();
    return IO::inb(PS2_DATA_PORT);
}

void enable_ps2_aux_port() {
    write(PS2_STATUS_PORT, MOUSE_ENABLE_AUX_DEVICE);
}

void init() {

    enable_ps2_aux_port();

    if (check_for_mouse()) {
        initialize_mouse();
    }

    PIC::clear_interrupt_mask(PIC_MOUSE);
}

} // namespace Mouse
