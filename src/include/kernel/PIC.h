#pragma once

#define PIC1         0x20 /* IO base address for master PIC */
#define PIC2         0xA0 /* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2 + 1)

#define PIC_EOI 0x20 /* End-of-interrupt command code */

#define ICW1_ICW4      0x01 /* Indicates that ICW4 will be present */
#define ICW1_SINGLE    0x02 /* Single (cascade) mode */
#define ICW1_INTERVAL4 0x04 /* Call address interval 4 (8) */
#define ICW1_LEVEL     0x08 /* Level triggered (edge) mode */
#define ICW1_INIT      0x10 /* Initialization - required! */

#define ICW4_8086       0x01 /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO       0x02 /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08 /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define ICW4_SFNM       0x10 /* Special fully nested (not) */

#define CASCADE_IRQ 2

// TODO: These need to be verified...
#define PIC_TIMER_TICK         (0)
#define PIC_KEYBOARD           (1)
#define PIC_SLAVE_PIC_CASCADE  (2)
#define PIC_SERIAL_PORT_COM2   (3)
#define PIC_SERIAL_PORT_COM1   (4)
#define PIC_PRL2_OR_SOUND_CARD (5)
#define PIC_FLOPPY             (6)
#define PIC_PRL1_OR_SPURIOUS   (7)
#define PIC_RTC                (8)
#define PIC_IRQ2_REDIRECT      (9)
#define PIC_PERPH1             (10)
#define PIC_PERPH2             (11)
#define PIC_MOUSE              (12)
#define PIC_COPRO_ERROR        (13)
#define PIC_ATA_IDE_1          (14)
#define PIC_ATA_IDE_2          (15)

#define IRQ_BASE               PIC1
#define IRQ_TIMER_TICK         (IRQ_BASE + 0)
#define IRQ_KEYBOARD           (IRQ_BASE + 1)
#define IRQ_SLAVE_PIC_CASCADE  (IRQ_BASE + 2)
#define IRQ_SERIAL_PORT_COM2   (IRQ_BASE + 3)
#define IRQ_SERIAL_PORT_COM1   (IRQ_BASE + 4)
#define IRQ_PRL2_OR_SOUND_CARD (IRQ_BASE + 5)
#define IRQ_FLOPPY             (IRQ_BASE + 6)
#define IRQ_PRL1_OR_SPURIOUS   (IRQ_BASE + 7)
#define IRQ_RTC                (IRQ_BASE + 8)
#define IRQ_IRQ2_REDIRECT      (IRQ_BASE + 9)
#define IRQ_PERPH1             (IRQ_BASE + 10)
#define IRQ_PERPH2             (IRQ_BASE + 11)
#define IRQ_MOUSE              (IRQ_BASE + 12)
#define IRQ_COPRO_ERROR        (IRQ_BASE + 13)
#define IRQ_ATA_IDE_1          (IRQ_BASE + 14)
#define IRQ_ATA_IDE_2          (IRQ_BASE + 15)

/*

ISR - In-Service Register
IRR - Interrupt Request Register

*/
#define PIC_READ_IRR 0x0a /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR 0x0b /* OCW3 irq service next CMD read */

static inline void outb(unsigned short port, unsigned char val) {
    asm volatile("outb %b0, %w1"
                 :
                 : "a"(val), "Nd"(port)
                 : "memory");
}

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile("inb %w1, %b0"
                 : "=a"(ret)
                 : "Nd"(port)
                 : "memory");
    return ret;
}

static inline void io_wait(void) {
    outb(0x80, 0);
}

void           PIC_remap(int offset1, int offset2);
void           PIC_sendEOI(unsigned char irq);
void           IRQ_set_mask(unsigned char IRQline);
void           IRQ_clear_mask(unsigned char IRQline);
unsigned short pic_get_irr(void);
unsigned short pic_get_isr(void);
