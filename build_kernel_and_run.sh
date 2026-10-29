#!/bin/bash

set -e

./cross/bin/i686-elf-g++ \
    src/boot.S           \
    src/kernel.cpp       \
    -T src/linker.ld     \
    -o kernel            \
    -std=c++17           \
    -ffreestanding       \
    -O2                  \
    -nostdlib            \
    -nodefaultlibs       \
    -fno-exceptions      \
    -lgcc                \
    -Wall                \
    -Wextra              \

if grub-file --is-x86-multiboot kernel; then
  echo multiboot confirmed
else
  echo the file is not multiboot
  exit 1
fi

make_grub_kernel_iso() {
    mkdir -p isodir/boot/grub
    cp src/grub.cfg isodir/boot/grub/grub.cfg
    cp kernel isodir/boot/kernel
    grub-mkrescue -o kernel.iso isodir
    rm -rf isodir
}

run_cdrom() {
    make_grub_kernel_iso
    QEMU_OPTS=(
        -cdrom kernel.iso
        -nic none
    )
    qemu-system-i386 "${QEMU_OPTS[@]}"
}

run_kernel() {
    QEMU_OPTS=(
        -kernel kernel
        -enable-kvm               # Use KVM support for direct hardware execution
        -s                        # Shorthand for -gdb tcp::1234
        -d cpu_reset,guest_errors # show CPU state before CPU resets, log when the guest OS does something invalid (eg accessing a non-existent register)
        -debugcon stdio           # Redirect the debug console to host device dev
        -device VGA,vgamem_mb=64
        -nic none
    )
    qemu-system-i386 "${QEMU_OPTS[@]}"
}

# run_cdrom
run_kernel
