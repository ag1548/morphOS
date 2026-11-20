#include "MemoryManager.h"

#define kprintf(...)            \
    do {                        \
        printf(__VA_ARGS__);    \
        debug_out(__VA_ARGS__); \
    } while (0)

extern "C" void printf(const char *format, ...);
void            debug_out(const char *format, ...);

namespace MemoryManager {

void init() {

    // Decide on a memory map :^)
    // Set the Page Directory
    // Set the Page Table Entries
    // Turn paging on
    // Get a page fault to trigger
    // Handle page faults :^)

    kprintf("Memory Manager initialized!\n");
}

} // namespace MemoryManager
