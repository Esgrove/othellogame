//==========================================================
// Class Board header
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#pragma once
#include "utils.hpp"

#include <optional>  // std::optional
#include <set>
#include <vector>

namespace othello {

class Board {
public:
    constexpr static int step_directions[8][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}, {1, -1}, {1, 1}, {-1, 1}, {-1, -1}};

    Board() : size(8), indices(8) {};
    explicit Board(int size);
    ~Board() = default;

    [[nodiscard]] bool can_play() const;
    [[nodiscard]] std::vector<Move> possible_moves(Disk disk) const;
    [[nodiscard]] Disk result() const;

    void place_disc(const Move& move);
    void print_score() const;
    void print_moves(const std::vector<Move>& moves);

    friend std::ostream& operator<<(std::ostream& out, const Board& othello);

private:
    [[nodiscard]] bool check_coordinates(const int& x, const int& y) const;
    [[nodiscard]] int score() const;
    [[nodiscard]] std::tuple<int, int> player_scores() const;
    [[nodiscard]] std::optional<othello::Disk> get_square(const Square& square) const;
    void set_square(const Square& square, Disk disk);

    int size;
    std::set<Square> empty_squares;
    std::vector<Disk> board;
    std::vector<int> indices;
};

}  // namespace othello
