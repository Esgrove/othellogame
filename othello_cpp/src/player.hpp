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
    Player() : rounds(0), m_color(BLACK), human(true) {};
    explicit Player(Disk col) : rounds(0), m_color(col), human(true) {};
    Player(Disk col, bool type) : rounds(0), m_color(col), human(type) {};
    ~Player() = default;;

    Disk            color() const { return m_color; }
    std::string     color_string() const { return get_disk_string(this->m_color); }
    std::string     color_string_upper() const;
    std::string     type_string() const;

    void            play_one_move(Board& game);
    static void     print_moves(const std::vector<Move>& moves);
    void            set_human(bool is_human) { this->human = is_human; }

    int             rounds;
    bool            canPlay {true};

    friend std::ostream& operator<< (std::ostream& out, Player& p);

private:
    Disk            m_color;
    bool            human;
    std::mt19937    rand_gen {std::mt19937 { std::random_device{}() }};
};

} // namespace game
