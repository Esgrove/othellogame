//==========================================================
// Class Player header
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2023
//==========================================================

#pragma once
#include "board.hpp"

#include <random>

namespace othello
{
/// Defines one player (human or computer).
class Player
{
public:
    explicit Player(Disk disk, PlayerSettings settings) : disk(disk), settings(settings) {};

    /// Shorthand to initialize a new player for black disks.
    static Player black(PlayerSettings settings) { return Player(Disk::black, settings); }

    /// Shorthand to initialize a new player for white disks.
    static Player white(PlayerSettings settings) { return Player(Disk::white, settings); }

    // nodiscard -> compiler should warn if value returned by function call is not used
    [[nodiscard]] std::string type_string() const { return this->human ? "Human   " : "Computer"; }

    [[nodiscard]] std::optional<std::string> play_one_move(Board& board);

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
    int rounds_played {0};
    PlayerSettings settings;

    std::mt19937 rand_gen {std::mt19937 {std::random_device {}()}};
};

}  // namespace othello
