#pragma once

/*

mmap_addr = 0x9000, mmap_length = 0xa8
 size = 0x14, base_addr = 0x000000000, length = 0x00009fc00, type = 0x1
 ----------------------------- NOT USABLE -----------------------------
 ----------------------------- NOT USABLE -----------------------------
 size = 0x14, base_addr = 0x000100000, length = 0x0bfee0000, type = 0x1
 ----------------------------- NOT USABLE -----------------------------
 ----------------------------- NOT USABLE -----------------------------
 size = 0x14, base_addr = 0x100000000, length = 0x040000000, type = 0x1

*/

namespace MemoryManager {

void init();

}
