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
    /// <summary>
    /// Return delta time in nanoseconds
    /// </summary>
    /// <returns></returns>
    static float delta();
    static float mult();

    template <typename T>
    static inline T synch(T val) {
        return val * mult();
    }
};

class bench_part {
private:
    std::chrono::steady_clock::time_point start_, stop_;
    float delta_ = 1.0f;
    bool enabled_ = false;
public:
    bench_part() = default;
    ~bench_part() = default;
    
    void start();
    /// <summary>
    /// Return delta time in nanoseconds and reset timer
    /// </summary>
    /// <returns></returns>
    float stop();

    float get_delta();

    void enable();
    void disable();
    bool is_enabled();
};

class bench {
private:

    static std::map<std::string, bench_part> parts;
    static std::map<std::string, float> aver_vals;
    static std::chrono::steady_clock::time_point global_start_, global_stop_;
    static std::string bench_results;
    static int frame_counter;

    static bool enabled_;

    bench() = default;
    ~bench() = default;
public:
    static bench& get_instance() {
        static bench inst;
        start();
        return inst;
    }

    static void add(std::string tag);
    static bench_part& get(std::string tag);

    static std::string get_info_str(bool per = false);

    static void start();
    static void stop();

    static void enable();
    static void disable();
    static bool is_enabled();
};