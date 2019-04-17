//==========================================================
// Utils source
// Helper utilities for Othello game
// Akseli Lukkarila
//==========================================================

#include "util.hpp"

bool Move::operator<(const Move& other) const
{
    if (value > other.value) {
        return true;
    }
    else if (value == other.value && square < other.square) {
        return true;
    }
    return false;
}

bool Square::operator<(const Square& other) const
{
    return x < other.x || (x <= other.x && y < other.y);
}
