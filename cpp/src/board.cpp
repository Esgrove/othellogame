//==========================================================
// Class Board source
// Defines the game board
// Akseli Lukkarila
// 2019-2025
//==========================================================

#include "board.hpp"

#include "colorprint.hpp"

#include <algorithm>  // std::remove, std::transform
#include <numeric>    // std::accumulate, std::iota
#include <ranges>     // std::ranges::transform (requires C++20)

namespace othello
{
Board::Board(const size_t size) : indices(size), size(size)
{
    // Init game board with empty disks.
    board.resize(size * size, Disk::empty);

    // Set starting positions
    const size_t row = size % 2 == 0 ? (size - 1) / 2 : (size - 1) / 2 - 1;
    const size_t col = size / 2;
    board[row * size + row] = Disk::white;
    board[row * size + col] = Disk::black;
    board[col * size + row] = Disk::black;
    board[col * size + col] = Disk::white;

    // Index list (0..size) to avoid repeating same range in for loops
    std::iota(indices.begin(), indices.end(), 0);

    // Keep track of empty squares on board to avoid checking already filled positions
    for (const auto y : indices) {
        for (const auto x : indices) {
            if (board[y * size + x] == Disk::empty) {
                empty_squares.emplace(static_cast<int>(x), static_cast<int>(y));
            }
        }
    }
}

/// Return true if board contains empty squares.
bool Board::can_play() const
{
    // Without keeping track of empty squares:
    // std::find(board.begin(), board.end(), Disk::empty) != board.end();
    return !empty_squares.empty();
}

/// Update board for given disk placement.
void Board::place_disk(const Move& player_move)
{
    auto start = player_move.square;
    if (get_square(start) != Disk::empty) {
        throw std::invalid_argument(
            fmt::format("Trying to place disk to an occupied square {}!", start)
        );
    }
    set_square(start, player_move.disk);
    empty_squares.erase(start);
    for (const auto& square : player_move.affected_squares()) {
        set_square(square, player_move.disk);
    }
}

/// Returns a list of possible moves for the given player.
std::vector<Move> Board::possible_moves(Disk disk) const
{
    std::vector<Move> moves;
    const Disk opposing_disk = opponent(disk);
    for (const Square& square : empty_squares) {
        size_t value {0};
        std::vector<std::pair<Step, size_t>> directions;
        for (const auto& step : STEP_DIRECTIONS) {
            Square pos {square + step};
            // Next square in this direction needs to be the opposing disk
            if (get_square(pos) != opposing_disk) {
                continue;
            }
            size_t num_steps {0};
            // keep stepping forward while opponents disks are found
            while (get_square(pos) == opposing_disk) {
                ++num_steps;
                pos += step;
            }
            // Valid move only if a line of opposing disks ends in own disk
            if (get_square(pos) == disk) {
                value += num_steps;
                directions.emplace_back(step, num_steps);
            }
        }
        if (value > 0) {
            moves.emplace_back(square, disk, value, directions);
        }
    }
    if (!moves.empty()) {
        std::sort(moves.begin(), moves.end());
    }
    return moves;
}

/// Print board with available move coordinates and the resulting points gained.
void Board::print_possible_moves(const std::vector<Move>& moves)
{
    print_yellow(fmt::format("  Possible moves ({}):\n", std::to_string(moves.size())));
    // Convert board from Disk enums to strings
    std::vector<std::string> formatted_board(board.size());
    std::ranges::transform(board, formatted_board.begin(), [&](const Disk& disk) {
        return board_char(disk);
    });
    // Add possible moves to board
    for (const Move& move : moves) {
        const auto index = square_index(move.square);
        formatted_board[index] = get_color(move.value, fmt::terminal_color::yellow);
        fmt::print("  {}\n", move);
    }
    // Print board with move positions
    fmt::print("   ", false);
    for (const auto i : indices) {
        fmt::print(fmt::emphasis::bold, " {}", i);
    }
    for (const auto y : indices) {
        fmt::print(fmt::emphasis::bold, "\n  {}", y);
        for (const auto x : indices) {
            fmt::print(" {}", formatted_board[y * size + x]);
        }
    }
    fmt::print("");
}

/// Print current score for both players.
void Board::print_score() const
{
    auto [black, white] = player_scores();
    fmt::print("");
    print(*this);
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
    const int sum = score();
    if (sum == 0) {
        return empty;
    }
    return sum > 0 ? white : black;
}

/// Get board status string for game log.
std::string Board::log_entry() const
{
    return std::accumulate(
        board.begin(),
        board.end(),
        std::string {},
        [](const std::string& accumulator, const Disk& disk) {
            return accumulator + board_char(disk, false);
        }
    );
}

/// Check that the given coordinates are valid (inside the board).
bool Board::check_coordinates(const int& x, const int& y) const
{
    return 0 <= x && x < static_cast<int>(size) && 0 <= y && y < static_cast<int>(size);
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

/// Returns the state of the board (empty, white, black) at the given coordinates.
std::optional<Disk> Board::get_square(const Square& square) const
{
    return check_coordinates(square.x, square.y) ? std::optional {board[square.y * size + square.x]}
                                                 : std::nullopt;
}

/// Sets the given square to given value.
void Board::set_square(const Square& square, const Disk disk)
{
    if (!check_coordinates(square.x, square.y)) {
        throw std::invalid_argument(fmt::format("Invalid coordinates: {}!", square));
    }
    board[square.y * size + square.x] = disk;
}

/// Map square to index on board.
size_t Board::square_index(const Square& square) const
{
    return static_cast<size_t>(square.y) * size + static_cast<size_t>(square.x);
}

/// Format game board to string
std::ostream& operator<<(std::ostream& out, const Board& board)
{
    // Horizontal header indices
    out << " " << fmt::format(fmt::emphasis::bold, " {}", fmt::join(board.indices, " "));
    for (const auto& y : board.indices) {
        // Vertical header index
        out << "\n" << fmt::format(fmt::emphasis::bold, "{}", y);
        // Output row
        for (const auto x : board.indices) {
            auto disk = board.board[y * board.size + x];
            out << " " << board_char(disk);
        }
    }
    return out;
}
}  // namespace othello
