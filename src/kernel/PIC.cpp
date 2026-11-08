#include "PIC.h"

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
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait(); // starts the initialization sequence (in cascade mode)
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    // ICW2
    outb(PIC1_DATA, offset1);
    io_wait(); // Master PIC vector offset
    outb(PIC2_DATA, offset2);
    io_wait(); // Slave PIC vector offset

    // ICW3
    outb(PIC1_DATA, 1 << CASCADE_IRQ);
    io_wait(); // Tell Master PIC that there is a slave PIC at IRQ2
    outb(PIC2_DATA, CASCADE_IRQ);
    io_wait(); // Tell Slave PIC its cascade identity

    // ICW4
    outb(PIC1_DATA, ICW4_8086);
    io_wait(); // Have the PICs use 8086 mode (and not 8080 mode)
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    // Mask both PICs, except for "2" which is the channel for slave PIC
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
    IRQ_clear_mask(PIC_SLAVE_PIC_CASCADE);

    // Enable Timer Tick (Enablement might need to be delayed for further setup)
    IRQ_clear_mask(PIC_TIMER_TICK);
}

void PIC_sendEOI(unsigned char irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }

    outb(PIC1_COMMAND, PIC_EOI);
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
    value = inb(port) | (1 << irq_line);
    outb(port, value);
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
    value = inb(port) & ~(1 << irq_line);
    outb(port, value);
}

static unsigned short __pic_get_irq_reg(int ocw3) {
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
    outb(PIC1_COMMAND, ocw3);
    outb(PIC2_COMMAND, ocw3);
    return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}

/* Returns the combined value of the cascaded PICs irq request register (IRR) */
unsigned short pic_get_irr(void) {
    return __pic_get_irq_reg(PIC_READ_IRR);
}

/* Returns the combined value of the cascaded PICs in-service register (ISR) */
unsigned short pic_get_isr(void) {
    return __pic_get_irq_reg(PIC_READ_ISR);
}
