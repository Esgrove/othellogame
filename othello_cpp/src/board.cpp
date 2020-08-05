//==========================================================
// Class Board source
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#include <algorithm>  // std::remove
#include <numeric>    // std::accumulate, std::iota

#include "board.hpp"
#include "colorprint.hpp"

othello::Board::Board(int size) : size(size), indices(size) {
    // init game board with empty disks.
    board.resize(size * size, Disk::Empty);

    // set starting positions
    auto row = size % 2 == 0 ? (size - 1) / 2 : (size - 1) / 2 - 1;
    auto col = size / 2;
    board[row * size + row] = Disk::Black;
    board[row * size + col] = Disk::White;
    board[col * size + row] = Disk::White;
    board[col * size + col] = Disk::Black;

    // index list (0...size) to avoid repeating same range in for loops
    std::iota(indices.begin(), indices.end(), 0);

    // keep track of empty squares on board to avoid checking already filled positions
    for (int y : indices) {
        for (int x : indices) {
            if (board[y * size + x] == Disk::Empty) {
                empty_squares.emplace(Square(x, y));
            }
        }
    }
}

/// Return true if board contains empty squares.
bool othello::Board::can_play() const {
    // std::find(board.begin(), board.end(), Disk::EMPTY) != board.end();
    return !empty_squares.empty();
}

/// Check that the given coordinates are inside the board.
bool othello::Board::check_coordinates(const int x, const int y) const {
    return 0 <= x && x < size && 0 <= y && y < size;
}

/// Update board for given disk placement.
void othello::Board::place_disc(const othello::Move& move) {
    auto start = move.square;
    if (get_square(start.x, start.y) != Disk::Empty) {
        throw std::invalid_argument(fmt::format("Trying to place disk to an occupied square {}!", start));
    }
    set_square(start.x, start.y, move.disk);
    empty_squares.erase(start);
    for (auto& dir : move.directions) {
        Square pos = start + dir;
        while (get_square(pos.x, pos.y) == other_disk(move.disk)) {
            set_square(pos.x, pos.y, move.disk);
            pos += dir;
        }
    }
}

/// Returns a list of possible moves for given player.
std::vector<othello::Move> othello::Board::possible_moves(Disk disk) const {
    std::vector<Move> moves;
    Disk other = other_disk(disk);
    for (const Square& square : empty_squares) {
        int value {0};
        std::vector<Square> directions;
        for (auto& dir : step_directions) {
            Square step {Square(dir[0], dir[1])};
            Square pos {square + step};
            // next square in this directions needs to be opponents disk
            if (get_square(pos.x, pos.y) != other) {
                continue;
            }
            int steps {0};
            // keep stepping forward while opponents disks are found
            while (get_square(pos.x, pos.y) == other) {
                ++steps;
                pos += step;
            }
            // valid move if a line of opponents disks ends in own disk
            if (get_square(pos.x, pos.y) != disk) {
                continue;
            }
            value += steps;
            directions.emplace_back(step);
        }
        if (value > 0) {
            moves.emplace_back(Move(square, value, disk, directions));
        }
    }
    if (!moves.empty()) {
        std::sort(moves.begin(), moves.end());
    }
    return moves;
}

void othello::Board::print_possible_moves(const std::vector<Move>& moves) {
    fmt::print(fmt::fg(fmt::color::yellow), "  Possible plays ({}):\n", std::to_string(moves.size()));
    std::vector<std::string> board_str;
    board_str.reserve(board.size());
    for (auto& disk : board) {
        board_str.emplace_back(fmt::format(fmt::fg(disk_color(disk)), "{}", board_char(disk)));
    }
    for (const Move& move : moves) {
        fmt::print("  {}\n", move);
        board_str[move.square.y * size + move.square.x] = get_color(move.value, fmt::color::yellow);
    }
    print("   ", false);
    for (int i : indices) {
        fmt::print(" {}", i);
    }
    for (int y : indices) {
        fmt::print("\n  {}", y);
        for (int x : indices) {
            fmt::print(" {}", board_str[y * size + x]);
        }
    }
    print("");
}

/// Print current score for both players.
void othello::Board::print_score() const {
    auto [black, white] = player_scores();
    print("");
    print(*this);
    fmt::print("Score: ");
    print_color(std::to_string(black), disk_color(Disk::Black));
    print(" | ", false);
    print_color(std::to_string(white) + "\n", disk_color(Disk::White));
}

/// Count and return the number of black and white disks.
std::tuple<int, int> othello::Board::player_scores() const {
    int black = 0, white = 0;
    for (auto& disk : board) {
        switch (disk) {
            case Disk::White:
                ++white;
                break;
            case Disk::Black:
                ++black;
                break;
            default:
                break;
        }
    }
    return std::tuple<int, int>(black, white);
}

/// Calculates the final score and returns the winner color.
othello::Disk othello::Board::result() const {
    int sum = score();
    if (sum == 0) {
        return Disk::Empty;
    }
    return sum > 0 ? Disk::White : Disk::Black;
}

/// Returns the total score (positive means more white disks and negative means more black disks).
int othello::Board::score() const {
    // enum class prevents implicit conversion to int, so have to use lambda to cast Disk values to int
    return std::accumulate(board.begin(), board.end(), 0, [](int s, Disk d) {
            return s + static_cast<int>(d);
    });
}

/// Sets the given square to given value.
void othello::Board::set_square(const Square& square, Disk disk) {
    if (!check_coordinates(square.x, square.y)) {
        throw std::invalid_argument(fmt::format("Invalid coordinates ({},{})!", x, y));
    }
    board[square.y * size + square.x] = disk;
}

/// Returns the state of the board (empty, white, black) at the given coordinates.
std::optional<othello::Disk> othello::Board::get_square(const Square& square) const {
    return check_coordinates(square.x, square.y) ? std::optional<Disk>{board[square.y * size + square.x]} : std::nullopt;
}

/// Format game board to string
std::ostream& othello::operator<<(std::ostream& out, const othello::Board& board) {
    out << " ";
    for (int i : board.indices) {
        out << " " << i;
    }
    for (int y : board.indices) {
        out << "\n" << y;
        for (int x : board.indices) {
            auto disk = board.board[y * board.size + x];
            out << fmt::format(fmt::fg(disk_color(disk)), " {}", board_char(disk));
        }
    }
    return out;
}
