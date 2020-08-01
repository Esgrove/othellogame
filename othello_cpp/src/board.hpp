//==========================================================
// Class Board header
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#pragma once
#include <set>
#include <vector>
#include "util.hpp"

namespace othello {

class Board {

public:
    constexpr static int DIRECTIONS[8][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}, {1, -1}, {1, 1}, {-1, 1}, {-1, -1}};

    Board() : size(8) {};
    explicit Board(int size);
    ~Board() = default;

    [[nodiscard]] bool                  can_play() const;
    [[nodiscard]] std::vector<Move>     possible_moves(Disk color) const;
    [[nodiscard]] Disk                  result() const;
    [[nodiscard]] int                   score() const;
    [[nodiscard]] Disk                  square(int x, int y) const;

    void                                place_disc(const Move& move);
    void                                print_score() const;
    void                                print_possible_moves(const std::vector<Move>& moves);

    friend std::ostream& operator<< (std::ostream& out, const Board& othello);

private:
    [[nodiscard]] bool                  check_coordinates(int x, int y) const;
    bool                                set_square(int x, int y, Disk disk);


    std::vector<Disk>    board;
    std::set<Square>     empty_squares;
    int                  size;
};

} // namespace othello
