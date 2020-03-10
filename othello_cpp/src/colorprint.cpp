#include "colorprint.hpp"

void print_color(const std::string& text, Color color) {
    // TODO: figure out a nicer way to do this...
    switch (color) {
    case Color::GREY:
        std::cout << termcolor::grey;
        break;
    case Color::RED:
        std::cout << termcolor::red;
        break;
    case Color::GREEN:
        std::cout << termcolor::green;
        break;
    case Color::YELLOW:
        std::cout << termcolor::yellow;
        break;
    case Color::BLUE:
        std::cout << termcolor::blue;
        break;
    case Color::MAGENTA:
        std::cout << termcolor::magenta;
        break;
    case Color::CYAN:
        std::cout << termcolor::cyan;
        break;
    case Color::WHITE:
        std::cout << termcolor::white;
        break;
    }
    std::cout << text << termcolor::reset;
}

void print_bold(const std::string& text, Color color) {
    std::cout << termcolor::bold;
    print_color(text, color);
}
