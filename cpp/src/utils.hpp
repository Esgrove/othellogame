//==========================================================
// Utils header
// Helper utilities for Othello game
// Akseli Lukkarila
// 2019-2025
//==========================================================

#pragma once
#include "colorprint.hpp"
#include "models.hpp"
#include "version.hpp"

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
/// Returns the opposing disk colour.
constexpr Disk opponent(const Disk& disk)
{
    switch (disk) {
        case Disk::white:
            return Disk::black;
        case Disk::black:
            return Disk::white;
        case Disk::empty:
            [[fallthrough]];
        default:
            return Disk::empty;
    }
}

/// Returns the print colour for the given Disk.
fmt::terminal_color disk_color(const Disk& disk);

/// Returns string character representing board status (black, white, empty).
std::string board_char(const Disk& disk, bool color = true);

/// Returns the disk formatted as a coloured string.
std::string disk_string(const Disk& disk);

/// Calculate SHA256 hash for the given string.
std::string calculate_sha256(const std::string& text);

inline std::ostream& operator<<(std::ostream& out, const Disk& disk)
{
    return out << disk_string(disk);
}

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

/// Print version string
inline void print_version()
{
    // TODO: change to C++23 std once supported with all implementations
    // std::println("{}", version::VERSION_STRING);
    fmt::print("{}\n", version::VERSION_STRING);
}
}  // namespace othello
