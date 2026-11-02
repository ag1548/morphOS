#pragma once

struct Descriptor {
    unsigned int low;
    unsigned int high;
};
typedef struct Descriptor Descriptor;

/*

Needs `[[gnu::packed]]` because the compiler inserts a 2-byte padding because of the `unsigned short`.

*/
struct [[gnu::packed]] DescriptorPointer {
    unsigned short limit;
    void *address;
};
typedef struct DescriptorPointer DescriptorPointer;

static_assert(sizeof(DescriptorPointer) == 6);

void gdt_init();
