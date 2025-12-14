#pragma once

#include "pch.h"

#include "types.h"

namespace print {
    extern bool line_printed;
    extern byte print_level;

    void increase_level();
    void decrease_level();

    void info(std::string text);

    void loading(std::string info);
    void loaded(std::string info);
    void loaded();

    void warning(std::string text);
    void warning(std::string text, int code);
    void warning(std::string text, std::string info);

    void error(std::string text);
    void error(std::string text, int code);
    void error(std::string text, std::string info);
}