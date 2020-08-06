//==========================================================
// Colorprint util
// Interface for colored printing
// Akseli Lukkarila
//==========================================================

#pragma once
#include <iostream>  // std::cout
#include <string>    // std::string

#include <fmt/ostream.h>
#include <fmt/color.h>

template<typename T>
std::string get_color(const T& object, fmt::color color) {
    return fmt::format(fmt::fg(color), "{}", object);
}

template<typename T>
void print_color(const T& object, fmt::color color=fmt::color::white) {
    fmt::print(fmt::fg(color), object);
}

template<typename T>
void print_bold(const T& object, fmt::color color=fmt::color::white) {
    fmt::print(fmt::emphasis::bold | fmt::fg(color), object);
}

template<typename T>
void print_error(const T& message, const int& indent=0) {
    auto whitespace = std::string(indent, ' ');
    fmt::print(fmt::fg(fmt::color::red), "{}{} {}",
               whitespace,
               get_color("Error:", fmt::color::red),
               message);
}
