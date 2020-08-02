//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
//==========================================================

#pragma once
#include <iostream> // cout, cin
#include <string>   // string
#include <utility>

#include <fmt/ostream.h>
#include <fmt/color.h>

namespace othello {

enum class Disk {
    BLACK = -1,
    EMPTY = 0,
    WHITE = 1,
    ERROR = 2
};

/// Represents one square location on the board.
struct Square {
    Square() : x(0), y(0) {}
    Square(int x, int y) : x(x), y(y) {}

    friend std::ostream& operator<< (std::ostream& out, const Square& sqr) {
        return out << fmt::format("({},{})", sqr.x, sqr.y);
    }

    bool operator<(const Square& other) const { return x < other.x || (x <= other.x && y < other.y); }
    bool operator==(const Square& other) const { return x == other.x && y == other.y; }
    Square operator+(const Square& other) const { return Square(x + other.x, y + other.y); }
    Square& operator+=(const Square& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    int x;
    int y;
};

/// Represents one possible disk placement for given disk color.
struct Move {
    Move() : square(0,0), value(0), disk(Disk::EMPTY) {}
    Move(Square square, int value, Disk disk, std::vector<Square> directions) :
        square(square), value(value), disk(disk), directions(std::move(directions)) {}

    friend std::ostream& operator<< (std::ostream& out, const Move& move) {
        return out << fmt::format("Square: {} -> value: {}", move.square, move.value);
    }

    bool operator< (const Move& other) const {
        // biggest value with smallest coordinates first
        return value > other.value || (value == other.value && square < other.square);
    }

    Square              square;
    int                 value;
    Disk                disk;
    std::vector<Square> directions;
};

struct Step {
    Step(int x, int y) : x(x), y(y) {}
    int x;
    int y;
};

inline std::string board_char(const Disk& disk) {
    if (disk == Disk::EMPTY) {
        return "_";
    }
    return disk == Disk::WHITE ? "W" : "B";
}

/// Returns print color for given Disk.
inline fmt::color disk_color(const Disk& disk) {
    if (disk == Disk::EMPTY) {
        return fmt::color::white;
    }
    return disk == Disk::WHITE ? fmt::color::cyan : fmt::color::magenta;
}

/// Returns disk color as a string.
inline std::string disk_string(const Disk& disk) {
    if (disk == Disk::EMPTY) {
        return "empty";
    }
    return disk == Disk::WHITE ? "white" : "black";
}

/// Returns disk color string in uppercase.
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
