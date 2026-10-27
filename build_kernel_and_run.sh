#!/bin/bash

# See https://wiki.osdev.org/Bare_Bones
# See https://wiki.osdev.org/GCC_Cross-Compiler

set -e

mkdir objs

./cross/bin/i686-elf-as \
    src/boot.s          \
    -o objs/boot.o      \

./cross/bin/i686-elf-gcc \
    -c                   \
    src/kernel.c         \
    -o objs/kernel.o     \
    -std=gnu99           \
    -ffreestanding       \
    -O2                  \
    -Wall                \
    -Wextra              \

./cross/bin/i686-elf-gcc \
    -T src/linker.ld     \
    -o kernel            \
    -ffreestanding       \
    -O2                  \
    -nostdlib            \
    objs/boot.o          \
    objs/kernel.o        \
    -lgcc                \

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

}

# qemu-system-i386 -cdrom kernel.iso

qemu-system-i386 -kernel kernel
