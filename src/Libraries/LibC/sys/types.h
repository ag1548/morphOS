#pragma once

static_assert(sizeof(unsigned char) == 1);
static_assert(sizeof(unsigned short) == 2);
static_assert(sizeof(unsigned int) == 4);
static_assert(sizeof(unsigned long) == 4);
static_assert(sizeof(unsigned long long) == 8);

typedef unsigned int size_t;
typedef signed int   ssize_t;
