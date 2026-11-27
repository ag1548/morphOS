#pragma once

#include <arch/i386/CPU.h>
#include <common.h>
#include <sys/types.h>

/*

Bitmap that uses `N` unsigned chars to represent bits.

*/
template<unsigned int N>
class Bitmap {

public:
    Bitmap() {
        for (unsigned int n = 0; n < N; n++) {
            bits[n] = 0;
        }
    }

    unsigned int size_chars() const { return N; }
    unsigned int size_bits() const { return N * BITS_PER_UNSIGNED_CHAR; }

    void clear_bit_at(unsigned int idx) {
        const unsigned int byte_idx        = idx >> 3;
        const unsigned int bit_idx_in_byte = idx & 0b111;

        assert_byte_index_in_range(byte_idx);

        bits[byte_idx] &= ~(1u << bit_idx_in_byte);
    }

    void set_bit_at(unsigned int idx) {
        const unsigned int byte_idx        = idx >> 3;
        const unsigned int bit_idx_in_byte = idx & 0b111;

        assert_byte_index_in_range(byte_idx);

        bits[byte_idx] |= (1u << bit_idx_in_byte);
    }

    unsigned int bit_at(unsigned int idx) const {
        const unsigned int byte_idx        = idx >> 3;
        const unsigned int bit_idx_in_byte = idx & 0b111;

        assert_byte_index_in_range(byte_idx);

        return (bits[byte_idx] >> bit_idx_in_byte) & 1u;
    }

private:
    void assert_byte_index_in_range(const unsigned int byte_idx) const {
        const bool byte_idx_out_of_range = byte_idx >= N;
        if (byte_idx_out_of_range) {
            kprintf("[[ ERROR ]]: bit_at(idx) byte index out of range: requested byte (%x) of [0, (%x))\n", byte_idx, N);
            CPU::hang();
        }
    }

    unsigned char bits[N];
};
