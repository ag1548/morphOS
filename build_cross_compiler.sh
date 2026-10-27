#!/bin/bash

# See https://wiki.osdev.org/Bare_Bones
# See https://wiki.osdev.org/GCC_Cross-Compiler

set -e

export SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
echo $SCRIPT_DIR
pushd $SCRIPT_DIR

    export PREFIX="$SCRIPT_DIR/cross"
    rm -rf cross

    mkdir -p "$PREFIX/bin"

    export TARGET=i686-elf
    export PATH="$PREFIX/bin:$PATH"

    pushd binutils

        rm -rf build-binutils
        mkdir build-binutils
        cd build-binutils

        ../binutils-2.47/configure        \
            --target=$TARGET              \
            --prefix="$PREFIX"            \
            --with-sysroot                \
            --disable-nls                 \
            --disable-werror              \
            --enable-default-execstack=no \

        make -j $(( $(nproc) / 2 ))
        make -j $(( $(nproc) / 2 )) install
    popd

    pushd gdb

        rm -rf build-gdb
        mkdir build-gdb
        cd build-gdb

        ../gdb-18.1/configure  \
            --target=$TARGET   \
            --prefix="$PREFIX" \
            --disable-werror   \

        make -j $(( $(nproc) / 2 )) all-gdb
        make -j $(( $(nproc) / 2 )) install-gdb
    popd

    pushd gcc

        if ! command -v "$TARGET-as" &> /dev/null; then
            echo "$TARGET-as is not in the PATH" >&2
            exit 1
        fi

        rm -rf build-gcc
        mkdir build-gcc
        cd build-gcc

        ../gcc-16.2.0/configure        \
            --target=$TARGET           \
            --prefix="$PREFIX"         \
            --disable-nls              \
            --enable-languages=c,c++   \
            --without-headers          \
            --enable-initfini-array    \
            --disable-hosted-libstdcxx \

        make -j $(( $(nproc) / 2 )) all-gcc
        make -j $(( $(nproc) / 2 )) all-target-libgcc
        make -j $(( $(nproc) / 2 )) all-target-libstdc++-v3

        make -j $(( $(nproc) / 2 )) install-gcc
        make -j $(( $(nproc) / 2 )) install-target-libgcc
        make -j $(( $(nproc) / 2 )) install-target-libstdc++-v3


    popd

popd
