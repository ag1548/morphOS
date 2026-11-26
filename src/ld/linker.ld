ENTRY(_start)

SECTIONS
{
    reserved_start = .;
    . = 1M;
    reserved_end = .;

    kernel_mem_start = .;

	.text       : ALIGN(4K) { *(.multiboot) *(.text*) }
	.rodata     : ALIGN(4K) { *(.rodata*) }
	.data       : ALIGN(4K) { *(.data*) }

    .init_array : ALIGN(4K) {
        __init_array_start = .;
        *(.init_array*)
        __init_array_end = .;
    }

	.bss        : ALIGN(4K) { *(.vm_structs) *(COMMON) *(.bss*) }

    . = ALIGN(4K);
    kernel_mem_end = .;
}


/*
PHDRS
{
    text       PT_LOAD FLAGS(0x5);
    rodata     PT_LOAD FLAGS(0x4);
    data       PT_LOAD FLAGS(0x6);
    init_array PT_LOAD FLAGS(0x6);
    bss        PT_LOAD FLAGS(0x6);
}

SECTIONS
{
    reserved_start = .;
    . = 1M;
    reserved_end = .;

    kernel_mem_start = .;

	.text       : ALIGN(4K) { *(.multiboot) *(.text*) }           :text
	.rodata     : ALIGN(4K) { *(.rodata*) }                       :rodata
	.data       : ALIGN(4K) { *(.data*) }                         :data

    .init_array : ALIGN(4K) {
        __init_array_start = .;
        *(.init_array*)
        __init_array_end = .;
    } :init_array

	.bss        : ALIGN(4K) { *(.vm_structs) *(COMMON) *(.bss*) } :bss

    . = ALIGN(4K);
    kernel_mem_end = .;
}
 */
