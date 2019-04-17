//==========================================================
// Utils
// Helper utilities for Othello game
// Akseli Lukkarila
//==========================================================

#pragma once
#include <iostream> // std::cout, std::cin
#include <string>	// std::string
//==========================================================

enum Color {
    BLACK = -1,
    EMPTY =  0,
    WHITE =  1,
    ERROR =  2
};
//==========================================================

struct Square {
    int x;
    int y;
    Square() : x(0), y(0) {}
    Square(int x, int y) : x(x), y(y) {}
    friend std::ostream& operator<< (std::ostream& out, const Square& sqr) {
        return out << "(" << sqr.x << "," << sqr.y << ")";
    }
    bool operator<(const Square& other) const;
};
//==========================================================

struct Move {
    Square	square;
    int     value;
    Move() : value(0) {}
    Move(int x, int y, int val) : square(x,y), value(val) {}
    friend std::ostream& operator<< (std::ostream& out, const Move& move) {
        return out << "point: " << move.square << " -> value: " << move.value;
    }
    bool operator< (const Move& other) const;
};
//==========================================================

inline std::string getColorString(const Color color) {
    return color == Color::BLACK ? std::string("black") : std::string("white");
}
//==========================================================

inline std::string getBoardString(const Color color) {
    std::string disk{ "_" };
    if (color == WHITE) {
        disk = "W";
    }
    else if (color == BLACK) {
        disk = "B";
    }
    return disk;
}
//==========================================================

template<typename T>
inline void print(T object) {
    std::cout << object << std::endl;
}
//==========================================================
