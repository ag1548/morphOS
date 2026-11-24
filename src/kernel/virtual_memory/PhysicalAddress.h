#pragma once

#include "common.h"
#include "CPU.h"

static_assert(sizeof(unsigned int) == 4);

class PhysicalAddress {

public:

    PhysicalAddress(unsigned int address)
        : address(address)
        {}

    unsigned int get() const {

        // FIXME: Remove this later...
        if (!address) {
            kprintf("PhysicalAddress get() called with address value of 0!\n");
            CPU::hang();
        }

        return address;
    }
    void set(unsigned int new_address) { address = new_address; }

    unsigned char* as_ptr() { return reinterpret_cast<unsigned char*>(address); }

private:

    unsigned int address = { 0 };

};
