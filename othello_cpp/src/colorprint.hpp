//==========================================================
// Colorprint header
// Interface for colored printing
// Akseli Lukkarila
// 2019-2023
//==========================================================

#pragma once

#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include <cctype>    // std::isspace
#include <iostream>  // std::cout
#include <string>    // std::string
#include <utility>   // std::pair

/// Split a string into the leading whitespace and the rest of the string.
inline std::pair<std::string, std::string> split_leading_whitespace(const std::string& message)
{
    // Find the index of the first non-whitespace character.
    size_t indent_size = 0;
    while (indent_size < message.length() && (std::isspace(message[indent_size]) != 0)) {
        ++indent_size;
    }
    return {message.substr(0, indent_size), message.substr(indent_size)};
}

/// Format string with colour using fmt.
template<typename T> inline std::string get_color(const T& object, fmt::terminal_color color)
{
    return fmt::format(fmt::fg(color), "{}", object);
}

/// Print text with colour.
template<typename T>
inline void print_color(const T& object, fmt::terminal_color color = fmt::terminal_color::white)
{
    fmt::print(fmt::fg(color), object);
}

/// Print bold text.
template<typename T>
inline void print_bold(const T& object, fmt::terminal_color color = fmt::terminal_color::white)
{
    fmt::print(fmt::emphasis::bold | fmt::fg(color), object);
}

/// Print error message with red colour.
inline void print_error(const std::string& message)
{
    auto [indent, text] = split_leading_whitespace(message);
    fmt::print(fmt::fg(fmt::terminal_color::red), "{}Error: {}\n", indent, text);
}

/// Print warning message with yellow colour.
inline void print_warn(const std::string& message)
{
    auto [indent, text] = split_leading_whitespace(message);
    fmt::print(fmt::fg(fmt::terminal_color::yellow), "{}Warning: {}\n", indent, text);
}

// Fallback with ANSI escape codes for stringstream

namespace ansi
{
// Foreground
[[maybe_unused]] static constexpr auto ANSI_BLACK = "\033[0;30m";
[[maybe_unused]] static constexpr auto ANSI_BLUE = "\033[0;34m";
[[maybe_unused]] static constexpr auto ANSI_CYAN = "\033[0;36m";
[[maybe_unused]] static constexpr auto ANSI_GREEN = "\033[0;32m";
[[maybe_unused]] static constexpr auto ANSI_MAGENTA = "\033[0;35m";
[[maybe_unused]] static constexpr auto ANSI_RED = "\033[0;31m";
[[maybe_unused]] static constexpr auto ANSI_WHITE = "\033[0;37m";
[[maybe_unused]] static constexpr auto ANSI_YELLOW = "\033[0;33m";

// Foreground bold
[[maybe_unused]] static constexpr auto ANSI_BOLD_BLACK = "\033[1;30m";
[[maybe_unused]] static constexpr auto ANSI_BOLD_BLUE = "\033[1;34m";
[[maybe_unused]] static constexpr auto ANSI_BOLD_CYAN = "\033[1;36m";
[[maybe_unused]] static constexpr auto ANSI_BOLD_GREEN = "\033[1;32m";
[[maybe_unused]] static constexpr auto ANSI_BOLD_MAGENTA = "\033[1;35m";
[[maybe_unused]] static constexpr auto ANSI_BOLD_RED = "\033[1;31m";
[[maybe_unused]] static constexpr auto ANSI_BOLD_WHITE = "\033[1;37m";
[[maybe_unused]] static constexpr auto ANSI_BOLD_YELLOW = "\033[1;33m";

[[maybe_unused]] static constexpr auto ANSI_RESET = "\033[0m";

// Stream manipulators for easy ANSI color usage with stringstreams.
// Example usage: std::cout << ansi::red << "example" << ansi::reset << std::endl;

template<class T, class Traits>
constexpr std::basic_ostream<T, Traits>& reset(std::basic_ostream<T, Traits>& stream)
{
    return stream << ANSI_RESET;
}

template<class T, class Traits>
constexpr std::basic_ostream<T, Traits>& black(std::basic_ostream<T, Traits>& stream)
{
    return stream << ANSI_BLACK;
}

template<class T, class Traits>
constexpr std::basic_ostream<T, Traits>& blue(std::basic_ostream<T, Traits>& stream)
{
    return stream << ANSI_BLUE;
}

template<class T, class Traits>
constexpr std::basic_ostream<T, Traits>& red(std::basic_ostream<T, Traits>& stream)
{
    return stream << ANSI_RED;
}

template<class T, class Traits>
constexpr std::basic_ostream<T, Traits>& yellow(std::basic_ostream<T, Traits>& stream)
{
    return stream << ANSI_YELLOW;
}

template<class T, class Traits>
constexpr std::basic_ostream<T, Traits>& cyan(std::basic_ostream<T, Traits>& stream)
{
    return stream << ANSI_CYAN;
}

template<class T, class Traits>
constexpr std::basic_ostream<T, Traits>& magenta(std::basic_ostream<T, Traits>& stream)
{
    return stream << ANSI_MAGENTA;
}

template<class T, class Traits>
constexpr std::basic_ostream<T, Traits>& green(std::basic_ostream<T, Traits>& stream)
{
    return stream << ANSI_GREEN;
}

}  // namespace ansi
