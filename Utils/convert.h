#pragma once
#include "pch.h"

namespace convert {
    int f2i(float val);
    float i2f(int val);

    float delta2fps(float delta);
    float fps2delta(float fps);

    float ns2ms(float time_ns);
}
