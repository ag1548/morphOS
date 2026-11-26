#pragma once

#include <sys/types.h>

template<unsigned int N>
class Bitmap {

public:
    unsigned int size() { return N; }

private:
    unsigned char bits[N];
};
