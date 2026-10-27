#!/bin/bash

# See https://wiki.osdev.org/Bare_Bones
# See https://wiki.osdev.org/GCC_Cross-Compiler

set -e

export SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
echo $SCRIPT_DIR
pushd $SCRIPT_DIR

    mkdir binutils
    pushd binutils

        wget https://ftp.gnu.org/gnu/binutils/binutils-2.47.tar.bz2
        wget https://ftp.gnu.org/gnu/binutils/binutils-2.47.tar.bz2.sig
        if ! gpg --verify binutils-2.47.tar.bz2.sig binutils-2.47.tar.bz2 &> /dev/null; then
            echo "Could not verify binutils!"
            exit 1;
        else
            echo "Verified binutils!"
        fi

        tar -xf binutils-2.47.tar.bz2
        echo "binutils done!"

    popd

    mkdir gdb
    pushd gdb

        wget https://ftp.gnu.org/gnu/gdb/gdb-18.1.tar.gz
        wget https://ftp.gnu.org/gnu/gdb/gdb-18.1.tar.gz.sig
        if ! gpg --verify gdb-18.1.tar.gz.sig gdb-18.1.tar.gz &> /dev/null; then
            echo "Could not verify gdb!"
            exit 1;
        else
            echo "Verified gdb!"
        fi

        tar -xf gdb-18.1.tar.gz
        echo "gdb done!"

    popd

    mkdir gcc
    pushd gcc

        wget https://ftp.gnu.org/gnu/gcc/gcc-16.2.0/gcc-16.2.0.tar.gz
        wget https://ftp.gnu.org/gnu/gcc/gcc-16.2.0/gcc-16.2.0.tar.gz.sig
        if ! gpg --verify gcc-16.2.0.tar.gz.sig gcc-16.2.0.tar.gz &> /dev/null; then
            echo "Could not verify gcc!"
            exit 1;
        else
            echo "Verified gcc!"
        fi

        tar -xf gcc-16.2.0.tar.gz
        echo "gcc done!"

    popd

popd
