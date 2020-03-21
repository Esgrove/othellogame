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

/// Outputs text to stream with given color. Does not add a linebreak to the end.
template<typename T>
void print_color(const T& object, Color color=Color::WHITE, std::ostream& out=std::cout) {
    // TODO: figure out a nicer way to do this...
    switch (color) {
        case Color::GREY:
            out << termcolor::grey;
            break;
        case Color::RED:
            out << termcolor::red;
            break;
        case Color::GREEN:
            out << termcolor::green;
            break;
        case Color::YELLOW:
            out << termcolor::yellow;
            break;
        case Color::BLUE:
            out << termcolor::blue;
            break;
        case Color::MAGENTA:
            out << termcolor::magenta;
            break;
        case Color::CYAN:
            out << termcolor::cyan;
            break;
        case Color::WHITE:
            out << termcolor::white;
            break;
    }
    out << object << termcolor::reset;
}

template<typename T>
void print_bold(const T& object, Color color=Color::WHITE, std::ostream& out=std::cout) {
    out << termcolor::bold;
    print_color(object, color);
}

template<typename T>
void print_error(const T& object, std::ostream& out=std::cout) {
    print_color(object, Color::RED);
}