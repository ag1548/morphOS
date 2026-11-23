#include "MemoryManager.h"

#include "multiboot.h"

#define kprintf(...)            \
    do {                        \
        printf(__VA_ARGS__);    \
        debug_out(__VA_ARGS__); \
    } while (0)

extern "C" void          printf(const char *format, ...);
void                     debug_out(const char *format, ...);
extern multiboot_info_t *multiboot_info_ptr;

namespace MemoryManager {

void print_memory_map() {

    multiboot_info_t      *&mbi = multiboot_info_ptr;
    multiboot_memory_map_t *mmap;

    kprintf("mmap_addr = 0x%x, ", (unsigned)mbi->mmap_addr);
    kprintf("mmap_length = 0x%x", (unsigned)mbi->mmap_length);
    kprintf("\n");

    for (mmap = (multiboot_memory_map_t *)mbi->mmap_addr;
         (unsigned long)mmap < mbi->mmap_addr + mbi->mmap_length;
         mmap = (multiboot_memory_map_t *)((unsigned long)mmap + mmap->size + sizeof(mmap->size))) {

        switch ((unsigned)mmap->type) {
            case MULTIBOOT_MEMORY_RESERVED:
                kprintf(" ------------------------------- RESERVED -----------------------------\n");
                continue;
                break;
            case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
                kprintf(" -------------------------- ACPI RECLAIMABLE --------------------------\n");
                continue;
                break;
            case MULTIBOOT_MEMORY_NVS:
                kprintf(" --------------------------------- NVS --------------------------------\n");
                continue;
                break;
            case MULTIBOOT_MEMORY_BADRAM:
                kprintf(" ------------------------------ BAD RAM -------------------------------\n");
                continue;
                break;
            default:
                break;
        }

        kprintf(" size = 0x%x,", (unsigned)mmap->size);
        kprintf(" base_addr = 0x%x%08x,", (unsigned)(mmap->addr >> 32), (unsigned)(mmap->addr & 0xffffffff));
        kprintf(" length = 0x%x%08x,", (unsigned)(mmap->len >> 32), (unsigned)(mmap->len & 0xffffffff));
        kprintf(" type = 0x%x", (unsigned)mmap->type);
        kprintf("\n");
    }
}

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
