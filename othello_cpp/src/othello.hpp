//==========================================================
// Class Othello header
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#pragma once
#include "player.hpp"

namespace othello {

class Othello
{

public:
    Othello() = default;
    ~Othello() = default;

    void                    play_game();

private:
    static std::string      get_answer(const std::string& text);
    static int              get_board_size();

    void                    init_game();

    Board                   board;
    Player                  player_black;
    Player                  player_white;
    int                     rounds {0};
};

} // namespace othello
