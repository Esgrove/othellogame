//==========================================================
// Class Board source
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#include <algorithm>  // std::remove
#include <numeric>    // std::accumulate

#include "board.hpp"
#include "colorprint.hpp"

othello::Board::Board(int size) : size(size) {
    // init game board with empty disks.
    board.resize(size * size, Disk::EMPTY);

    // set starting positions
    auto row = size % 2 == 0 ? (size - 1) / 2 : (size - 1) / 2 - 1;
    auto col = size / 2;
    board[row * size + row] = Disk::BLACK;
    board[row * size + col] = Disk::WHITE;
    board[col * size + row] = Disk::WHITE;
    board[col * size + col] = Disk::BLACK;

    // keep track of empty squares on board to avoid checking already filled positions
    for(int y = 0; y < size; ++y) {
        for(int x = 0; x < size; ++x) {
            if(board[y * size + x] == Disk::EMPTY) {
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
    return ( x >= 0 && x < size) && (y >= 0 && y < size );
}

/// Returns a list of possible moves for given player.
std::vector<othello::Move> othello::Board::possible_moves(Disk color) const {
    std::vector<Move> moves;
    Disk other = other_disk(color);
    for (auto& pos : empty_squares) {
        int value = 0;
        std::vector<Square> directions;
        for (auto& dir : DIRECTIONS) {
            int tx = pos.x + dir[0];
            int ty = pos.y + dir[1];
            // next square in this directions needs to be opponents disk
            if (square(tx, ty) != other) {
                continue;
            }
            int steps = 0;
            // keep stepping forward while opponents disks are found
            while (square(tx, ty) == other) {
                ++steps;
                tx += dir[0];
                ty += dir[1];
            }
            // valid move if a line of opponents disks ends in own disk
            if (square(tx, ty) == color) {
                value += steps;
                directions.emplace_back(Square(dir[0], dir[1]));
            }
        }
        if (value > 0) {
            moves.emplace_back(Move(pos, value, color, directions));
        }
    }
    if (!moves.empty()) {
        std::sort(moves.begin(), moves.end());
    }
    return moves;
}

/// Tries to place the given disk color to the given square.
void othello::Board::place_disc(const othello::Move& move) {
    auto start = move.square;
    set_square(start.x, start.y, move.disk);
    empty_squares.erase(start);
    for (auto& dir : move.directions) {
        Square step = start + dir;
        while (square(step.x, step.y) == other_disk(move.disk)) {
            set_square(step.x, step.y, move.disk);
            step += dir;
        }
    }
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
    for (int i = 0; i < size; ++i) {
        fmt::print(" {}", i);
    }
    for (int y = 0; y < size; ++y) {
        fmt::print("\n  {}", y);
        for (int x = 0; x < size; ++x) {
            fmt::print(" {}", board_str[y * size + x]);
        }
    }
    print("");
}

/// Print current score for both players.
void othello::Board::print_score() const {
    int black = 0, white = 0;
    for (auto& disk : board) {
        if (disk == Disk::WHITE) {
            ++white;
        } else if (disk == Disk::BLACK) {
            ++black;
        }
    }
    print("");
    print(*this);
    fmt::print("Score: ");
    print_color(std::to_string(white), disk_color(Disk::WHITE));
    print(" | ", false);
    print_color(std::to_string(black) + "\n", disk_color(Disk::BLACK));
}

/// Calculates the final score and returns the winner color.
othello::Disk othello::Board::result() const {
    int sum = score();
    if (sum == 0) {
        return Disk::EMPTY;
    }
    return sum > 0 ? Disk::WHITE : Disk::BLACK;
}

/// Returns the total score (positive means more white disks and negative means more black disks).
int othello::Board::score() const {
    // enum class prevents implicit conversion to int, so have to use lambda to cast Disk values to int
    return std::accumulate(board.begin(), board.end(), 0, [](int s, Disk d) {
            return s + static_cast<int>(d);
    });
}

/// Sets the given square to given value.
void othello::Board::set_square(int x, int y, Disk disk) {
    if (!check_coordinates(x, y)) {
        throw std::invalid_argument(fmt::format("Invalid coordinates ({},{})!", x, y));
    }
    board[y * size + x] = disk;
}

/// Returns the state of the board (empty, white, black) at the given coordinates.
othello::Disk othello::Board::square(const int x, const int y) const {
    return check_coordinates(x, y) ? board[y * size + x] : Disk::ERROR;
}

/// Format game board to string
std::ostream& othello::operator<<(std::ostream& out, const othello::Board& board) {
    out << " ";
    for (int i = 0; i < board.size; ++i) {
        out << " " << i;
    }
    for (int y = 0; y < board.size; ++y) {
        out << "\n" << y;
        for (int x = 0; x < board.size; ++x) {
            auto disk = board.board[y * board.size + x];
            out << fmt::format(fmt::fg(disk_color(disk)), " {}", board_char(disk));
        }
    }
    return out;
}
