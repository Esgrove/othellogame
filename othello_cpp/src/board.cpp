//==========================================================
// Class Board source
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#include <numeric>  // std::accumulate

#include "board.hpp"
#include "colorprint.hpp"

othello::Board::Board(int size) : size(size) {
    board.resize(size * size, Disk::EMPTY);
    // set starting positions
    auto row = size % 2 == 0 ? (size - 1) / 2 : (size - 1) / 2 - 1;
    auto col = size / 2;
    board[row * size + row] = Disk::BLACK;
    board[row * size + col] = Disk::WHITE;
    board[col * size + row] = Disk::WHITE;
    board[col * size + col] = Disk::BLACK;

    // keep track of empty squares on board to avoid traversing already filled positions
    empty_squares.reserve(board.size() - 4);
    for(int y = 0; y < size; ++y) {
        for(int x = 0; x < size; ++x) {
            if(board[y * size + x] == Disk::EMPTY) {
                empty_squares.emplace_back(x, y);
            }
        }
    }
}

/// Return true if board contains empty squares
bool othello::Board::can_play() const {
    return std::find(board.begin(), board.end(), Disk::EMPTY) != board.end();
}

/// Check can the given disk color be placed in the given position
bool othello::Board::can_place_to_square(const int x, const int y, const Disk color) const {
    if (square(x, y) == Disk::EMPTY) {
        Disk other = other_disk(color);
        for (auto& dir : directions) {
            int tx = x + dir[0];
            int ty = y + dir[1];
            // next square in this directions should contain opponents disk
            if (square(tx, ty) != other) {
                continue;
            }
            // keep stepping forward while opponents disks are found
            while (square(tx, ty) == other) {
                tx += dir[0];
                ty += dir[1];
            }
            // valid position if the line of opponents disks ends with this players disk
            if (square(tx, ty) == color) {
                return true;
            }
        }
    }
    return false;
}

/// Check that the given coordinates are inside the board
bool othello::Board::check_coordinates(const int x, const int y) const {
    return ( x >= 0 && x < size) && (y >= 0 && y < size );
}

/// Returns a list of possible moves with their value
std::vector<othello::Move> othello::Board::possible_moves(Disk color) const {
    std::vector<Move> moves;
    Disk other = other_disk(color);
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            if (square(x, y) == Disk::EMPTY) {
                int value = 0;
                for (auto& dir : directions) {
                    int tx = x + dir[0];
                    int ty = y + dir[1];
                    if (square(tx, ty) != other) {
                        continue;
                    }
                    int steps = 0;
                    while (square(tx, ty) == other) {
                        tx += dir[0];
                        ty += dir[1];
                        ++steps;
                    }
                    if (square(tx, ty) == color) {
                        value += steps;
                    }
                }
                if (value > 0) {
                    moves.emplace_back(Move(x, y, value));
                }
            }
        }
    }
    if (!moves.empty()) {
        std::sort(moves.begin(), moves.end());
    }
    return moves;
}

/// Tries to place the given disk color to the given square.
bool othello::Board::place_disc(int x, int y, Disk color) {
    if (!can_place_to_square(x, y, color)) {
        return false;
    }
    Disk other = other_disk(color);
    for (auto& dir : directions) {
        int tx = x + dir[0];
        int ty = y + dir[1];
        if (square(tx, ty) != other) {
            continue;
        }
        while (square(tx, ty) == other) {
            tx += dir[0];
            ty += dir[1];
        }
        if (square(tx, ty) != color) {
            continue;
        }
        while ((tx != x) || (ty != y)) {
            tx -= dir[0];
            ty -= dir[1];
            set_square(tx, ty, color);
        }
    }
    return true;
}

void othello::Board::print_possible_moves(const std::vector<Move>& moves) {
    fmt::print(fmt::fg(fmt::color::yellow), "  Possible plays ({}):\n", std::to_string(moves.size()));
    for (const Move& move : moves) {
        fmt::print("  {}\n", move);
    }
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
bool othello::Board::set_square(int x, int y, Disk disk) {
    if (!check_coordinates(x, y)) {
        return false;
    }
    board[y * size + x] = disk;
    return true;
}

/// Returns the state of the board (empty, white, black) at the given coordinates.
othello::Disk othello::Board::square(const int x, const int y) const {
    return check_coordinates(x, y) ? board[y * size + x] : Disk::ERROR;
}

/// Print board
std::ostream& othello::operator<<(std::ostream& out, const Board& othello) {
    out << " ";
    for (int i = 0; i < othello.size; ++i) {
        out << " " << i;
    }
    for (int y = 0; y < othello.size; ++y) {
        out << "\n" << y;
        for (int x = 0; x < othello.size; ++x) {
            auto disk = othello.board[y * othello.size + x];
            out << fmt::format(fmt::fg(disk_color(disk)), " {}", board_char(disk));
        }
    }
    return out;
}
