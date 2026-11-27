#include <heap/kmalloc.h>

#include <arch/i386/CPU.h>
#include <common.h>
#include <sys/types.h>

namespace KMALLOC {

Bitmap<CHARS_NEEDED_FOR_ALLOC_MAP> kmalloc_map;

struct AllocationHeader {
    unsigned int start_bit_idx = {0};
    unsigned int num_bits_used = {0};
};
typedef struct AllocationHeader AllocationHeader;

void init() {
    kprintf("Allocation map uses 0x%x unsigned chars!\n", kmalloc_map.size_chars());
    kprintf("kmalloc initialized!\n");
}

/*

FIXME: Surely, there's a better way. Optimize.
FIXME: Keep track of whether we have enough space in the first place instead of traversing the whole of the allocation map

*/
void *kmalloc(size_t n) {

    kprintf("Allocating %x bytes!\n", n);

    AllocationHeader header;
    static_assert(sizeof(header) == sizeof(AllocationHeader));

    const size_t        real_size    = n + sizeof(header);
    const unsigned int  units_needed = (static_cast<unsigned int>(real_size) + BYTES_PER_UNIT - 1) / BYTES_PER_UNIT;
    const unsigned int &bits_needed  = units_needed;
    header.num_bits_used             = bits_needed;

    // Find a range of bits that can accommodate the requested size + the header
    const unsigned int total_bits = CHARS_NEEDED_FOR_ALLOC_MAP * BITS_PER_UNSIGNED_CHAR;
    for (unsigned int i = 0; i < total_bits; i++) {
        if (!kmalloc_map.bit_at(i)) {
            header.start_bit_idx        = i;
            unsigned int bits_available = 1;
            for (;;) {

                ++i;

                if (i >= total_bits) {
                    break;
                }

                if (kmalloc_map.bit_at(i)) {
                    i++; // Bit at `i` has already been accounted for, doesn't need to be checked again
                    break;
                }

                bits_available++;
                if (bits_available == bits_needed) {

                    // Mark the space as used in the allocation map
                    for (unsigned int i = 0; i < bits_needed; i++) {
                        kmalloc_map.set_bit_at(header.start_bit_idx + i);
                    }

                    // Build the header in memory for bookkeeping
                    AllocationHeader *allocation_header_ptr = reinterpret_cast<AllocationHeader *>(KERNEL_MALLOC_PHYSICAL_ADDRESS_START + (header.start_bit_idx * UNITS_PER_BIT * BYTES_PER_UNIT));
                    allocation_header_ptr->start_bit_idx    = header.start_bit_idx;
                    allocation_header_ptr->num_bits_used    = header.num_bits_used;

                    unsigned char *target_address_ptr = reinterpret_cast<unsigned char *>(allocation_header_ptr) + sizeof(header);

                    // TODO: Investigate whether something like this is needed to "clean out" the area ptrs point to before returning them
                    //       const unsigned char some_canary_value = 0xBB;
                    //       memset(target_address_ptr, some_canary_value, (allocation_header_ptr->num_bits_used * UNITS_PER_BIT * BYTES_PER_UNIT) - sizeof(header));

                    kprintf("    Allocated bytes at 0x%x (header at 0x%x)!\n", reinterpret_cast<unsigned int>(target_address_ptr), reinterpret_cast<unsigned int>(target_address_ptr) - sizeof(header));
                    return static_cast<void *>(target_address_ptr);

                } else if (bits_available > bits_needed) {
                    kprintf("[[ ERROR ]]: bits_available > bits_needed... wat?");
                    CPU::hang();
                }
            }
        }
    }

    kprintf("[[ ERROR ]]: Could not find space for allocation of (real_size) 0x%x bytes\n", real_size);
    CPU::hang();

    return nullptr;
}

void kfree(void *ptr) {

    const unsigned int address = reinterpret_cast<unsigned int>(ptr);
    kprintf("    Freeing bytes at 0x%x\n", address);

    if (!ptr) {

        // FIXME: Policy decision? Remove later...
        kprintf("Freeing a null ptr. Error?\n");
        CPU::hang();

        // return
    }

    // Mark the space as available in the allocation map
    AllocationHeader *allocation_header_ptr = reinterpret_cast<AllocationHeader *>(static_cast<unsigned char *>(ptr) - sizeof(AllocationHeader));
    for (unsigned int i = 0; i < allocation_header_ptr->num_bits_used; i++) {
        kmalloc_map.clear_bit_at(allocation_header_ptr->start_bit_idx + i);
    }

    // TODO: Investigate whether something like this is needed to "clean out" the area ptrs point to before returning them
    //       const unsigned char some_canary_value = 0xAA;
    //       memset(allocation_header_ptr, some_canary_value, allocation_header_ptr->num_bits_used * UNITS_PER_BIT * BYTES_PER_UNIT);
}

} // namespace KMALLOC
