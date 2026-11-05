#!/bin/bash

shopt -s globstar
clang-format src/**/*.cpp src/**/*.h -i -Werror
