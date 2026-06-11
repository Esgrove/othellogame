//==========================================================
// Utils header
// Helper utilities for Othello game
// Akseli Lukkarila
// 2019-2026
//==========================================================

#pragma once
#include "colorprint.hpp"

#include <concepts>
#include <iostream>  // cout, cin
#include <sstream>   // stringstream
#include <string>    // string

// Concept: checks if T can be streamed into std::ostream
template<typename T>
concept Streamable = requires(std::ostream& os, T a) {
    { os << a } -> std::same_as<std::ostream&>;
};

namespace othello
{
/// Calculate SHA256 hash for the given string.
std::string calculate_sha256(const std::string& text);

/// Print an object to stream (default is std::cout).
///
/// Requires that the stream insertion operator `<<` has been implemented for the given object.
template<Streamable T>
void print(T object, const bool newline = true, std::ostream& out = std::cout)
{
    out << object;
    if (newline) {
        out << "\n";
    }
}

/// Convert an object to string using a stringstream.
///
/// Requires that the stream insertion operator `<<` has been implemented for the given object.
/// Workaround for custom types without formatter specialisation for fmt.
template<Streamable T>
std::string to_string(const T& object)
{
    std::ostringstream stream;
    stream << object;
    return stream.str();
}

/// Remove leading and trailing whitespace from the given string.
[[nodiscard]] inline std::string trim(const std::string& text)
{
    constexpr auto whitespace = " \t\n\r\f\v";
    const auto start = text.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        return "";
    }
    const auto end = text.find_last_not_of(whitespace);
    return text.substr(start, end - start + 1);
}
}  // namespace othello
