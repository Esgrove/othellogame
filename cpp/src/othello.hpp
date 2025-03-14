//==========================================================
// Class Othello header
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2025
//==========================================================

#pragma once
#include "player.hpp"

namespace othello
{
/// Gameplay loop and main logic
class Othello
{
public:
    explicit Othello(Settings settings);

    void play();

    static size_t get_board_size();

private:
    static bool get_answer(
        const std::string& question,
        const std::string& yes = "y",
        const std::string& no = "n"
    );

    void init_game();
    void game_loop();
    void print_log() const;
    void print_result() const;
    void print_status() const;

    Board board;
    Settings settings;
    Player player_black;
    Player player_white;
    std::vector<std::string> game_log;
    size_t rounds_played {0};
    size_t games_played {0};
};

}  // namespace othello
