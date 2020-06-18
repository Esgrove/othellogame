//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
//==========================================================

#pragma once
#include <iostream> // cout, cin
#include <string>	// string

#include <fmt/ostream.h>
#include <fmt/color.h>

namespace othello {

struct Square {
    Square() : x(0), y(0) {}
    Square(int x, int y) : x(x), y(y) {}

    friend std::ostream& operator<< (std::ostream& out, const Square& sqr) {
        return out << fmt::format("({},{})", sqr.x, sqr.y);
    }

    bool operator<(const Square& other) const;

    int x;
    int y;
};

struct Move {
    Move() : value(0) {}
    Move(int x, int y, int value) : square(x, y), value(value) {}

    friend std::ostream& operator<< (std::ostream& out, const Move& move) {
        return out << fmt::format("point: {} -> value: {}", move.square, move.value);
}

    bool operator< (const Move& other) const;

    Square   square;
    int      value;
};

enum class Disk {
    BLACK = -1,
    EMPTY = 0,
    WHITE = 1,
    INVALID = 2
};

inline std::string board_char(const Disk color) {
    if (color == Disk::EMPTY) {
        return "_";
    }
    return color == Disk::WHITE ? "W" : "B";
}

/// Returns print color for given Disk.
inline fmt::color disk_color(const Disk disk) {
    if (disk == Disk::EMPTY) {
        return fmt::color::white;
    }
    return disk == Disk::WHITE ? fmt::color::magenta : fmt::color::cyan;
}

/// Returns disk color as a string.
inline std::string disk_string(const Disk& disk) {
    if (disk == Disk::EMPTY) {
        return "empty";
    }
    return disk == Disk::BLACK ? "black" : "white";
}

/// Returns disk color as an uppercase string.
inline std::string disk_string_upper(const Disk& disk) {
    std::string text = disk_string(disk);
    std::transform(text.begin(), text.end(), text.begin(), ::toupper);
    return text;
}

/// Returns the opponents disk color
inline Disk other_disk(const Disk color) {
    if (color == Disk::EMPTY) {
        return Disk::EMPTY;
    }
    return color == Disk::WHITE ? Disk::BLACK : Disk::WHITE;
}

inline std::ostream& operator<<(std::ostream &out, const Disk& color) {
    return out << disk_string(color);
}

/// Print an object to stream (default is std::cout)
template<typename T>
inline void print(T object, bool newline=true, std::ostream& out=std::cout) {
    out << object;
    if (newline) {
        out << "\n";
    }
}

} // namespace othello
