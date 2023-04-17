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
#include <string>     // string
#include <utility>    // move
#include <vector>

namespace othello
{
/// Represents one game piece or lack of one.
enum class Disk { black = -1, empty = 0, white = 1 };

/// Represents one step direction on the board.
struct Step {
    Step(int x, int y) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& out, const Step& step)
    {
        return out << fmt::format("({},{})", step.x, step.y);
    }

    bool operator<(const Step& other) const { return x < other.x || (x <= other.x && y < other.y); }
    bool operator==(const Step& other) const { return x == other.x && y == other.y; }
    Step operator+(const Step& other) const { return {x + other.x, y + other.y}; }
    Step& operator+=(const Step& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    int x;
    int y;
};

/// Represents one square location on the board.
struct Square {
    Square() : x(0), y(0) {}
    Square(int x, int y) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& out, const Square& square)
    {
        return out << fmt::format("({},{})", square.x, square.y);
    }

    bool operator<(const Square& other) const
    {
        return x < other.x || (x <= other.x && y < other.y);
    }
    bool operator==(const Square& other) const { return x == other.x && y == other.y; }
    Square operator+(const Square& other) const { return {x + other.x, y + other.y}; }
    Square& operator+=(const Square& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Square operator+(const Step& other) const { return {x + other.x, y + other.y}; }

    Square& operator+=(const Step& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    int x;
    int y;
};
}  // namespace othello

// Formatter specialization:
// For some reason MSVC needs to have this here before fmt::format is called below in Move.
// Having it at the end of the file works fine on Clang like it should.
// Bug in fmt lib with C++20 maybe? ¯\_(ツ)_/¯
template<> struct fmt::formatter<othello::Square> : ostream_formatter {};

namespace othello
{
/// Represents one possible disk placement for given disk color.
struct Move {
    Move() : square(0, 0), disk(Disk::empty), value(0) {}
    Move(Square square, Disk disk, unsigned int value, std::vector<Step> directions)
        : square(square)
        , disk(disk)
        , value(value)
        , directions(std::move(directions))
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

    Square square;
    Disk disk;
    unsigned int value;
    std::vector<Step> directions;
};

/// Returns print color for given Disk.
inline fmt::color disk_color(const Disk& disk)
{
    using enum fmt::color;
    if (disk == Disk::empty) {
        return white;
    }
    return disk == Disk::white ? cyan : magenta;
}

/// Returns string character representing board status (black, white, empty).
inline std::string board_char(const Disk& disk)
{
    if (disk == Disk::empty) {
        return "_";
    }
    return get_color(disk == Disk::white ? "W" : "B", disk_color(disk));
}

/// Returns the disk formatted as a colored string.
inline std::string disk_string(const Disk& disk)
{
    auto color = disk_color(disk);
    switch (disk) {
        case Disk::empty:
            return get_color("EMPTY", color);
        case Disk::black:
            return get_color("BLACK", color);
        case Disk::white:
            return get_color("WHITE", color);
        default:
            return "UNKNOWN";
    }
}

/// Returns the opponents disk color.
inline Disk opponent(const Disk& disk)
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

/// Convert object to string using a stringstream.
/// Requires that the stream insertion operator `<<` has been implemented for the given object.
/// Workaround for custom types without formatter specialization for fmt.
template<typename T> inline std::string to_string(const T& object)
{
    std::ostringstream ss;
    ss << object;
    return ss.str();
}
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
//        return formatter<std::string>::format(
//            fmt::format("Square: {} -> value: {}", m.square, m.value), ctx);
//    }
//};
