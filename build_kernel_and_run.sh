#!/bin/bash

set -e

./cross/bin/i686-elf-gcc \
    src/boot.S           \
    src/kernel.c         \
    -T src/linker.ld     \
    -o kernel            \
    -std=gnu99           \
    -ffreestanding       \
    -O2                  \
    -nostdlib            \
    -lgcc                \
    -Wall                \
    -Wextra              \

if grub-file --is-x86-multiboot kernel; then
  echo multiboot confirmed
else
  echo the file is not multiboot
  exit 1
fi

# Optional kernel iso
make_grub_kernel_iso() {

    mkdir -p isodir/boot/grub
    cp src/grub.cfg isodir/boot/grub/grub.cfg
    cp kernel isodir/boot/kernel

    grub-mkrescue -o kernel.iso isodir

    rm -rf isodir

}

# make_grub_kernel_iso
# qemu-system-i386 -cdrom kernel.iso -nic none

QEMU_OPTS=(
    -kernel kernel
    -enable-kvm               # Use KVM support for direct hardware execution
    -s                        # Shorthand for -gdb tcp::1234
    -d cpu_reset,guest_errors # show CPU state before CPU resets, log when the guest OS does something invalid (eg accessing a non-existent register)
)

qemu-system-i386 "${QEMU_OPTS[@]}"
