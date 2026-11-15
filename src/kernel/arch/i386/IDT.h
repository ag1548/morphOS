#pragma once

#define PS2_DATA_PORT 0x60

namespace IDT {

struct Descriptor {
    unsigned int low;
    unsigned int high;
};
typedef struct Descriptor Descriptor;

/*

Needs `[[gnu::packed]]` because the compiler inserts a 2-byte padding because of the `unsigned short`.

*/
struct [[gnu::packed]] DescriptorPointer {
    unsigned short size;
    void          *address;
};
typedef struct DescriptorPointer DescriptorPointer;

static_assert(sizeof(DescriptorPointer) == 6);

void init();

} // namespace IDT
