//==========================================================
// Class Board header
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#pragma once
#include <vector>

#include "util.hpp"

namespace othello {

class Board {

public:
    constexpr static int directions[8][2] = {{0,-1}, {1, 0}, {0, 1}, {-1, 0}, {1,-1}, {1, 1}, {-1, 1}, {-1,-1}};

    Board() : dim(0), size(8) {};
    explicit Board(int dim);
    ~Board() = default;

    [[nodiscard]] std::vector<Move>     get_possible_moves(Disk color) const;
    [[nodiscard]] Disk                  get_result() const;
    [[nodiscard]] Disk                  square(int x, int y) const;

    [[nodiscard]] bool                  can_place_to_square(int x, int y, Disk color) const;
    [[nodiscard]] bool                  check_coordinates(int x, int y) const;
    bool                                place_disc(int x, int y, Disk color);

    void                                print_score() const;

    friend std::ostream& operator<< (std::ostream& out, const Board& othello);

private:
    bool                set_square(int x, int y, Disk disk);

    std::vector<Disk>   board;
    int                 dim;
    int                 size;
};

} // namespace othello
