#include <arch/i386/multiboot.h>

#include <arch/i386/CPU.h>
#include <arch/i386/GDT.h>
#include <arch/i386/IDT.h>
#include <arch/i386/PIC.h>
#include <arch/i386/PIT.h>
#include <common.h>
#include <heap/kmalloc.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <virtual_memory/MemoryManager.h>

#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))

#define WHITE_FG               0b00001111

#define COLUMNS                80
#define LINES                  24
#define VIDEO                  0xB8000

static int                     xpos;
static int                     ypos;
static volatile unsigned char *video;

/*

Refer to GCC manual's 6.11.2 - Extended ASM and 6.11.3.4 for x86

Basic ASM
=========
asm asm-qualifiers (AssemblerInstructions)

Extended ASM
============
asm asm-qualifiers (AssemblerTemplate
                        : OutputOperands
                        [ : InputOperands
                        [ : Clobbers ] ])

asm asm-qualifiers (AssemblerTemplate
                        : OutputOperands
                        : InputOperands
                        : Clobbers
                        : GotoLabels)

6.11.2.6 Clobbers
=================
"memory": "Using the `memory` clobber effectively forms a read/write memory barrier for the compiler."

6.11.3.1 Simple Constraints
===========================
OutputOperands:
    Output constraints must begin with either ‘=’ or ‘+’
    - ‘=’ - a variable overwriting an existing value
    - ‘+’ - when reading and writing
InputOperands:
    "r": "A register operand is allowed provided that it is in a general register."
    "m": "A memory operand is allowed, with any kind of address that the machine supports in general. Note that the letter used for the general memory constraint can be re-defined by a back end using the TARGET_MEM_CONSTRAINT macro."

6.11.3.4 Constraints for Particular Machines
============================================
x86:
    "a": "The 'a' register"
    "d": "The 'd' register"
    "N": "Unsigned 8-bit integer constant"

*/

#define WELCOME_STR "                                _    ___  ___     \\/ \n"    \
                    "\\ /      ._ _ _  ___  _ _  ___ | |_ | . |/ __>  (\\/) \n"   \
                    "(oo)ooo  | ' ' |/ . \\| '_>| . \\| . || | |\\__ \\   )(  \n" \
                    " ||||||  |_|_|_|\\___/|_|  |  _/|_|_|\\___'<___/  (/\\) \n"  \
                    "                          |_|                    \n"

static void itoa(char *buf, int base, int d);

void debug_out_putchar(int c) {
    unsigned char c_ = static_cast<unsigned char>(c);
    asm volatile("outb %0, $0xe9"
                 :
                 : "a"(c_));
}

void debug_out(const char *format, ...) {

    char **arg = (char **)&format;
    int    c;
    char   buf[20];

    arg++;

    while ((c = *format++) != 0) {

        if (c != '%') {
            debug_out_putchar(c);
            continue;
        }

        char *p;
        char *p2;
        int   pad0 = 0;
        int   pad  = 0;

        c = *format++;
        if (c == '0') {
            pad0 = 1;
            c    = *format++;
        }

        if (c >= '0' && c <= '9') {
            pad = c - '0';
            c   = *format++;
        }

        switch (c) {
            case 'd':
            case 'u':
            case 'x':
                itoa(buf, c, *((int *)arg++));
                p = buf;
                goto string;
                break;

            case 's':
                p = *arg++;
                // if (!p) {
                //     p = "(null)";
                // }

            string:
                for (p2 = p; *p2; p2++)
                    ;
                for (; p2 < p + pad; p2++) {
                    debug_out_putchar(pad0 ? '0' : ' ');
                }
                while (*p) {
                    debug_out_putchar(*p++);
                }
                break;

            default:
                debug_out_putchar(*((int *)arg++));
                break;
        }
    }
}

