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
class Player
{
public:
    Player() : disk(Disk::Black), human(true) {};
    explicit Player(Disk disk) : disk(disk), human(true) {};
    ~Player() = default;

    // nodiscard -> compiler should warn if value returned by function call is not used
    [[nodiscard]] bool can_play() const { return this->can_play_; }
    [[nodiscard]] std::string type_string() const { return this->human ? "Human   " : "Computer"; }

    void play_one_move(Board& board);
    void set_human(bool is_human) { this->human = is_human; }

    friend std::ostream& operator<<(std::ostream& out, Player& p);

private:
    Move get_computer_move(const std::vector<Move>& moves);
    Move get_human_move(const std::vector<Move>& moves);
    static Square get_square();

    Disk disk;
    bool can_play_ {true};
    bool human;
    bool show_helpers {true};
    int rounds_played {0};
    std::mt19937 rand_gen {std::mt19937 {std::random_device {}()}};
};

}  // namespace othello
