#pragma once

#include <arch/i386/CPU.h>
#include <common.h>
#include <virtual_memory/PageDirectory.h>

#define PAGE_PRESENT 1ull

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
        kprintf("    PDPT::set_pd_addr[%x]    = 0x%08x%08x\n", idx, (unsigned int)(addr >> 32), (unsigned int)(addr & 0xFFFFFFFF));
        pd_ptrs[idx] = addr;
    }

    void set_pd_present(const unsigned int idx) {
        kprintf("    PDPT::set_pd_present[%x] = 0x%08x%08x -> ", idx, (unsigned int)(pd_ptrs[idx] >> 32), (unsigned int)(pd_ptrs[idx] & 0xFFFFFFFF));
        pd_ptrs[idx] |= PAGE_PRESENT;
        kprintf("0x%08x%08x\n", (unsigned int)(pd_ptrs[idx] >> 32), (unsigned int)(pd_ptrs[idx] & 0xFFFFFFFF));
    }

    void set_pd_not_present(const unsigned int idx) {
        kprintf("    PDPT::set_pd_not_present[%x] = 0x%08x%08x -> ", idx, (unsigned int)(pd_ptrs[idx] >> 32), (unsigned int)(pd_ptrs[idx] & 0xFFFFFFFF));
        pd_ptrs[idx] &= ~(PAGE_PRESENT);
        kprintf("0x%08x%08x\n", (unsigned int)(pd_ptrs[idx] >> 32), (unsigned int)(pd_ptrs[idx] & 0xFFFFFFFF));
    }

private:
    unsigned long long pd_ptrs[4] = {0, 0, 0, 0};
};

static_assert(sizeof(PageDirectoryPointerTable) == (8 * 4));
