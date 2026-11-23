/*

CPU
===
EAX=00000000 EBX=00000000 ECX=00000000 EDX=00000663
ESI=00000000 EDI=00000000 EBP=00000000 ESP=00000000
EIP=0000fff0 EFL=00000002 [-------] CPL=0 II=0 A20=1 SMM=0 HLT=0
ES =0000 00000000 0000ffff 00009300
CS =f000 ffff0000 0000ffff 00009b00
SS =0000 00000000 0000ffff 00009300
DS =0000 00000000 0000ffff 00009300
FS =0000 00000000 0000ffff 00009300
GS =0000 00000000 0000ffff 00009300
LDT=0000 00000000 0000ffff 00008200
TR =0000 00000000 0000ffff 00008b00
GDT=     00000000 0000ffff
IDT=     00000000 0000ffff
CR0=60000010 CR2=00000000 CR3=00000000 CR4=00000000
DR0=00000000 DR1=00000000 DR2=00000000 DR3=00000000
DR6=ffff0ff0 DR7=00000400
CCS=00000000 CCD=00000000 CCO=DYNAMIC
EFER=0000000000000000
FCW=037f FSW=0000 [ST=0] FTW=00 MXCSR=00001f80
FPR0=0000000000000000 0000 FPR1=0000000000000000 0000
FPR2=0000000000000000 0000 FPR3=0000000000000000 0000
FPR4=0000000000000000 0000 FPR5=0000000000000000 0000
FPR6=0000000000000000 0000 FPR7=0000000000000000 0000
XMM00=0000000000000000 0000000000000000 XMM01=0000000000000000 0000000000000000
XMM02=0000000000000000 0000000000000000 XMM03=0000000000000000 0000000000000000
XMM04=0000000000000000 0000000000000000 XMM05=0000000000000000 0000000000000000
XMM06=0000000000000000 0000000000000000 XMM07=0000000000000000 0000000000000000

*/

#include "CPU.h"

#include "common.h"
#include "ControlRegisters.h"

namespace CPU {

[[noreturn]] void hang() {
    asm volatile("cli" ::: "memory");
    for (;;) {
        asm volatile("hlt" ::: "memory");
    }
}

/*

Pg 161 of Intel 64 IA-32 Arch Manual.pdf

"PUSHA pushes the values in all the general-purpose registers on the stack in the following order:
EAX, ECX, EDX, EBX, ESP (the value prior to executing the PUSHA instruction), EBP, ESI, and EDI."

*/
extern "C" __attribute__((regparm(1))) void do_print_cpu(RegisterDump &reg) {

    kprintf("eax: %u\n", reg.eax);
    kprintf("ebx: %u\n", reg.ebx);
    kprintf("ecx: %u\n", reg.ecx);
    kprintf("edx: %u\n", reg.edx);

    kprintf("ebp: %u\n", reg.ebp);
    kprintf("esp: %u\n", reg.esp);

    kprintf("edi: %u\n", reg.edi);
    kprintf("esi: %u\n", reg.esi);
}
static void print_cpu() {
    // asm volatile(
    //     "    movl $0xff, %%eax\n"
    //     "    pusha\n"
    //     "    mov %%esp, %%eax\n"
    //     "    call do_print_cpu\n"
    //     "    popa\n"
    //     :
    //     :
    //     : "eax"
    //     );

    asm volatile(
        "    pusha\n"
        "    mov %esp, %eax\n"
        "    call do_print_cpu\n"
        "    popa\n");
}

[[noreturn]] void print_control_registers_and_hang() {

    print_cpu();

    unsigned int cr0;
    unsigned int cr2;
    unsigned int cr3;
    unsigned int cr4;

    asm volatile("mov %%cr0, %%eax" : "=a"(cr0));
    asm volatile("mov %%cr2, %%eax" : "=a"(cr2));
    asm volatile("mov %%cr3, %%eax" : "=a"(cr3));
    asm volatile("mov %%cr4, %%eax" : "=a"(cr4));

    kprintf("CR0=%x CR2=%x CR3=%x CR4=%x\n", cr0, cr2, cr3, cr4);
    kprintf("\n");

    print_CR0_dump(ControlRegister0Dump(ControlRegister0(cr0)));
    print_CR2_dump(ControlRegister2(cr2));
    print_CR3_dump(ControlRegister3Dump(ControlRegister3(cr3)));
    print_CR4_dump(ControlRegister4Dump(ControlRegister4(cr4)));

    hang();
}

} // namespace CPU
