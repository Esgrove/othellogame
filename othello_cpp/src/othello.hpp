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

    void                    playGame();

private:
    static std::string      getAnswer(const std::string& text);
    static int              getBoardSize();

    Board                   board;
    Player                  player_black;
    Player                  player_white;
};

} // namespace game
