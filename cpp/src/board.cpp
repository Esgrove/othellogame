//==========================================================
// Class Board source
// Defines the game board
// Akseli Lukkarila
// 2019-2026
//==========================================================

#include "board.hpp"

#include "colorprint.hpp"

#include <algorithm>  // std::ranges::sort, std::ranges::transform
#include <numeric>    // std::accumulate, std::iota
#include <ranges>     // std::ranges::transform (requires C++20)
#include <stdexcept>  // exceptions

namespace othello
{
/// Initialize a new board for the given board size.
Board::Board(const size_t size) :
    board(init_board(size)),
    // Keep track of empty squares on board to avoid checking already filled positions.
    empty_squares(init_empty_squares(size, board)),
    indices(size),
    size(size)
{
    // Index list (0...size) to avoid repeating same range in loops.
    std::iota(indices.begin(), indices.end(), 0);
}

/// Return true if board contains empty squares.
bool Board::can_play() const
{
    return !empty_squares.empty();
}

/// Update board for given disk placement.
void Board::place_disk(const Move& chosen_move)
{
    const auto start = chosen_move.square;
    const auto square = get_square(start);
    if (!square.has_value()) {
        throw std::invalid_argument(fmt::format("Invalid coordinates: {}", start));
    }
    if (square.value() != Disk::empty) {
        throw std::invalid_argument(
            fmt::format("Trying to place disk to an occupied square: {}!", start)
        );
    }
    set_square(start, chosen_move.disk);
    empty_squares.erase(start);
    for (const auto& affected_square : chosen_move.affected_squares()) {
        set_square(affected_square, chosen_move.disk);
    }
}

/// Returns a list of possible moves for the given player.
std::vector<Move> Board::possible_moves(Disk disk) const
{
    std::vector<Move> moves;
    const Disk opposing_disk = opponent(disk);
    for (const Square& square : empty_squares) {
        size_t value {0};
        std::vector<Direction> directions;
        for (const auto& step : STEP_DIRECTIONS) {
            Square pos {square + step};
            // Next square in this direction needs to be the opposing disk
            if (get_square(pos) != opposing_disk) {
                continue;
            }
            size_t num_steps {0};
            // Keep stepping over opponents disks
            while (get_square(pos) == opposing_disk) {
                ++num_steps;
                pos += step;
            }
            // Valid move only if a line of opposing disks ends with own disk
            if (get_square(pos) == disk) {
                directions.emplace_back(step, num_steps);
                value += num_steps;
            }
        }
        if (value > 0) {
            moves.emplace_back(square, disk, value, directions);
        }
    }
    std::ranges::sort(moves);
    return moves;
}

/// Print board with available move coordinates and the resulting points gained.
void Board::print_possible_moves(const std::vector<Move>& moves) const
{
    print_yellow("  Possible moves ({}):\n", moves.size());
    // Convert board from Disk enums to strings
    std::vector<std::string> formatted_board(board.size());
    std::ranges::transform(board, formatted_board.begin(), [&](const Disk& disk) {
        return board_char_with_color(disk);
    });
    // Add possible moves to board
    for (const Move& possible_move : moves) {
        const auto index = square_index(possible_move.square);
        formatted_board[index] = get_color(possible_move.value, fmt::terminal_color::yellow);
        fmt::print("  {}\n", possible_move);
    }
    // Print board with move positions
    fmt::print("    ");
    for (const auto i : indices) {
        print_bold(" {}", i);
    }
    for (const auto y : indices) {
        print_bold("\n  {}", y);
        for (const auto x : indices) {
            fmt::print(" {}", formatted_board[y * size + x]);
        }
    }
    fmt::print("\n");
}

/// Print current score for both players.
void Board::print_score() const
{
    const auto [black, white] = player_scores();
    fmt::print("\n{}\n", *this);
    fmt::print(
        "Score: {} | {}\n",
        get_color(std::to_string(black), disk_color(Disk::black)),
        get_color(std::to_string(white), disk_color(Disk::white))
    );
}

/// Returns the winning disk colour. Empty indicates a draw.
Disk Board::result() const
{
    using enum Disk;
    const int total_score = score();
    if (total_score == 0) {
        return empty;
    }
    return total_score > 0 ? white : black;
}

/// Get board status string for game log.
std::string Board::log_entry() const
{
    return std::accumulate(
        board.begin(),
        board.end(),
        std::string {},
        [](const std::string& accumulator, const Disk& disk) {
            return accumulator + board_char(disk);
        }
    );
}

/// Check that the given coordinates are valid (inside the board).
constexpr bool Board::check_coordinates(const int x, const int y) const
{
    return 0 <= x && x < static_cast<int>(size) && 0 <= y && y < static_cast<int>(size);
}

/// Check that the given square is valid (inside the board).
constexpr bool Board::check_square(const Square& square) const
{
    return check_coordinates(square.x, square.y);
}

/// Returns the state of the board (empty, white, black) at the given square.
std::optional<Disk> Board::get_square(const Square& square) const
{
    return check_square(square) ? std::optional {board[square_index(square)]} : std::nullopt;
}

/// Map square to board index.
constexpr size_t Board::square_index(const Square& square) const
{
    return static_cast<size_t>(square.y) * size + static_cast<size_t>(square.x);
}

/// Count and return the number of black and white disks.
std::tuple<int, int> Board::player_scores() const
{
    int black = 0;
    int white = 0;
    for (const auto& disk : board) {
        switch (disk) {
            case Disk::black:
                ++black;
                break;
            case Disk::white:
                ++white;
                break;
            default:
                break;
        }
    }
    return {black, white};
}

/// Returns the total score.
/// Positive value means more white disks and negative means more black disks.
int Board::score() const
{
    // Enum class prevents implicit conversion from Disk to int,
    // need to use lambda to cast Disk values to int.
    return std::accumulate(board.begin(), board.end(), 0, [](const int sum, Disk disk) {
        return sum + static_cast<int>(disk);
    });
}

/// Sets the given square to the given value.
void Board::set_square(const Square& square, const Disk disk)
{
    if (!check_square(square)) {
        throw std::invalid_argument(fmt::format("Invalid coordinates: {}", square));
    }
    board[square_index(square)] = disk;
}

/// Initialize game board with starting disk positions.
std::vector<Disk> Board::init_board(const size_t size)
{
    // Initialize game board with empty disks
    std::vector<Disk> board(size * size, Disk::empty);
    // Set starting positions
    const size_t row = size % 2 == 0 ? (size - 1) / 2 : (size - 1) / 2 - 1;
    const size_t col = size / 2;
    board[row * size + row] = Disk::white;
    board[row * size + col] = Disk::black;
    board[col * size + row] = Disk::black;
    board[col * size + col] = Disk::white;
    return board;
}

/// Initialize empty squares for the board.
std::set<Square> Board::init_empty_squares(const size_t size, const std::vector<Disk>& board)
{
    std::set<Square> empty_squares;
    for (size_t index = 0; index < board.size(); ++index) {
        if (board[index] == Disk::empty) {
            empty_squares.emplace(static_cast<int>(index % size), static_cast<int>(index / size));
        }
    }
    return empty_squares;
}

/// Format game board to string
std::ostream& operator<<(std::ostream& out, const Board& board)
{
    // Horizontal indices
    out << "  " << fmt::format(fmt::emphasis::bold, "{}", fmt::join(board.indices, " "));
    for (const auto& y : board.indices) {
        // Vertical index
        out << "\n" << fmt::format(fmt::emphasis::bold, "{}", y);
        // Row values
        for (const auto x : board.indices) {
            out << " " << board_char_with_color(board.board[y * board.size + x]);
        }
    }
    return out;
}
}  // namespace othello
