//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
//==========================================================

#pragma once
#include "colorprint.hpp"

#include <iostream>  // cout, cin
#include <string>    // string
#include <utility>   // move

namespace othello
{
enum class Disk { Black = -1, Empty = 0, White = 1 };

/// Represents one square location on the board.
struct Square {
    Square() : x(0), y(0) {}
    Square(int x, int y) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& out, const Square& sqr)
    {
        return out << fmt::format("({},{})", sqr.x, sqr.y);
    }

    bool operator<(const Square& other) const { return x < other.x || (x <= other.x && y < other.y); }
    bool operator==(const Square& other) const { return x == other.x && y == other.y; }
    Square operator+(const Square& other) const { return Square(x + other.x, y + other.y); }
    Square& operator+=(const Square& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    int x;
    int y;
};

/// Represents one possible disk placement for given disk color.
struct Move {
    Move() : square(0, 0), value(0), disk(Disk::Empty) {}
    Move(Square square, int value, Disk disk, std::vector<Square> directions)
        : square(square)
        , value(value)
        , disk(disk)
        , directions(std::move(directions))
    {
    }

    friend std::ostream& operator<<(std::ostream& out, const Move& move)
    {
        return out << fmt::format("Square: {} -> value: {}", move.square, move.value);
    }

    bool operator<(const Move& other) const
    {
        // biggest value with smallest coordinates first
        return value > other.value || (value == other.value && square < other.square);
    }

    Disk disk;
    Square square;
    std::vector<Square> directions;
    int value;
};

/// Returns print color for given Disk.
inline fmt::color disk_color(const Disk& disk)
{
    if (disk == Disk::Empty) {
        return fmt::color::white;
    }
    return disk == Disk::White ? fmt::color::cyan : fmt::color::magenta;
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
