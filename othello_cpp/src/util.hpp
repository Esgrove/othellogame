//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
//==========================================================

#pragma once
#include <iostream> // std::cout, std::cin
#include <string>	// std::string

namespace game {

enum Disk {
    BLACK = -1,
    EMPTY =  0,
    WHITE =  1,
    INVALID =  2
};

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

    Square	 square;
    int      value;
};

inline std::string get_disk_string(const Disk color) {
    return color == Disk::BLACK ? std::string("black") : std::string("white");
}

inline std::string get_board_string(const Disk color) {
    if (color == WHITE) {
        return "W";
    }
    else if (color == BLACK) {
        return "B";
    }
    return "_";
}

template<typename T>
inline void print(T object, bool newline=true) {
    std::cout << object;
    if (newline) { 
        std::cout << "\n";
    }
}

} // namespace game
