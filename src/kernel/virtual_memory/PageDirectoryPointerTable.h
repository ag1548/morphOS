#pragma once

#include "common.h"
#include "CPU.h"
#include "PageDirectory.h"

#define PAGE_PRESENT 1u

class PageDirectoryPointerTable {

public:
    void reset() {
        for (unsigned int i = 0; i < 4; i++) {
            pd_ptrs[i] = 0;
        }
    }

    PageDirectory &get_pd(const unsigned int idx) const {

        // FIXME: Remove this later...
        if (!pd_ptrs[idx]) {
            kprintf("PDPT::get_pd(idx) called with pd_ptrs[idx]-value of 0!\n");
            CPU::hang();
        }

        return *reinterpret_cast<PageDirectory *>(pd_ptrs[idx] & 0xFFFFF000);
    }

    void set_pd_addr(const unsigned int idx, const unsigned long long addr) {
        kprintf("    PDPT::set_pd_addr[%x] = %x\n", idx, addr);
        pd_ptrs[idx] = addr;
    }

    void set_pd_present(const unsigned int idx) {
        kprintf("    PDPT::set_pd_present[%x] = %x -> ", idx, pd_ptrs[idx]);
        pd_ptrs[idx] |= PAGE_PRESENT;
        kprintf("%x\n", pd_ptrs[idx]);
    }

    void set_pd_not_present(const unsigned int idx) {
        kprintf("    PDPT::set_pd_present[%x] = %x -> ", idx, pd_ptrs[idx]);
        pd_ptrs[idx] &= ~PAGE_PRESENT;
        kprintf("%x\n", pd_ptrs[idx]);
    }

private:
    unsigned long long pd_ptrs[4] = {0, 0, 0, 0};
};

static_assert(sizeof(PageDirectoryPointerTable) == (8 * 4));
