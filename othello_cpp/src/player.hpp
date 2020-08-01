//==========================================================
// Class Player header
// Defines one player for Othello
// Akseli Lukkarila
//==========================================================

#pragma once
#include <random>

#include "board.hpp"

namespace othello {

class Player {
public:
    Player() : disk(Disk::BLACK), human(true) {};
    explicit Player(Disk disk) : disk(disk), human(true) {};
    ~Player() = default;

    // nodiscard -> compiler should warn if value returned by function call is not used
    [[nodiscard]] bool            can_play() const { return this->can_play_; }
    [[nodiscard]] std::string     type_string() const { return this->human ? "Human   " : "Computer"; }

    void            play_one_move(Board& board);
    void            set_player_type(bool is_human) { this->human = is_human; }

    friend std::ostream& operator<< (std::ostream& out, Player& p);

private:
    Move            get_computer_move(std::vector<Move> moves);
    Move            get_human_move(std::vector<Move> moves);
    static Square   get_square();

    Disk            disk;
    bool            can_play_ {true};
    bool            human;
    bool            show_helpers {true};
    int             rounds_played {0};
    std::mt19937    rand_gen { std::mt19937 {std::random_device{}() } };
};

} // namespace othello
