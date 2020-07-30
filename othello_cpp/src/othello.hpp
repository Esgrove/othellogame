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

    void                    play();

private:
    static bool             get_answer(const std::string& text, const std::string& yes="y", const std::string& no="n");
    static int              get_board_size();

    void                    init_game();
    void                    play_loop();
    void                    show_result();

    Board                   board_;
    Player                  player_black_;
    Player                  player_white_;
    int                     rounds_played_ {0};
};

} // namespace othello
