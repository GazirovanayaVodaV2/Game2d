#include "print.h"

static std::string format_for_info(std::string info)
{
    return std::format("info: {}", info);
}

static std::string format_for_loading(std::string text)
{
    return std::format("Loading: {}", text);
}

static std::string format_for_loaded(std::string text)
{
    return std::format("Loaded!: {}", text);
}

static std::string format_for_warning(std::string warning)
{
    return std::format("Warning: {}", warning);
}
static std::string format_for_warning(std::string warning, int code, int print_level=0)
{
    auto level = std::string(print_level, '-');
    std::string res = level + std::format("Warning: {}\n", warning);
    res += level + std::format("Info Code: {}", code);
    return res;
}
static std::string format_for_warning(std::string warning, std::string info, int print_level=0)
{
    auto level = std::string(print_level, '-');
    std::string res = level + std::format("Warning: {}\n", warning);
    res += level + std::format("More Info: {}", info);
    return res;
}

static std::string format_for_error(std::string text)
{
    return std::format("ERROR: {}", text);
}
static std::string format_for_error(std::string text, int code, int print_level = 0)
{
    auto level = std::string(print_level, '-');
    std::string res = level + std::format("ERROR: {}\n", text);
    res += level + std::format("ERROR CODE: {}", code);
    return res;
}
static std::string format_for_error(std::string text, std::string info, int print_level = 0)
{
    auto level = std::string(print_level, '-');
    std::string res = level + std::format("ERROR: {}\n", text);
    res += level + std::format("ERROR INFO: {}", info);
    return res;
}

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

void print::error(std::string text) {
    throw std::logic_error(format_for_error(text));
}

void print::error(std::string text, int code) {
    throw std::logic_error(format_for_error(text, code));
}

void print::error(std::string text, std::string info) {
    throw std::logic_error(format_for_error(text, info));
}
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
    std::cout << std::string(print_level, '-') << format_for_info(text) << std::endl;
}

void print::loading(std::string info) {
    if (line_printed) {
        line_printed = false;
    }
    std::cout << std::string(print_level, '-') << format_for_loading(info) << std::endl;
}

void print::loaded(std::string info) {
    if (line_printed) {
        line_printed = false;
    }
    std::cout << std::string(print_level, '-') << format_for_loaded(info) << std::endl;
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
    std::cout << std::string(print_level, '-') << format_for_warning(text) << std::endl;
    PRINT_LINE;
}

void print::warning(std::string text, int code) {
    if (!line_printed) {
        PRINT_LINE;
        line_printed = true;
    }

    std::cout << format_for_warning(text, code, print_level) << std::endl;
    PRINT_LINE;
}

void print::warning(std::string text, std::string info) {
    if (!line_printed) {
        PRINT_LINE;
        line_printed = true;
    }

    std::cout << format_for_warning(text, info, print_level) << std::endl;
    PRINT_LINE;
}

void print::error(std::string text) {
    if (!line_printed) {
        PRINT_LINE;
        line_printed = true;
    }
    auto err = format_for_error(text);
    std::cout << std::string(print_level, '-') << err << std::endl;
    PRINT_LINE;

    throw std::logic_error(err);
}

void print::error(std::string text, int code) {
    if (!line_printed) {
        PRINT_LINE;
        line_printed = true;
    }
    auto err = format_for_error(text, code, print_level);
    std::cout << err << std::endl;
    PRINT_LINE;
    throw std::logic_error(err);
}

void print::error(std::string text, std::string info) {
    if (!line_printed) {
        PRINT_LINE;
        line_printed = true;
    }

    auto err = format_for_error(text, info, print_level);
    std::cout << err << std::endl;
    PRINT_LINE;
    throw std::logic_error(err);
}

#undef PRINT_LINE
#endif