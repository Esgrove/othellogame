//==========================================================
// Class Board source
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#include <numeric>  // std::accumulate

#include "board.hpp"
#include "colorprint.hpp"

game::Board::Board(int dim) : dim(dim), size(dim * dim) {
    // init board
    board.resize(size, EMPTY);
    int row = (dim - 1) / 2;
    int col = dim / 2;
    board[row * dim + row] = BLACK;
    board[row * dim + col] = WHITE;
    board[col * dim + row] = WHITE;
    board[col * dim + col] = BLACK;
}

/// Check can the given disk color be placed in the given position
bool game::Board::can_place_to_square(const int x, const int y, const Disk color) {
    if (square(x, y) == EMPTY) {
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
bool game::Board::check_coordinates(const int x, const int y) {
    return ( x >= 0 && x < dim) && ( y >= 0 && y < dim );
}

/// Returns a list of possible moves with their value
std::vector<game::Move> game::Board::get_possible_moves(Disk color) {
    std::vector<Move> moves;
    Disk other = other_disk(color);
    for (int y = 0; y < dim; ++y) {
        for (int x = 0; x < dim; ++x) {
            if (square(x, y) == EMPTY) {
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
game::Disk game::Board::get_result() {
    int sum = std::accumulate(board.begin(), board.end(), 0);
    if (sum > 0) {
        return WHITE;
    }
    else if (sum < 0) {
        return BLACK;
    }
    return EMPTY;
}

/// Returns the state of the board (empty, white, black) at the given coordinates
game::Disk game::Board::square(const int x, const int y) {
    if (check_coordinates(x, y)) {
        return board[y * dim + x];
    } else {
        return ERROR;
    }
}

/// Sets the given square to given value
bool game::Board::set_square(int x, int y, Disk disk) {
    if (check_coordinates(x, y)) {
        board[y * dim + x] = disk;
        return true;
    }
    return false;
}

/// Returns the opponents color
game::Disk game::Board::other_disk(Disk color) {
    return color == WHITE ? BLACK : WHITE;
}

/// Tries to place the given disk color to the given square
bool game::Board::place_disc(int x, int y, Disk color) {
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
void game::Board::print_score() {
    int black = 0; // std::accumulate(board.begin(), board.end(), 0, [](Disk col){return col == WHITE; })
    int white = 0; // std::accumulate(board.begin(), board.end(), 0, [](Disk col){return col == BLACK; })
    for (auto& col : board) {
        if (col == WHITE) {
            ++white;
        }
        else if (col == BLACK) {
            ++black;
        }
    }
    std::cout << "Score: " << termcolor::magenta << std::to_string(white) << termcolor::reset;
    std::cout << " | ";
    std::cout << termcolor::cyan << std::to_string(black) << termcolor::reset << "\n\n";
}

/// Print board
std::ostream& game::operator<<(std::ostream& out, const Board& othello)
{
    out << " ";
    for (int i = 0; i < othello.dim; ++i) {
        out << " " + std::to_string(i);
    }
    out << "\n";
    for (int y = 0; y < othello.dim; ++y) {
        out << std::to_string(y);
        for (int x = 0; x < othello.dim; ++x) {
            auto color = othello.board[y * othello.dim + x];
            if (color == Disk::BLACK) {
                out << termcolor::cyan;
            }
            else if (color == Disk::WHITE) {
                out << termcolor::magenta;
            }
            out << " " + get_board_string(color) << termcolor::reset;
        }
        out << "\n";
    }
    out << "\n";
    return out;
}
