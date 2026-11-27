#pragma once

#define kprintf(...)            \
    do {                        \
        printf(__VA_ARGS__);    \
        debug_out(__VA_ARGS__); \
    } while (0)

extern "C" void printf(const char *format, ...);
void            debug_out(const char *format, ...);

#define KB                     1024u
#define MB                     (KB * KB)
#define GB                     (KB * KB * KB)

#define BITS_PER_UNSIGNED_CHAR 8u
