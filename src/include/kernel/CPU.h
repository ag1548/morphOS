#pragma once

void cli();
void sti();

struct Descriptor {
    unsigned int low;
    unsigned int high;
};
typedef struct Descriptor Descriptor;

struct DescriptorPointer {
    unsigned short limit;
    void *address;
};
typedef struct DescriptorPointer DescriptorPointer;

void gdt_init();
