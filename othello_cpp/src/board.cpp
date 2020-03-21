//==========================================================
// Class Board source
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#include <numeric>  // std::accumulate

#include "board.hpp"
#include "colorprint.hpp"
#include "util.hpp"

othello::Board::Board(int size) : size_(size) {
    board_.resize(size * size, Disk::EMPTY);
    // set starting positions
    int row = size_ % 2 == 0 ? (size_ - 1) / 2 : (size_ - 1) / 2 - 1;
    int col = size_ / 2;
    board_[row * size_ + row] = Disk::BLACK;
    board_[row * size_ + col] = Disk::WHITE;
    board_[col * size_ + row] = Disk::WHITE;
    board_[col * size_ + col] = Disk::BLACK;
}

/// Return true if board contains empty squares
bool othello::Board::can_play() const {
    return std::find(board_.begin(), board_.end(), Disk::EMPTY) != board_.end();
}

/// Check can the given disk color be placed in the given position
bool othello::Board::can_place_to_square(const int x, const int y, const Disk color) const {
    if (square(x, y) == Disk::EMPTY) {
        Disk other = other_disk(color);
        for (auto& dir : directions) {
            int tx = x + dir[0];
            int ty = y + dir[1];
            // next square in this directions should contain opponents disk
            if (square(tx, ty) != other) continue;
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
    return ( x >= 0 && x < size_) && (y >= 0 && y < size_ );
}

/// Returns a list of possible moves with their value
std::vector<othello::Move> othello::Board::possible_moves(Disk color) const {
    std::vector<Move> moves;
    Disk other = other_disk(color);
    for (int y = 0; y < size_; ++y) {
        for (int x = 0; x < size_; ++x) {
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
    print_color("  Possible plays (" + std::to_string(moves.size()) + "):\n", Color::YELLOW);
    for (const Move& move : moves) {
        std::cout << "  " << move << "\n";
    }
}

/// Print current score for both players.
void othello::Board::print_score() const {
    int black = 0, white = 0;
    for (auto& disk : board_) {
        if (disk == Disk::WHITE) {
            ++white;
        } else if (disk == Disk::BLACK) {
            ++black;
        }
    }
    print("Score: ", false);
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
    return std::accumulate(board_.begin(), board_.end(), 0,[](int s, Disk d) {
            return s + static_cast<int>(d);
    });
}

/// Sets the given square to given value.
bool othello::Board::set_square(int x, int y, Disk disk) {
    if (!check_coordinates(x, y)) {
        return false;
    }
    board_[y * size_ + x] = disk;
    return true;
}

/// Returns the state of the board (empty, white, black) at the given coordinates.
othello::Disk othello::Board::square(const int x, const int y) const {
    return check_coordinates(x, y) ? board_[y * size_ + x] : Disk::INVALID;
}

/// Print board
std::ostream& othello::operator<<(std::ostream& out, const Board& othello) {
    out << " ";
    for (int i = 0; i < othello.size_; ++i) {
        out << " " + std::to_string(i);
    }
    for (int y = 0; y < othello.size_; ++y) {
        out << "\n" + std::to_string(y);
        for (int x = 0; x < othello.size_; ++x) {
            auto disk = othello.board_[y * othello.size_ + x];
            print_color(" " + board_char(disk), disk_color(disk), out);
        }
    }
    return out;
}
