#pragma once

namespace IO {

inline void outb(unsigned short port, unsigned char val) {
    asm volatile("outb %b0, %w1"
                 :
                 : "a"(val), "Nd"(port)
                 : "memory");
}

inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile("inb %w1, %b0"
                 : "=a"(ret)
                 : "Nd"(port)
                 : "memory");
    return ret;
}

inline void io_wait(void) {
    outb(0x80, 0);
}

} // namespace IO
