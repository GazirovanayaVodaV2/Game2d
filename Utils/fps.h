#pragma once

#include "pch.h"

class fps
{
private:
    static std::chrono::steady_clock::time_point start, stop;
    static float current_fps;
    static float delta_;

    fps();
    ~fps() = default;
public:
    static fps& get_instance() {
        static fps inst;
        return inst;
    }

    static void update();

    static float get();
    static float delta();
    static float mult();

    template <typename T>
    static inline T synch(T val) {
        return val * mult();
    }
};