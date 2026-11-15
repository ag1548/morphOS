/*

Whenever you write to the mouse

MOUSE COMMANDS
==============

Hex value  Meaning                   Description
0xFF       Reset                     The mouse probably sends ACK (0xFA) plus several more bytes, then resets itself, and always sends 0xAA.
0xFE       Resend                    This command makes the mouse send its most recent packet to the host again.
0xF6       Set Defaults              Disables streaming, sets the packet rate to 100 per second, and resolution to 4 pixels per mm.
0xF5       Disable Packet Streaming  The mouse stops sending automatic packets.
0xF4       Enable Packet Streaming   The mouse starts sending automatic packets when the mouse moves or is clicked.
0xF3       Set Sample Rate           Requires an additional data byte: automatic packets per second (see below for legal values).
0xF2       Get MouseID               The mouse sends sends its current "ID", which may change with mouse initialization.
0xEB       Request Single Packet     The mouse sends ACK, followed by a complete mouse packet with current data.
0xE9       Status Request            The mouse sends ACK, then 3 status bytes. See below for the status byte format.
0xE8       Set Resolution            Requires an additional data byte: pixels per millimeter resolution (value 0 to 3)

*/

#pragma once

#define PS2_DATA_PORT                      0x60
#define PS2_STATUS_PORT                    0x64
#define PS2_ACK                            0xFA
#define PS2_BYTE_READY                     1

#define MOUSE__IS_MOUSE_DATA_BIT           0x20

#define MOUSE_WRITE_DATA                   0xD4
#define MOUSE_ENABLE_AUX_DEVICE            0xA8
#define MOUSE_GET_COMPAQ_STATUS_BYTE       0x20
#define MOUSE_SET_COMPAQ_STATUS_BYTE       0x60
#define MOUSE_REQ_SINGLE_PACKET            0xEB

#define MOUSE_CMD_SET_RESOLUTION           0xE8
#define MOUSE_CMD_STATUS_REQUEST           0xE9
#define MOUSE_CMD_REQUEST_SINGLE_PACKET    0xEB
#define MOUSE_CMD_GET_MOUSEID              0xF2
#define MOUSE_CMD_SET_SAMPLE_RATE          0xF3
#define MOUSE_CMD_ENABLE_PACKET_STREAMING  0xF4
#define MOUSE_CMD_DISABLE_PACKET_STREAMING 0xF5
#define MOUSE_CMD_SET_DEFAULTS             0xF6
#define MOUSE_CMD_RESEND                   0xFE
#define MOUSE_CMD_RESET                    0xFF

#define BIT_IRQ_12                         (1u << 1)
#define BIT_MOUSE_CLOCK                    (1u << 5)

namespace Mouse {

void handle_interrupt();
void init();

} // namespace Mouse
