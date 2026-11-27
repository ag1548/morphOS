#pragma once

/*

See <virtual_memory/MemoryManger.h> for memory map.

*/

#include <AG/Bitmap.h>
#include <common.h>
#include <sys/types.h>

#define KERNEL_MALLOC_PHYSICAL_ADDRESS_START (8u * MB)
#define KERNEL_MALLOC_ALLOCATION_SPACE_SIZE  (8u * MB)

#define BYTES_PER_UNIT                       8u
#define UNITS_PER_BIT                        1u

#define CHARS_NEEDED_FOR_ALLOC_MAP           (KERNEL_MALLOC_ALLOCATION_SPACE_SIZE / BYTES_PER_UNIT / UNITS_PER_BIT / BITS_PER_UNSIGNED_CHAR)

namespace KMALLOC {

void init();

void *kmalloc(size_t n);
void  kfree(void *);

} // namespace KMALLOC

using KMALLOC::kfree;
using KMALLOC::kmalloc;
