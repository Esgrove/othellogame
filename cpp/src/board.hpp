//==========================================================
// Class Board header
// Defines the game board
// Akseli Lukkarila
// 2019-2024
//==========================================================

#pragma once
#include "utils.hpp"

#include <array>
#include <optional>
#include <set>
#include <vector>

namespace othello
{
constexpr int UP = 1;
constexpr int DOWN = -1;
constexpr int LEFT = -1;
constexpr int RIGHT = 1;
constexpr int STILL = 0;

/// All possible step directions for a square on the board.
static constexpr std::array<Step, 8> STEP_DIRECTIONS {{
    {UP, LEFT},
    {UP, STILL},
    {UP, RIGHT},
    {STILL, LEFT},
    {STILL, RIGHT},
    {DOWN, LEFT},
    {DOWN, STILL},
    {DOWN, RIGHT},
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

template<> struct fmt::formatter<othello::Board> : ostream_formatter {};
