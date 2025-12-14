#include "convert.h"

int convert::f2i(float val)
{
    return static_cast<int>(round(val));
}

float convert::i2f(int val)
{
    return static_cast<float>(val);
}

float convert::delta2fps(float delta)
{
    if (delta > 0) {
        return 1000000000.0f / delta;
    }
    else {
        return std::numeric_limits<float>::max();
    }
}

float convert::fps2delta(float fps)
{
    if (fps > 0) {
        return 1000000000.0f / fps;
    }
    else {
        return std::numeric_limits<float>::max();
    }
}

float convert::ns2ms(float time_ns)
{
    return time_ns / 1000000;
}