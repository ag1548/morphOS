#pragma once

/*

Kernel mem start: 0x100000
Kernel mem end  : 0x10a860 (43,104₁₀)
...
mmap_addr = 0x9000, mmap_length = 0xa8

                                                 [  651,264 B ]
 size = 0x14, base_addr = 0x000000000, length = 0x00009f000, type = 0x1

 ------------------------------- RESERVED -----------------------------
 ------------------------------- RESERVED -----------------------------

                           [1,048,576 B ]   [ 3,220,045,824 B ]
 size = 0x14, base_addr = 0x000100000, length = 0x0bfee0000, type = 0x1

 ------------------------------- RESERVED -----------------------------
 ------------------------------- RESERVED -----------------------------

                      [ 4,294,967,296 B]    [ 1,073,741,824 B]
 size = 0x14, base_addr = 0x100000000, length = 0x040000000, type = 0x1

##########################
Physical Memory Map
##########################
 0      -> ~700 KB           Reserved by QEMU
 1 MB   ->    8 MB           Kernel image
 8 MB   ->   16 MB           kernel malloc
16 MB   ->    4 GB           Userspace physical pages
 4 GB   ->    5 GB           Supervisor physical pages (just seemed easier this way...)

##########################
Virtual Memory Map
##########################
 0    ->  4 KB             Null page (so nullptr dereferences crash!)
 4 KB -> 16 MB             Identity mapped.
16 MB ->  3 GB             Available to userspace.
 3 GB ->  4 GB             Kernel-only virtual address space (>=0xc0000000)

*/

namespace MemoryManager {

void print_memory_map();
void init();

} // namespace MemoryManager
