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

    kprintf("Memory Manager initialized!\n");
}

} // namespace MemoryManager
