print("""#include "irq_handlers.h"

""")

function_body = """
static void unimplemented_interrupt_handler_{0}() {{
    printf("Unhandled IRQ {0}");

    void debug_out(const char *const str);
    debug_out("Unhandled IRQ {0}");

    hang();
}}
"""
for irq_handler_id in range(256):
    print(function_body.format(irq_handler_id))
