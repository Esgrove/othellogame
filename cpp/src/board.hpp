//==========================================================
// Class Board header
// Defines the game board
// Akseli Lukkarila
// 2019-2026
//==========================================================

#pragma once
#include "models.hpp"

#include <array>
#include <optional>
#include <set>
#include <tuple>
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
    {DOWN, LEFT},
    {DOWN, RIGHT},
    {DOWN, STILL},
    {STILL, LEFT},
    {STILL, RIGHT},
    {UP, LEFT},
    {UP, RIGHT},
    {UP, STILL},
}};

/// Handles game board state and logic.
class Board
{
public:
    explicit Board(size_t size);

    [[nodiscard]] bool can_play() const;
    void place_disk(const Move& chosen_move);
    [[nodiscard]] std::vector<Move> possible_moves(Disk disk) const;
    void print_possible_moves(const std::vector<Move>& moves) const;
    void print_score() const;
    [[nodiscard]] Disk result() const;
    [[nodiscard]] std::string log_entry() const;

private:
    [[nodiscard]] constexpr bool check_coordinates(int x, int y) const;
    [[nodiscard]] constexpr bool check_square(const Square& square) const;
    [[nodiscard]] std::optional<Disk> get_square(const Square& square) const;
    [[nodiscard]] constexpr size_t square_index(const Square& square) const;
    [[nodiscard]] std::tuple<int, int> player_scores() const;
    [[nodiscard]] int score() const;
    void set_square(const Square& square, Disk disk);
    [[nodiscard]] static std::vector<Disk> init_board(size_t size);
    [[nodiscard]] static std::set<Square> init_empty_squares(
        size_t size,
        const std::vector<Disk>& board
    );

    friend std::ostream& operator<<(std::ostream& out, const Board& board);

    friend class BoardTest;

    std::vector<Disk> board;
    std::set<Square> empty_squares;
    std::vector<size_t> indices;
    size_t size;
};

}  // namespace othello

template<>
struct fmt::formatter<othello::Board> : ostream_formatter {};
