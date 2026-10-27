#!/bin/bash

set -e

export SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)
echo $SCRIPT_DIR
pushd $SCRIPT_DIR

    ./build_binutils_gcc_gdb_tree.sh
    ./build_cross_compiler.sh
    ./build_kernel_and_run.sh

popd
