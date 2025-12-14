#include "print.h"

#ifndef _DEBUG
void print::increase_level() {}
void print::decrease_level() {}
void print::info(std::string text) {}

void print::loading(std::string info) {}

void print::loaded(std::string info) {}

void print::loaded() {}

void print::warning(std::string text) {}

void print::warning(std::string text, int code) {}

void print::warning(std::string text, std::string info) {}

void print::error(std::string text) {}

void print::error(std::string text, int code) {}

void print::error(std::string text, std::string info) {}
#endif

#ifdef _DEBUG

#define PRINT_LINE std::cout << "------------------" << std::endl

bool print::line_printed = false;
byte print::print_level = 0;

void print::increase_level() {
    print_level++;
}
void print::decrease_level() {
    print_level--;
}

void print::info(std::string text) {
    if (line_printed) {
        line_printed = false;
    }
    std::cout << std::string(print_level, '-') << std::format("info: {}", text) << std::endl;
}

void print::loading(std::string info) {
    if (line_printed) {
        line_printed = false;
    }
    std::cout << std::string(print_level, '-') << std::format("loading: {}", info) << std::endl;
}

void print::loaded(std::string info) {
    if (line_printed) {
        line_printed = false;
    }
    std::cout << std::string(print_level, '-') << std::format("loaded: {}", info) << std::endl;
}

void print::loaded()
{
    if (line_printed) {
        line_printed = false;
    }
    std::cout << std::string(print_level, '-') << "loaded" << std::endl;
}

void print::warning(std::string text) {
    if (!line_printed) {
        PRINT_LINE;
        line_printed = true;
    }
    std::cout << std::string(print_level, '-') << std::format("Warning: {}", text) << std::endl;
    PRINT_LINE;
}

void print::warning(std::string text, int code) {
    if (!line_printed) {
        PRINT_LINE;
        line_printed = true;
    }

    std::cout << std::string(print_level, '-') << std::format("Warning: {}", text) << std::endl;
    std::cout << std::string(print_level, '-') << std::format("Info Code: {}", code) << std::endl;
    PRINT_LINE;
}

void print::warning(std::string text, std::string info) {
    if (!line_printed) {
        PRINT_LINE;
        line_printed = true;
    }

    std::cout << std::string(print_level, '-') << std::format("Warning: {}", text) << std::endl;
    std::cout << std::string(print_level, '-') << std::format("More Info: {}", info) << std::endl;
    PRINT_LINE;
}

void print::error(std::string text) {
    if (!line_printed) {
        PRINT_LINE;
        line_printed = true;
    }
    std::cout << std::string(print_level, '-') << std::format("ERROR: {}", text) << std::endl;
    PRINT_LINE;
}

void print::error(std::string text, int code) {
    if (!line_printed) {
        PRINT_LINE;
        line_printed = true;
    }

    std::cout << std::string(print_level, '-') << std::format("ERROR: {}", text) << std::endl;
    std::cout << std::string(print_level, '-') << std::format("ERROR CODE: {}", code) << std::endl;
    PRINT_LINE;
}

void print::error(std::string text, std::string info) {
    if (!line_printed) {
        PRINT_LINE;
        line_printed = true;
    }

    std::cout << std::string(print_level, '-') << std::format("ERROR: {}", text) << std::endl;
    std::cout << std::string(print_level, '-') << std::format("ERROR INFO: {}", info) << std::endl;
    PRINT_LINE;
}

#undef PRINT_LINE
#endif