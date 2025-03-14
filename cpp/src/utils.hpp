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

#include <iostream>  // cout, cin
#include <sstream>   // stringstream
#include <string>    // string

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
template<typename T> void print(T object, const bool newline = true, std::ostream& out = std::cout)
{
    out << object;
    if (newline) {
        out << "\n";
    }
}

/// Convert object to string using a stringstream.
///
/// Requires that the stream insertion operator `<<` has been implemented for the given object.
/// Workaround for custom types without formatter specialization for fmt.
template<typename T>
    requires requires(T obj) { std::declval<std::ostream&>() << obj; }
std::string to_string(const T& object)
{
    std::ostringstream stream;
    stream << object;
    return stream.str();
}

/// Print version string
inline void print_version()
{
    fmt::print(
        "{} {} {} {} {}\n",
        version::APP_NAME,
        version::VERSION_NUMBER,
        version::TIMESTAMP,
        version::BRANCH,
        version::COMMIT
    );
}
}  // namespace othello
