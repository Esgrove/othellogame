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
    void                    init_game();
    void                    game_loop();
    void                    show_result();

    static bool             get_answer(const std::string& question, const std::string& yes="y", const std::string& no="n");
    static int              get_board_size();

    Board                   board_;
    Player                  player_black_;
    Player                  player_white_;
    int                     rounds_played_ {0};
};

} // namespace othello
