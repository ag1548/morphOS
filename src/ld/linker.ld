ENTRY(_start)

SECTIONS
{
    . = 1M;
    kernel_mem_start = .;
	.text   : ALIGN(4K) { *(.multiboot) *(.text*) }
	.rodata : ALIGN(4K) { *(.rodata*) }
	.data   : ALIGN(4K) { *(.data*) }
	.bss    : ALIGN(4K) { *(.vm_structs) *(COMMON) *(.bss*) }
    kernel_mem_end = .;
}
