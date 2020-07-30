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

    Board() : size(8) {};
    explicit Board(int size);
    ~Board() = default;

    static void                         print_possible_moves(const std::vector<Move>& moves);

    [[nodiscard]] bool                  can_place_to_square(int x, int y, Disk color) const;
    [[nodiscard]] bool                  can_play() const;
    [[nodiscard]] bool                  check_coordinates(int x, int y) const;
    [[nodiscard]] std::vector<Move>     possible_moves(Disk color) const;
    [[nodiscard]] Disk                  result() const;
    [[nodiscard]] int                   score() const;
    [[nodiscard]] Disk                  square(int x, int y) const;

    bool                                place_disc(int x, int y, Disk color);
    void                                print_score() const;

    friend std::ostream& operator<< (std::ostream& out, const Board& othello);

private:
    bool                set_square(int x, int y, Disk disk);

    std::vector<Disk>    board;
    std::vector<Square>  empty_squares;
    int                  size;
};

} // namespace othello
