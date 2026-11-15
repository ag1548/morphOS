#include "Mouse.h"

#include "CPU.h"
#include "IO.h"
#include "PIC.h"

extern "C" void printf(const char *format, ...);
void            debug_out(const char *format, ...);

namespace Mouse {

unsigned char mouse_current_byte_index;
unsigned char mouse_packet[3];

/*

All output to ports
    0x60 PS2_DATA_PORT
    0x64 PS2_STATUS_PORT
must be preceded by waiting for bit 1 (value=2) of port 0x64 to become clear.

*/
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

// Bytes cannot be read from port 0x60 PS2_DATA_PORT until bit 0 (value=1) of port 0x64 is set.
void wait_to_read() {
    for (;;) {
        if (IO::inb(PS2_STATUS_PORT) & 1) {
            return;
        }
    }
}

unsigned char read() {
    wait_to_read();
    return IO::inb(PS2_DATA_PORT);
}

void ack() {
    unsigned char data = read();
    if (data != PS2_ACK) {
        printf("Mouse ACK missed!\n\n");
        debug_out("Mouse ACK missed!\n\n");
        CPU::hang();
    }
}

void enable_ps2_aux_port() {
    write(PS2_STATUS_PORT, MOUSE_ENABLE_AUX_DEVICE);
}

void assert_mouse_first_byte() {
    const unsigned char bit_check = mouse_packet[0] & 0x08;
    if (!bit_check) {
        printf("[[ ERRROR ]] Unexpected First Mouse Byte! %x\n", mouse_packet[0]);
        debug_out("[[ ERRROR ]] Unexpected First Mouse Byte! %x\n", mouse_packet[0]);
        CPU::hang();
    }
}

void reset_mouse_packet() {
    mouse_current_byte_index = 0;
    mouse_packet[0]          = 0;
    mouse_packet[1]          = 0;
    mouse_packet[2]          = 0;
}

void handle_mouse_packet() {
    const unsigned char flags = mouse_packet[0];
    const unsigned char x     = mouse_packet[1];
    const unsigned char y     = mouse_packet[2];

    printf("Mouse Packet Received! (1) - %x (2) - %x (3) - %x\n", flags, x, y);
    debug_out("Mouse Packet Received! (1) - %x (2) - %x (3) - %x\n", flags, x, y);

    reset_mouse_packet();
}

void handle_interrupt() {
    for (;;) {

        const unsigned char status        = IO::inb(PS2_STATUS_PORT);
        const bool          can_read      = status & PS2_BYTE_READY;
        const bool          is_mouse_data = status & MOUSE__IS_MOUSE_DATA_BIT;

        if (!can_read || !is_mouse_data) {
            return;
        }

        unsigned char data                     = IO::inb(PS2_DATA_PORT);
        mouse_packet[mouse_current_byte_index] = data;
        switch (mouse_current_byte_index) {
            case 0:
                assert_mouse_first_byte();
                mouse_current_byte_index++;
                break;
            case 1:
                mouse_current_byte_index++;
                break;
            case 2:
                handle_mouse_packet();
                break;
            default:
                printf("[[ ERRROR ]] Unexpected Mouse Byte Index! %x\n", mouse_current_byte_index);
                debug_out("[[ ERRROR ]] Unexpected Mouse Byte Index! %x\n", mouse_current_byte_index);
                CPU::hang();
                break;
        }
    }
}

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

void init_state() {
    mouse_current_byte_index = 0;
    mouse_packet[0]          = 0;
    mouse_packet[1]          = 0;
    mouse_packet[2]          = 0;
}

void init() {

    init_state();

    enable_ps2_aux_port();

    if (check_for_mouse()) {
        initialize_mouse();
    }

    printf("Mouse initialized!\n");
    debug_out("Mouse initialized!\n");
    PIC::clear_interrupt_mask(PIC_MOUSE);
}

} // namespace Mouse
