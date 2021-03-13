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
    explicit Othello(int board_size) : board_size(board_size) {};
    ~Othello() = default;

    void play();
    static int get_board_size();

private:
    void init_game();
    void game_loop();
    void print_result();
    void print_status();

    static bool get_answer(const std::string& question, const std::string& yes = "y", const std::string& no = "n");

    Board board;
    Player player_black;
    Player player_white;
    int board_size;
    int rounds_played {0};
};

}  // namespace othello
