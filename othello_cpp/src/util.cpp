//==========================================================
// Utils source
// Helper utilities for Othello game
// Akseli Lukkarila
//==========================================================

#include "util.hpp"

bool othello::Move::operator<(const Move& other) const
{
    // biggest value with smallest coordinates first
    return value > other.value || (value == other.value && square < other.square);
}

bool othello::Square::operator<(const Square& other) const
{
    return x < other.x || (x <= other.x && y < other.y);
}
