//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
//==========================================================

#pragma once
#include "colorprint.hpp"

#include <algorithm>  // transform
#include <iostream>   // cout, cin
#include <sstream>
#include <string>   // string
#include <utility>  // move
#include <vector>

namespace othello
{
enum class Disk { Black = -1, Empty = 0, White = 1 };

// Workaround for custom types without formatter specialization for fmt
template<typename T> inline std::string to_string(const T& object)
{
    std::ostringstream ss;
    ss << object;
    return ss.str();
}

/// Represents one square location on the board.
struct Square {
    Square() : x(0), y(0) {}
    Square(int x, int y) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& out, const Square& square)
    {
        return out << fmt::format("({},{})", square.x, square.y);
    }

    bool operator<(const Square& other) const { return x < other.x || (x <= other.x && y < other.y); }
    bool operator==(const Square& other) const { return x == other.x && y == other.y; }
    Square operator+(const Square& other) const { return {x + other.x, y + other.y}; }
    Square& operator+=(const Square& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    int x;
    int y;
};

/// Returns print color for given Disk.
inline fmt::color disk_color(const Disk& disk)
{
    using enum fmt::color;
    if (disk == Disk::Empty) {
        return white;
    }
    return disk == Disk::White ? cyan : magenta;
}

/// Returns string character representing board status (black, white, empty).
inline std::string board_char(const Disk& disk)
{
    if (disk == Disk::Empty) {
        return "_";
    }
    return get_color(disk == Disk::White ? "W" : "B", disk_color(disk));
}

/// Returns disk color as a string.
inline std::string disk_string(const Disk& disk)
{
    std::string text = disk == Disk::Empty ? "empty" : (disk == Disk::White ? "white" : "black");
    std::transform(text.begin(), text.end(), text.begin(), ::toupper);
    return get_color(text, disk_color(disk));
}

/// Returns the opponents disk color.
inline Disk other_disk(const Disk& disk)
{
    if (disk == Disk::Empty) {
        return Disk::Empty;
    }
    return disk == Disk::White ? Disk::Black : Disk::White;
}

inline std::ostream& operator<<(std::ostream& out, const Disk& disk)
{
    return out << disk_string(disk);
}

/// Print an object to stream (default is std::cout).
template<typename T> inline void print(T object, bool newline = true, std::ostream& out = std::cout)
{
    out << object;
    if (newline) {
        out << "\n";
    }
}
}  // namespace othello

// Formatter specialization:
// For some reason MSVC needs to have this here before fmt::format is called below in Move.
// Having it at the end of the file works fine on Clang like it should. Bug in fmt lib with C++20? ¯\_(ツ)_/¯
template<> struct fmt::formatter<othello::Square> : ostream_formatter {};

namespace othello
{
/// Represents one possible disk placement for given disk color.
struct Move {
    Move() : disk(Disk::Empty), square(0, 0), value(0) {}
    Move(Square square, int value, Disk disk, std::vector<Square> directions)
        : disk(disk)
        , square(square)
        , directions(std::move(directions))
        , value(value)
    {}

    friend std::ostream& operator<<(std::ostream& out, const Move& move)
    {
        return out << fmt::format("Square: {} -> value: {}", move.square, move.value);
    }

    bool operator<(const Move& other) const
    {
        // biggest value with the smallest coordinates first
        return value > other.value || (value == other.value && square < other.square);
    }

    Disk disk;
    Square square;
    std::vector<Square> directions;
    int value;
};
}  // namespace othello

// Formatter specialization: simply use overridden ostream operator
template<> struct fmt::formatter<othello::Move> : ostream_formatter {};

// Other option would be to customize based on std::string for example:
// template<> struct fmt::formatter<othello::Square> : fmt::formatter<std::string> {
//    auto format(othello::Square s, format_context& ctx)
//    {
//        return formatter<std::string>::format(fmt::format("({}, {})", s.x, s.y), ctx);
//    }
//};
//
// template<> struct fmt::formatter<othello::Move> : fmt::formatter<std::string> {
//    auto format(othello::Move m, format_context& ctx)
//    {
//        return formatter<std::string>::format(fmt::format("Square: {} -> value: {}", m.square, m.value), ctx);
//    }
//};