static void make_buffer_newline() {

    // Move lines upwards
    for (int line = 1; line < LINES; line++) {
        for (int column = 0; column < COLUMNS; column++) {
            *(video + (column + (line - 1) * COLUMNS) * 2)     = *(video + (column + line * COLUMNS) * 2);
            *(video + (column + (line - 1) * COLUMNS) * 2 + 1) = *(video + (column + line * COLUMNS) * 2 + 1);
        }
    }
}
static void clear_remaining_line_characters() {
    const int line = LINES - 1;
    for (int column = xpos; column < COLUMNS; column++) {
        *(video + (column + (line - 1) * COLUMNS) * 2)     = ' ';
        *(video + (column + (line - 1) * COLUMNS) * 2 + 1) = WHITE_FG;
    }
}

static void start_new_line() {
    xpos = 0;
    ypos++;
    if (ypos >= LINES) {
        ypos--;
        make_buffer_newline();
    }
}

static void putchar(int c) {
    if (c == '\n' || c == '\r') {
        clear_remaining_line_characters();
        start_new_line();
        return;
    }

    *(video + (xpos + ypos * COLUMNS) * 2)     = c & 0xFF;
    *(video + (xpos + ypos * COLUMNS) * 2 + 1) = WHITE_FG;

    xpos++;
    if (xpos >= COLUMNS) {
        start_new_line();
    }
}

/* Convert the integer D to a string and save the string in BUF. If
   BASE is equal to ’d’, interpret that D is decimal, and if BASE is
   equal to ’x’, interpret that D is hexadecimal. */
static void itoa(char *buf, int base, int d) {

    char         *p = buf;
    char         *p1, *p2;
    unsigned long ud      = d;
    int           divisor = 10;

    /* If %d is specified and D is minus, put ‘-’ in the head. */
    if (base == 'd' && d < 0) {
        *p++ = '-';
        buf++;
        ud = -d;
    } else if (base == 'x') {
        divisor = 16;
    }

    /* Divide UD by DIVISOR until UD == 0. */
    do {
        int remainder = ud % divisor;
        *p++          = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    } while (ud /= divisor);

    /* Terminate BUF. */
    *p = 0;

    /* Reverse BUF. */
    p1 = buf;
    p2 = p - 1;
    while (p1 < p2) {
        char tmp = *p1;
        *p1      = *p2;
        *p2      = tmp;
        p1++;
        p2--;
    }
}

extern "C" void printf(const char *format, ...) {

    char **arg = (char **)&format;
    int    c;
    char   buf[20];

    arg++;

    while ((c = *format++) != 0) {

        if (c != '%') {
            putchar(c);
            continue;
        }

        char *p;
        char *p2;
        int   pad0 = 0;
        int   pad  = 0;

        c = *format++;
        if (c == '0') {
            pad0 = 1;
            c    = *format++;
        }

        if (c >= '0' && c <= '9') {
            pad = c - '0';
            c   = *format++;
        }

        switch (c) {
            case 'd':
            case 'u':
            case 'x':
                itoa(buf, c, *((int *)arg++));
                p = buf;
                goto string;
                break;

            case 's':
                p = *arg++;
                // if (!p) {
                //     p = "(null)";
                // }

            string:
                for (p2 = p; *p2; p2++)
                    ;
                for (; p2 < p + pad; p2++) {
                    putchar(pad0 ? '0' : ' ');
                }
                while (*p) {
                    putchar(*p++);
                }
                break;

            default:
                putchar(*((int *)arg++));
                break;
        }
    }
}

static void cls(void) {

    video = (unsigned char *)VIDEO;

    for (int i = 0; i < COLUMNS * LINES * 2; i++) {
        *(video + i) = 0;
    }

    xpos = 0;
    ypos = 0;
}

extern "C" {
unsigned int multiboot_magic;
}
static bool assert_multiboot_magic_number() {
    if (multiboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf("Invalid magic number: 0x%x\n", multiboot_magic);
        return false;
    }

    return true;
}

