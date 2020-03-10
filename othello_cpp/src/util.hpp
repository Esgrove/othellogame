//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
//==========================================================

#pragma once
#include <iostream> // cout, cin
#include <string>	// string

#include "colorprint.hpp"

namespace othello {

struct Square {
    Square() : x(0), y(0) {}
    Square(int x, int y) : x(x), y(y) {}

    friend std::ostream& operator<< (std::ostream& out, const Square& sqr) {
        return out << "(" << sqr.x << "," << sqr.y << ")";
    }

    bool operator<(const Square& other) const;

    int x;
    int y;
};

struct Move {
    Move() : value(0) {}
    Move(int x, int y, int value) : square(x, y), value(value) {}

    friend std::ostream& operator<< (std::ostream& out, const Move& move) {
        return out << "point: " << move.square << " -> value: " << move.value;
    }

    bool operator< (const Move& other) const;

    Square   square;
    int      value;
};

enum class Disk {
    BLACK = -1,
    EMPTY =  0,
    WHITE =  1,
    INVALID =  2
};

/// Returns print color for given Disk.
    inline Color disk_color(const Disk color) {
        if (color == Disk::EMPTY) {
            return Color::WHITE;
        }
        return color == Disk::WHITE ? Color::MAGENTA : Color::CYAN;
    }

/// Returns the opponents color
inline Disk other_disk(const Disk color) {
    return color == Disk::WHITE ? Disk::BLACK : Disk::WHITE;
}

inline std::string get_disk_string(const Disk color) {
    return color == Disk::BLACK ? "black" : "white";
}

inline std::string get_board_char(const Disk color) {
    if (color == Disk::WHITE) {
        return "W";
    } else if (color == Disk::BLACK) {
        return "B";
    }
    return "_";
}

/// Print object to std::cout
template<typename T>
inline void print(T object, bool newline=true) {
    std::cout << object;
    if (newline) { 
        std::cout << "\n";
    }
}

} // namespace othello
