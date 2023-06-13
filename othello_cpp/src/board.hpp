//==========================================================
// Class Board header
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#pragma once
#include "utils.hpp"

#include <array>
#include <optional>  // std::optional
#include <set>
#include <vector>

namespace othello
{
/// Handles game board state and logic.
class Board
{
public:
    explicit Board(size_t size);

    void place_disk(const Move& move);
    void print_score() const;
    void print_moves(const std::vector<Move>& moves);

    [[nodiscard]] Disk result() const;
    [[nodiscard]] bool can_play() const;
    [[nodiscard]] std::vector<Move> possible_moves(Disk disk) const;

    friend std::ostream& operator<<(std::ostream& out, const Board& board);

private:
    [[nodiscard]] bool check_coordinates(const int& x, const int& y) const;
    [[nodiscard]] int score() const;
    [[nodiscard]] std::optional<othello::Disk> get_square(const Square& square) const;
    [[nodiscard]] std::tuple<int, int> player_scores() const;
    void set_square(const Square& square, Disk disk);

    std::set<Square> empty_squares;
    std::vector<Disk> board;
    std::vector<size_t> indices;
    size_t size;
    std::array<Step, 8> step_directions {{
        {-1, -1},
        {-1, 0},
        {-1, 1},
        {0, -1},
        {0, 1},
        {1, -1},
        {1, 0},
        {1, 1},
    }};
};

}  // namespace othello
