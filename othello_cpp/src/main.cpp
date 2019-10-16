//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#include "colorprint.hpp"
#include "othello.hpp"

int main() {
    print_color("OTHELLO GAME - C++\n", Color::GREEN);
    game::Othello game;
    game.playGame();
}
