#include <heap/kmalloc.h>

#include <common.h>

namespace KMALLOC {

Bitmap<CHARS_NEEDED> allocation_map;

void init() {

    kprintf("Allocation map uses 0x%x unsigned chars!\n", allocation_map.size());

    kprintf("kmalloc initialized!\n");
}

void *kmalloc() {
    // TODO
    return 0;
}

void kfree(void *ptr) {
    // TODO
    (void)ptr;
}

} // namespace KMALLOC
