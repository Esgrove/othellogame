//==========================================================
// Class Player header
// Defines one player for Othello
// Akseli Lukkarila
//==========================================================

#pragma once
#include <random>

#include "board.hpp"

namespace game {

class Player {
public:
    Player() : rounds(0), color(BLACK), human(true) {};
    explicit Player(Disk col) : rounds(0), color(col), human(true) {};
    Player(Disk col, bool type) : rounds(0), color(col), human(type) {};
    ~Player() = default;;

    // nodiscard == compiler should warn if value returned by function call is not used
    [[nodiscard]] std::string     color_string() const { return get_disk_string(this->color); }
    [[nodiscard]] std::string     color_string_upper() const;
    [[nodiscard]] std::string     type_string() const;

    void            play_one_move(Board& game);
    static void     print_moves(const std::vector<Move>& moves);
    void            set_human(bool is_human) { this->human = is_human; }

    friend std::ostream& operator<< (std::ostream& out, Player& p);

    int             rounds;
    bool            can_play {true};

private:
    Disk            color;
    bool            human;
    std::mt19937    rand_gen {std::mt19937 { std::random_device{}() }};
};

} // namespace game
