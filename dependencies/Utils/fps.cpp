#include "fps.h"

#include "convert.h"
#include <format>
#include <fstream>
#include <string>
#include <regex>

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


void bench_part::start()
{
    if (enabled_)
        start_ = std::chrono::steady_clock::now();
}

float bench_part::stop()
{
    if (enabled_) {
        stop_ = std::chrono::steady_clock::now();
        delta_ = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_ - start_).count();

        return delta_;
    }
    return 0.0f;
}

float bench_part::get_delta()
{
    return delta_;
}
void bench_part::enable()
{
    enabled_ = true;
}

void bench_part::disable()
{
    enabled_ = false;
}

bool bench_part::is_enabled()
{
    return enabled_;
}


std::map<std::string, bench_part> bench::parts;
std::chrono::steady_clock::time_point bench::global_start_, bench::global_stop_;
std::map<std::string, float> bench::aver_vals;
std::string bench::bench_results = "";
int bench::frame_counter = 0;
bool bench::enabled_ = false;

void bench::add(std::string tag)
{
    parts[tag] = bench_part();
}

bench_part& bench::get(std::string tag)
{
    return parts.at(tag);
}

std::string bench::get_info_str(bool per)
{
    constexpr int wait = 30;
    if (enabled_) {
        if (frame_counter >= wait) {
            frame_counter = 0;

            bench_results.clear();
            if (!per) {
                for (auto& [key, val] : parts) {
                    bench_results += std::format("{}: {}ns\n", key, aver_vals[key] / wait);
                }
            }
            else {
                auto delta_ = abs(std::chrono::duration_cast<std::chrono::nanoseconds>(global_stop_ - global_start_).count());
                bench_results = std::format("Time: {}ns\n", delta_);
                for (auto& [key, val] : parts) {
                    bench_results += std::format("{}: {}% ({}ns)\n", key, round((aver_vals[key] / (delta_ * wait)) * 100), aver_vals[key] / wait);
                }
            }
            bench_results += std::format("Ram usage: {}\n", get_mem_usage_s());

            for (auto& [key, val] : aver_vals) {
                aver_vals[key] = 0.0f;
            };

            return bench_results;
        }

        for (auto& [key, val] : parts) {
            aver_vals[key] += val.get_delta();
        } frame_counter++;
    }

    return bench_results;
}

void bench::start()
{
    if (enabled_)
        global_start_ = std::chrono::steady_clock::now();
}

void bench::stop()
{
    if (enabled_)
        global_stop_ = std::chrono::steady_clock::now();
}

void bench::enable()
{
    for (auto& [key, val] : parts) {
        val.enable();
    }
    enabled_ = true;
}

void bench::disable()
{
    for (auto& [key, val] : parts) {
        val.disable();
    }
    enabled_ = false;
}

bool bench::is_enabled()
{
    return enabled_;
}

std::string bench::get_mem_usage_s()
{
    #ifdef __linux__
    std::ifstream file("/proc/self/status");
    std::string res;
       while (std::getline(file, res)) {
        if (res.compare(0, 6, "VmRSS:") == 0) {
            return res.substr(7);
        }
    }
    #endif
    return "1";
}

int bench::get_mem_usage_i()
{
    auto str = get_mem_usage_s();
    
    std::regex pattern(R"(\d+)");
    std::smatch matches;
    
    if (std::regex_search(str, matches, pattern)) {
        auto res_str = matches[1].str();
        return std::stoi(res_str);
    }

    return 1;
}

