//==========================================================
// Class Board header
// Defines the game board
// Akseli Lukkarila
// 2019-2024
//==========================================================

#pragma once
#include "utils.hpp"

#include <array>
#include <optional>  // std::optional
#include <set>
#include <vector>

namespace othello
{

/// All possible step directions for a square on the board.
static const std::array<Step, 8> STEP_DIRECTIONS {{
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, -1},
    {0, 1},
    {1, -1},
    {1, 0},
    {1, 1},
}};

/// Handles game board state and logic.
class Board
{
public:
    explicit Board(size_t size);

    void place_disk(const Move& player_move);
    void print_score() const;
    void print_possible_moves(const std::vector<Move>& moves);

    [[nodiscard]] bool can_play() const;
    [[nodiscard]] Disk result() const;
    [[nodiscard]] std::string log_entry() const;
    [[nodiscard]] std::vector<Move> possible_moves(Disk disk) const;

    friend std::ostream& operator<<(std::ostream& out, const Board& board);

private:
    [[nodiscard]] bool check_coordinates(const int& x, const int& y) const;
    [[nodiscard]] int score() const;
    [[nodiscard]] std::optional<Disk> get_square(const Square& square) const;
    [[nodiscard]] std::tuple<int, int> player_scores() const;
    [[nodiscard]] size_t square_index(const Square& square) const;
    void set_square(const Square& square, Disk disk);

    std::set<Square> empty_squares;
    std::vector<Disk> board;
    std::vector<size_t> indices;
    size_t size;
};

}  // namespace othello
