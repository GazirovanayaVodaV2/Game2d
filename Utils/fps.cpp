#include "fps.h"

#include "convert.h"

std::chrono::steady_clock::time_point fps::start;
std::chrono::steady_clock::time_point fps::stop;
float fps::current_fps = 60.0f;
float fps::delta_ = 1.0f / 60.0f;

fps::fps()
{
    start = std::chrono::steady_clock::now();
}

void fps::update()
{
    stop = std::chrono::steady_clock::now();
    auto delta__ = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
    start = std::chrono::steady_clock::now();

    current_fps = convert::delta2fps(delta__);
    delta_ = delta__;
}

float fps::delta()
{
    return delta_;
}

float fps::mult()
{
    return 60.0f / get();
}

float fps::get()
{
    return current_fps;
}