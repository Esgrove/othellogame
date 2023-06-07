//==========================================================
// Class Othello header
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#pragma once
#include "player.hpp"

namespace othello
{
class Othello
{
public:
    explicit Othello(size_t board_size);
    void play();

    static size_t get_board_size();

private:
    static bool get_answer(const std::string& question, const std::string& yes = "y", const std::string& no = "n");

    void init_game();
    void game_loop();
    void print_result() const;
    void print_status() const;

    Board board;
    Player player_black {Player(Disk::black)};
    Player player_white {Player(Disk::white)};
    size_t board_size;
    int rounds_played {0};
    int games_played {0};
};

}  // namespace othello
