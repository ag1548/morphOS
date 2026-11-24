#include "MemoryManager.h"

#include "common.h"
#include "multiboot.h"

extern multiboot_info_t *multiboot_info_ptr;

namespace MemoryManager {

#define PAGE_SIZE 4096

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

void align_memory_map_regions_to_page_boundaries() {

    multiboot_info_t      *&mbi = multiboot_info_ptr;
    multiboot_memory_map_t *mmap;

    for (mmap = (multiboot_memory_map_t *)mbi->mmap_addr;
         (unsigned long)mmap < mbi->mmap_addr + mbi->mmap_length;
         mmap = (multiboot_memory_map_t *)((unsigned long)mmap + mmap->size + sizeof(mmap->size))) {

        switch ((unsigned)mmap->type) {
            case MULTIBOOT_MEMORY_RESERVED:
            case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
            case MULTIBOOT_MEMORY_NVS:
            case MULTIBOOT_MEMORY_BADRAM:
                continue;
                break;
            default:
                break;
        }

        // Align memory map page start
        if ((unsigned int)mmap->addr % PAGE_SIZE != 0) {
            kprintf("PAGE_START: mmap->addr start not aligned to page size. Realigning...\n");
            kprintf("    - %x -> ", (unsigned int)mmap->addr);
            unsigned int extra_bytes  = PAGE_SIZE - (unsigned int)mmap->addr % PAGE_SIZE;
            mmap->addr               += extra_bytes;
            mmap->len                -= extra_bytes;
            kprintf("%x\n", (unsigned int)mmap->addr);
            kprintf("DONE!\n");
        }

        // Align memory map page end
        if (mmap->len % PAGE_SIZE != 0) {
            kprintf("PAGE_END: mmap->len not aligned to page size. Realigning...\n");
            kprintf("    - %x -> ", (unsigned int)mmap->len);
            mmap->len -= mmap->len % PAGE_SIZE;
            kprintf("%x\n", (unsigned int)mmap->len);
            kprintf("DONE!\n");
        }
    }
}

void init() {

    align_memory_map_regions_to_page_boundaries();
    print_memory_map();

    // Set the Page Directory
    // Set the Page Table Entries
    // Turn paging on
    // Get a page fault to trigger
    // Handle page faults :^)

    kprintf("Memory Manager initialized!\n");
}

} // namespace MemoryManager
