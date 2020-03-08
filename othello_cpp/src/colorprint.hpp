//==========================================================
// Colorprint util
// Interface for termcolor colored printing
// Akseli Lukkarila
//==========================================================

#pragma once
#include <iostream>  // std::cout
#include <string>	 // std::string

#include <termcolor/termcolor.hpp>

/// Color options
enum class Color {
    GREY,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
};

/// Outputs text to std::cout with given color. Does not add a linebreak to the end.
void print_color(const std::string& text, Color color=Color::WHITE);