extern "C" {
multiboot_info_t *multiboot_info_ptr;
char              kernel_mem_start;
char              kernel_mem_end;
}
unsigned int kernel_mem_start_address() {
    return reinterpret_cast<unsigned int>(&kernel_mem_start);
}
unsigned int kernel_mem_end_address() {
    return reinterpret_cast<unsigned int>(&kernel_mem_end);
}
static void print_multiboot_info() {

    multiboot_info_t *&mbi = multiboot_info_ptr;

    if (!CHECK_FLAG(mbi->flags, 9)) {
        kprintf("No FLAGS!\n");
        CPU::hang();
    }

    kprintf("boot_loader_name = \"%s\"\n", mbi->boot_loader_name);
    kprintf("flags = 0x%x\n", (unsigned)mbi->flags);

    /*
    flags = 0x24f
            0b00000000_00000000_00000010_01001111
                                                ^ (0) `mem_*` fields are valid
                                               ^ (1) `boot_device` field is valid
                                              ^ (2) `cmdline` field is valid
                                             ^ (3) `mods` fields indicate what modules were loaded and where they are
                                          ^ (6) `mmap_*` fields are valid
                                      ^ (9) `boot_loader_name` field is valid
    */

    kprintf("Kernel mem start: 0x%08x\n", kernel_mem_start_address());
    kprintf("Kernel mem end  : 0x%08x\n", kernel_mem_end_address());
    kprintf("length          : 0x%08x\n", kernel_mem_end_address() - kernel_mem_start_address());
    kprintf("length          : %d\n", kernel_mem_end_address() - kernel_mem_start_address());

    if (CHECK_FLAG(mbi->flags, 0)) {
        kprintf("mem_lower = %uKB, mem_upper = %uKB\n", (unsigned)mbi->mem_lower, (unsigned)mbi->mem_upper);
    }

    /* Is boot_device valid? */
    if (CHECK_FLAG(mbi->flags, 1)) {
        kprintf("boot_device = 0x%x\n", (unsigned)mbi->boot_device);
    }

    /* Is the command line passed? */
    if (CHECK_FLAG(mbi->flags, 2)) {
        kprintf("cmdline = %s\n", (char *)mbi->cmdline);
    }

    /* Are mods_* valid? */
    if (CHECK_FLAG(mbi->flags, 3)) {

        multiboot_module_t *mod;
        int                 i;

        kprintf("mods_count = %d, mods_addr = 0x%x\n", (int)mbi->mods_count, (int)mbi->mods_addr);
        for (i = 0, mod = (multiboot_module_t *)mbi->mods_addr; i < (int)mbi->mods_count; i++, mod++) {
            kprintf(" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n", (unsigned)mod->mod_start, (unsigned)mod->mod_end, (char *)mod->cmdline);
        }
    }

    /* Bits 4 and 5 are mutually exclusive! */
    if (CHECK_FLAG(mbi->flags, 4) && CHECK_FLAG(mbi->flags, 5)) {
        kprintf("Both bits 4 and 5 are set.\n");
        return;
    }

    /* Is the section header table of ELF valid? */
    if (CHECK_FLAG(mbi->flags, 5)) {
        multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);

        kprintf("multiboot_elf_sec: "
                "num = %u, "
                "size = 0x%x, "
                "addr = 0x%x, "
                "shndx = 0x%x"
                "\n",
                (unsigned)multiboot_elf_sec->num,
                (unsigned)multiboot_elf_sec->size,
                (unsigned)multiboot_elf_sec->addr,
                (unsigned)multiboot_elf_sec->shndx);
    }

    /* Are mmap_* valid? */
    if (CHECK_FLAG(mbi->flags, 6)) {
        MemoryManager::print_memory_map();
    }
}

extern "C" void cmain() {

    cls();

    if (!assert_multiboot_magic_number()) {
        return;
    }

    debug_out(WELCOME_STR "\n");

    print_multiboot_info();

    KMALLOC::init();

    void *test_ptr = kmalloc(5);
    kfree(test_ptr);

    PIC::init();
    GDT::init();
    IDT::init();
    PIT::init();

    Keyboard::init();
    Mouse::init();

    // MemoryManager::init();

    CPU::set_interrupt_flag();
    CPU::idle_loop();
}
