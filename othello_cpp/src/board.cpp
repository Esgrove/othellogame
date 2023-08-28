//==========================================================
// Class Board source
// Defines the game board
// Akseli Lukkarila
// 2019-2023
//==========================================================

#include "board.hpp"

#include "colorprint.hpp"

#include <algorithm>  // std::remove, std::transform
#include <numeric>    // std::accumulate, std::iota

namespace othello
{
othello::Board::Board(size_t size) : indices(size), size(size)
{
    // init game board with empty disks.
    board.resize(size * size, Disk::empty);

    // set starting positions
    auto row = size % 2 == 0 ? (size - 1) / 2 : (size - 1) / 2 - 1;
    auto col = size / 2;
    board[row * size + row] = Disk::black;
    board[row * size + col] = Disk::white;
    board[col * size + row] = Disk::white;
    board[col * size + col] = Disk::black;

    // index list (0...size) to avoid repeating same range in for loops
    std::iota(indices.begin(), indices.end(), 0);

    // keep track of empty squares on board to avoid checking already filled positions
    for (auto y : indices) {
        for (auto x : indices) {
            if (board[y * size + x] == Disk::empty) {
                empty_squares.emplace(static_cast<int>(x), static_cast<int>(y));
            }
        }
    }
}

/// Return true if board contains empty squares.
/// -> still possible to make a move.
bool othello::Board::can_play() const
{
    // std::find(board.begin(), board.end(), Disk::empty) != board.end();
    return !empty_squares.empty();
}

/// Update board for given disk placement.
void othello::Board::place_disk(const othello::Move& move)
{
    auto start = move.square;
    if (get_square(start) != Disk::empty) {
        throw std::invalid_argument(
            fmt::format("Trying to place disk to an occupied square {}!", start));
    }
    set_square(start, move.disk);
    empty_squares.erase(start);
    for (const auto& step : move.directions) {
        Square pos = start + step;
        while (get_square(pos) == opponent(move.disk)) {
            set_square(pos, move.disk);
            pos += step;
        }
    }
}

/// Returns a list of possible moves for the given player.
std::vector<othello::Move> othello::Board::possible_moves(Disk disk) const
{
    std::vector<Move> moves;
    Disk opposing_disk = opponent(disk);
    for (const Square& square : empty_squares) {
        int value {0};
        std::vector<Step> directions;
        for (const auto& step : step_directions) {
            Square pos {square + step};
            // next square in this direction needs to be opponents disk
            if (get_square(pos) != opposing_disk) {
                continue;
            }
            int num_steps {0};
            // keep stepping forward while opponents disks are found
            while (get_square(pos) == opposing_disk) {
                ++num_steps;
                pos += step;
            }
            // valid move only if a line of opponents disks ends in own disk
            if (get_square(pos) == disk) {
                value += num_steps;
                directions.emplace_back(step);
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
void othello::Board::print_moves(const std::vector<Move>& moves)
{
    fmt::print(
        fmt::fg(fmt::color::yellow), "  Possible moves ({}):\n", std::to_string(moves.size()));
    // Convert board from Disk enums to strings
    std::vector<std::string> formatted_board(board.size());
    std::transform(
        board.begin(), board.end(), formatted_board.begin(), [&](Disk disk) -> std::string {
            return board_char(disk);
        });
    // Add possible moves
    for (const Move& move : moves) {
        formatted_board[move.square.y * size + move.square.x]
            = get_color(move.value, fmt::color::yellow);
        fmt::print("  {}\n", move);
    }
    // Print board with move positions
    print("   ", false);
    for (const auto i : indices) {
        fmt::print(" {}", i);
    }
    for (const auto y : indices) {
        fmt::print("\n  {}", y);
        for (const auto x : indices) {
            fmt::print(" {}", formatted_board[y * size + x]);
        }
    }
    print("");
}

/// Print current score for both players.
void othello::Board::print_score() const
{
    auto [black, white] = player_scores();
    print("");
    print(*this);
    fmt::print(
        "Score: {} | {}\n",
        get_color(std::to_string(black), disk_color(Disk::black)),
        get_color(std::to_string(white), disk_color(Disk::white)));
}

/// Returns the winner color.
othello::Disk othello::Board::result() const
{
    using enum othello::Disk;
    int sum = score();
    if (sum == 0) {
        return empty;
    }
    return sum > 0 ? white : black;
}

/// Check that the given coordinates are inside the board.
bool othello::Board::check_coordinates(const int& x, const int& y) const
{
    return 0 <= x && x < static_cast<int>(size) && 0 <= y && y < static_cast<int>(size);
}

/// Count and return the number of black and white disks.
std::tuple<int, int> othello::Board::player_scores() const
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
int othello::Board::score() const
{
    // enum class prevents implicit conversion from Disk to int,
    // need to use lambda to cast Disk values to int
    return std::accumulate(
        board.begin(), board.end(), 0, [](int s, Disk d) { return s + static_cast<int>(d); });
}

/// Returns the state of the board (empty, white, black) at the given coordinates.
std::optional<othello::Disk> othello::Board::get_square(const Square& square) const
{
    return check_coordinates(square.x, square.y)
        ? std::optional<Disk> {board[square.y * size + square.x]}
        : std::nullopt;
}

/// Sets the given square to given value.
void othello::Board::set_square(const Square& square, Disk disk)
{
    if (!check_coordinates(square.x, square.y)) {
        throw std::invalid_argument(fmt::format("Invalid coordinates: {}!", square));
    }
    board[square.y * size + square.x] = disk;
}

/// Format game board to string
std::ostream& operator<<(std::ostream& out, const Board& board)
{
    out << " ";
    // Horizontal header indices
    // TODO: use fmt::join here
    for (const auto i : board.indices) {
        out << " " << i;
    }
    for (const auto y : board.indices) {
        // Vertical header index
        out << "\n" << y;
        // Output row
        for (const auto x : board.indices) {
            auto disk = board.board[y * board.size + x];
            out << " " << board_char(disk);
        }
    }
    return out;
}
}  // namespace othello
