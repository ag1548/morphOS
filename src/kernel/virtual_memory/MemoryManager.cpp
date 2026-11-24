#include "MemoryManager.h"

#include "common.h"
#include "ControlRegisters.h"
#include "multiboot.h"
#include "PageDirectoryPointerTable.h"
#include "PhysicalAddress.h"

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

    kprintf("Aligning memory map regions to page boundaries...\n");

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
            kprintf("    PAGE_START: mmap->addr start not aligned to page size. Realigning...\n");
            kprintf("        - %x -> ", (unsigned int)mmap->addr);
            unsigned int extra_bytes  = PAGE_SIZE - (unsigned int)mmap->addr % PAGE_SIZE;
            mmap->addr               += extra_bytes;
            mmap->len                -= extra_bytes;
            kprintf("%x\n", (unsigned int)mmap->addr);
            kprintf("    DONE!\n");
        }

        // Align memory map page end
        if (mmap->len % PAGE_SIZE != 0) {
            kprintf("    PAGE_END: mmap->len not aligned to page size. Realigning...\n");
            kprintf("        - %x -> ", (unsigned int)mmap->len);
            mmap->len -= mmap->len % PAGE_SIZE;
            kprintf("%x\n", (unsigned int)mmap->len);
            kprintf("    DONE!\n");
        }
    }

    print_memory_map();
}

extern "C" {
PageDirectoryPointerTable *virtual_memory_data_structures_ptr;
}
PageDirectoryPointerTable &pdpt() {
    return *virtual_memory_data_structures_ptr;
}

PageDirectory &create_page_directory_at_addr(PhysicalAddress addr) {
    return *reinterpret_cast<PageDirectory *>(addr.get());
}

void move_pdpt_into_cr3() {

    // The address here is already page aligned
    const unsigned int vmds = reinterpret_cast<unsigned int>(virtual_memory_data_structures_ptr);

    kprintf("Assigning %x to cr3\n", vmds);
    asm volatile("mov %0, %%cr3"
                 :
                 : "r"(vmds)
                 : "memory");
}

/*

The Page Directory Pointer Table lives hardcoded at address `virtual_memory_data_structures` defined in `boot.S`.

Page Directory 1 lives hardcoded at address `virtual_memory_data_structures` + PAGE_SIZE * 1
Page Directory 2 lives hardcoded at address `virtual_memory_data_structures` + PAGE_SIZE * 2
Page Directory 3 lives hardcoded at address `virtual_memory_data_structures` + PAGE_SIZE * 3
Page Directory 4 lives hardcoded at address `virtual_memory_data_structures` + PAGE_SIZE * 4

*/
void create_virtual_memory_data_structures() {

    const unsigned int vmds = reinterpret_cast<unsigned int>(virtual_memory_data_structures_ptr);
    kprintf("Creating VM Data Structures at addr: %x\n", vmds);

    // Page Directory Pointer Table & Page Directories 1 - 4
    pdpt().reset();
    for (unsigned int i = 0; i < 4; i++) {

        PageDirectory &pd = create_page_directory_at_addr(PhysicalAddress(vmds + PAGE_SIZE * (i + 1)));

        unsigned long long pd_addr = reinterpret_cast<unsigned int>(&pd);
        pdpt().set_pd_addr(i, pd_addr);

        pdpt().set_pd_present(i);
    }

    move_pdpt_into_cr3();
}

/*

Since we have sections of RAM that are above the 0x1_00_00_00_00 boundary.

*/
void turn_on_physical_address_extension() {
    kprintf("Turning on PAE...\n");
    asm volatile("mov %%cr4,                   %%eax\n\t"
                 "or  $" STRINGIFY(PAE_BIT) ", %%eax\n\t"
                 "mov %%eax,                   %%cr4\n\t"
                :
                :
                : "eax", "cc", "memory");
}

/*

_Must_ be loaded before this point... See Intel manual Vol 3, Section 11.8

    - IDT
    - GDT
    - At least one Page Directory and one Page Table
    - <A few others...>

*/
void turn_on_paging() {
    // See Intel manual Vol 3, 11.9.1 - which requires a far jump immediately after the `mov into cr0`
    asm volatile("mov %%cr0,                 %%eax\n\t"
                 "or $" STRINGIFY(PG_BIT) ", %%eax\n\t" // Paging Enable
                 "or $" STRINGIFY(PE_BIT) ", %%eax\n\t" // Protection Enable
                 "mov %%eax,                 %%cr0\n\t"
                 "ljmp $0x08, $1f\n\t" // The Code segment descriptor is 8 bytes (0x08) from the GDT pointer; and can only be changed by a `ljmp` (not a mov)
                 "1:\n\t"
                     :
                     :
                     : "eax", "memory");
}

void init() {

    align_memory_map_regions_to_page_boundaries();

    turn_on_physical_address_extension();

    create_virtual_memory_data_structures();

    // Set the Page Table Entries (?)

    turn_on_paging();

    // Get a page fault to trigger
    // Handle page faults :^)

    kprintf("Memory Manager initialized!\n");
}

} // namespace MemoryManager
