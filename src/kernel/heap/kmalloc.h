#pragma once

/*

See <virtual_memory/MemoryManger.h> for memory map.

*/

#include <AG/Bitmap.h>

#define KERNEL_MALLOC_PHYSICAL_ADDRESS_START (8 * MB)
#define KERNEL_MALLOC_ALLOCATION_SPACE_SIZE  (8 * MB)

#define BYTES_PER_CHUNK                      8
#define CHUNKS_PER_BIT                       1
#define BITS_PER_UNSIGNED_CHAR               8

#define CHARS_NEEDED                         (KERNEL_MALLOC_ALLOCATION_SPACE_SIZE / BYTES_PER_CHUNK / CHUNKS_PER_BIT / BITS_PER_UNSIGNED_CHAR)

namespace KMALLOC {

void init();

void *kmalloc();
void  kfree(void *);

} // namespace KMALLOC
