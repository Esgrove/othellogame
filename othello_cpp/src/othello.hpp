//==========================================================
// Class Othello header
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#pragma once
#include "player.hpp"

namespace game {

class Othello
{

public:
    Othello();
    ~Othello() = default;;

    void                    play_game();

private:
    static std::string      get_answer(const std::string& text);
    static int              get_board_size();

    Board                   board;
    Player                  player_black;
    Player                  player_white;
    int                     rounds;
};

} // namespace game
