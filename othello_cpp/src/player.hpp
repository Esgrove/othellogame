//==========================================================
// Class Player header
// Defines one player for Othello
// Akseli Lukkarila
//==========================================================

#pragma once
#include "board.hpp"

#include <random>

namespace othello
{
/// Defines one player (human or computer).
class Player {
public:
    explicit Player(Disk disk) : disk(disk) {};

    // nodiscard -> compiler should warn if value returned by function call is not used
    [[nodiscard]] std::string type_string() const { return this->human ? "Human   " : "Computer"; }

    void play_one_move(Board& board);
    void set_human(bool is_human);
    void reset();

    // String formatting
    friend std::ostream& operator<<(std::ostream& out, Player& player);

    bool can_play {true};

private:
    static Square get_square();

    Move get_computer_move(const std::vector<Move>& moves);
    Move get_human_move(const std::vector<Move>& moves);

    Disk disk;
    bool human {true};
    bool show_helpers {true};
    int rounds_played {0};
    std::mt19937 rand_gen {std::mt19937 {std::random_device {}()}};
};

}  // namespace othello
