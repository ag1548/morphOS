#include "PIT.h"

#include "IO.h"
#include "PIC.h"

namespace PIT {

void init() {

    unsigned short timer_reload = (BASE_FREQUENCY / TICKS_PER_SECOND);

    IO::outb(PIT_CTL, TIMER0_SELECT | WRITE_WORD | MODE_RATE);
    IO::outb(TIMER0_CTL, timer_reload & 0xFF);
    IO::outb(TIMER0_CTL, (timer_reload >> 8) & 0xFF);

    PIC::clear_interrupt_mask(PIC_TIMER_TICK);
}

}
