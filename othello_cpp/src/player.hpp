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
    Player() : color_(Disk::BLACK), human_(true) {};
    explicit Player(Disk col) : color_(col), human_(true) {};
    ~Player() = default;

    // nodiscard -> compiler should warn if value returned by function call is not used
    [[nodiscard]] std::string     type_string() const { return this->human_ ? "Human   " : "Computer"; }
    [[nodiscard]] bool            can_play() const { return this->can_play_; }

    void            play_one_move(Board& board);
    void            set_human(bool is_human) { this->human_ = is_human; }

    friend std::ostream& operator<< (std::ostream& out, Player& p);

private:
    Disk            color_;
    bool            human_;
    bool            can_play_ {true};
    bool            show_helpers_ {true};
    int             rounds_played_ {0};
    std::mt19937    rand_gen_ {std::mt19937 {std::random_device{}() }};
};

} // namespace othello
