#include "PIC.h"

#include "IO.h"

/*
arguments:
        offset1 - vector offset for master PIC
                vectors on the master become offset1..offset1+7
        offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_remap(int offset1, int offset2) {

    // "ICW" = "Initialization Command Word"
    // "OCW" = "Operation Command Word"

    /* This command makes the PIC wait for 3 extra "initialisation words" on the data port */
    IO::outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    IO::io_wait(); // starts the initialization sequence (in cascade mode)
    IO::outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    IO::io_wait();

    // ICW2
    IO::outb(PIC1_DATA, offset1);
    IO::io_wait(); // Master PIC vector offset
    IO::outb(PIC2_DATA, offset2);
    IO::io_wait(); // Slave PIC vector offset

    // ICW3
    IO::outb(PIC1_DATA, 1 << CASCADE_IRQ);
    IO::io_wait(); // Tell Master PIC that there is a slave PIC at IRQ2
    IO::outb(PIC2_DATA, CASCADE_IRQ);
    IO::io_wait(); // Tell Slave PIC its cascade identity

    // ICW4
    IO::outb(PIC1_DATA, ICW4_8086);
    IO::io_wait(); // Have the PICs use 8086 mode (and not 8080 mode)
    IO::outb(PIC2_DATA, ICW4_8086);
    IO::io_wait();

    // Mask both PICs, except for "2" which is the channel for slave PIC
    IO::outb(PIC1_DATA, 0xff);
    IO::outb(PIC2_DATA, 0xff);
    IRQ_clear_mask(PIC_SLAVE_PIC_CASCADE);
}

void PIC_sendEOI(unsigned char irq) {
    if (irq >= 8) {
        IO::outb(PIC2_COMMAND, PIC_EOI);
    }

    IO::outb(PIC1_COMMAND, PIC_EOI);
}

/* To "set" the mask = Blocked IRQ */
void IRQ_set_mask(unsigned char irq_line) {
    unsigned short port;
    unsigned char  value;

    if (irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port      = PIC2_DATA;
        irq_line -= 8;
    }
    value = IO::inb(port) | (1 << irq_line);
    IO::outb(port, value);
}

/* To "clear" the mask = Allowed IRQ */
void IRQ_clear_mask(unsigned char irq_line) {
    unsigned short port;
    unsigned char  value;

    if (irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port      = PIC2_DATA;
        irq_line -= 8;
    }
    value = IO::inb(port) & ~(1 << irq_line);
    IO::outb(port, value);
}

static unsigned short __pic_get_irq_reg(int ocw3) {
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
    IO::outb(PIC1_COMMAND, ocw3);
    IO::outb(PIC2_COMMAND, ocw3);
    return (IO::inb(PIC2_COMMAND) << 8) | IO::inb(PIC1_COMMAND);
}

/* Returns the combined value of the cascaded PICs irq request register (IRR) */
unsigned short pic_get_irr(void) {
    return __pic_get_irq_reg(PIC_READ_IRR);
}

/* Returns the combined value of the cascaded PICs in-service register (ISR) */
unsigned short pic_get_isr(void) {
    return __pic_get_irq_reg(PIC_READ_ISR);
}

void initialize_PIT() {
    unsigned short timer_reload = (BASE_FREQUENCY / TICKS_PER_SECOND);

    IO::outb(PIT_CTL, TIMER0_SELECT | WRITE_WORD | MODE_RATE);
    IO::outb(TIMER0_CTL, timer_reload & 0xFF);
    IO::outb(TIMER0_CTL, (timer_reload >> 8) & 0xFF);

    IRQ_clear_mask(PIC_TIMER_TICK);
}
