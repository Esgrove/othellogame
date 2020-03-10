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

othello::Board::Board(int dim) : dim(dim), size(dim * dim) {
    // init board
    board.resize(size, Disk::EMPTY);
    int row = dim % 2 == 0 ? (dim-1)/2 : (dim-1)/2 - 1;
    int col = dim / 2;
    board[row * dim + row] = Disk::BLACK;
    board[row * dim + col] = Disk::WHITE;
    board[col * dim + row] = Disk::WHITE;
    board[col * dim + col] = Disk::BLACK;
}

/// Check can the given disk color be placed in the given position
bool othello::Board::can_place_to_square(const int x, const int y, const Disk color) const {
    if (square(x, y) == Disk::EMPTY) {
        Disk other = other_disk(color);
        for (auto dir : directions) {
            int tx = x + dir[0];
            int ty = y + dir[1];
            if (square(tx, ty) == other) {
                while (square(tx, ty) == other) {
                    tx += dir[0];
                    ty += dir[1];
                }
                if (square(tx, ty) == color) {
                    return true;
                }
            }
        }
    }
    return false;
}

/// Check that the given coordinates are inside the board
bool othello::Board::check_coordinates(const int x, const int y) const {
    return ( x >= 0 && x < dim) && ( y >= 0 && y < dim );
}

/// Returns a list of possible moves with their value
std::vector<othello::Move> othello::Board::get_possible_moves(Disk color) const {
    std::vector<Move> moves;
    Disk other = other_disk(color);
    for (int y = 0; y < dim; ++y) {
        for (int x = 0; x < dim; ++x) {
            if (square(x, y) == Disk::EMPTY) {
                int value = 0;
                for (auto dir : directions) {
                    int tx = x + dir[0];
                    int ty = y + dir[1];
                    if (square(tx, ty) == other) {
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
                }
                if (value > 0) {
                    moves.emplace_back(Move(x, y, value));
                }
            }
        }
    }
    return moves;
}

/// Calculates the final score and returns the winner color
othello::Disk othello::Board::get_result() const {
    int sum = std::accumulate(board.begin(), board.end(), 0);
    if (sum > 0) {
        return Disk::WHITE;
    } else if (sum < 0) {
        return Disk::BLACK;
    }
    return Disk::EMPTY;
}

/// Returns the state of the board (empty, white, black) at the given coordinates
othello::Disk othello::Board::square(const int x, const int y) const {
    if (check_coordinates(x, y)) {
        return board[y * dim + x];
    } else {
        return Disk::INVALID;
    }
}

/// Sets the given square to given value
bool othello::Board::set_square(int x, int y, Disk disk) {
    if (check_coordinates(x, y)) {
        board[y * dim + x] = disk;
        return true;
    }
    return false;
}

/// Tries to place the given disk color to the given square
bool othello::Board::place_disc(int x, int y, Disk color) {
    if (can_place_to_square(x, y, color)) {
        Disk other = other_disk(color);
        for (auto dir : directions) {
            int tx = x + dir[0];
            int ty = y + dir[1];
            if (square(tx, ty) == other) {
                while (square(tx, ty) == other) {
                    tx += dir[0];
                    ty += dir[1];
                }
                if (square(tx, ty) == color) {
                    while ( (tx != x) || (ty != y) ) {
                        tx -= dir[0];
                        ty -= dir[1];
                        set_square(tx, ty, color);
                    }
                }
            }
        }
        return true;
    }
    return false;
}

/// Print current score for both players
void othello::Board::print_score() const {
    int black = 0; // std::accumulate(board.begin(), board.end(), 0, [](Disk col){return col == WHITE; })
    int white = 0; // std::accumulate(board.begin(), board.end(), 0, [](Disk col){return col == BLACK; })
    for (auto& col : board) {
        if (col == Disk::WHITE) {
            ++white;
        } else if (col == Disk::BLACK) {
            ++black;
        }
    }
    std::cout << "Score: " << termcolor::magenta << std::to_string(white) << termcolor::reset;
    std::cout << " | ";
    std::cout << termcolor::cyan << std::to_string(black) << termcolor::reset << "\n\n";
}

/// Print board
std::ostream& othello::operator<<(std::ostream& out, const Board& othello)
{
    out << " ";
    for (int i = 0; i < othello.dim; ++i) {
        out << " " + std::to_string(i);
    }
    out << "\n";
    for (int y = 0; y < othello.dim; ++y) {
        out << std::to_string(y);
        for (int x = 0; x < othello.dim; ++x) {
            auto disk = othello.board[y * othello.dim + x];
            print_color(" " + get_board_char(disk), disk_color(disk));
        }
        out << "\n";
    }
    out << "\n";
    return out;
}
