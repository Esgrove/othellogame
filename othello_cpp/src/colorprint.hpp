//==========================================================
// Colorprint util
// Interface for termcolor colored printing
// Akseli Lukkarila
//==========================================================

#pragma once
#include <iostream>  // std::cout
#include <string>	 // std::string

#include <fmt/ostream.h>
#include <fmt/color.h>

/// Outputs text to st::cout with given color. Does not add a linebreak to the end.
template<typename T>
void print_color(const T& object, fmt::color color=fmt::color::white) {
    fmt::print(fmt::fg(color), object);
}

template<typename T>
void print_bold(const T& object, fmt::color color=fmt::color::white) {
    fmt::print(fmt::emphasis::bold | fmt::fg(color), object);
}

void print_error(const std::string& message) {
    fmt::print(fmt::fg(fmt::color::red), message);
}