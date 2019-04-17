//==========================================================
// Class Othello source
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#include <numeric>  // std::accumulate

#include "othello.hpp"
//==========================================================

const int Othello::directions[8][2] = {{0,-1}, {1, 0}, {0, 1}, {-1, 0}, {1,-1}, {1, 1}, {-1, 1}, {-1,-1}};
//----------------------------------------------------------

Othello::Othello(int dim) : dim(dim), size(dim * dim) {
    // init board
    board.resize(size, EMPTY);
    int row = (dim - 1) / 2;
    int col = dim / 2;
    board[row * dim + row] = BLACK;
    board[row * dim + col] = WHITE;
    board[col * dim + row] = WHITE;
    board[col * dim + col] = BLACK;
}
//----------------------------------------------------------

Othello::~Othello() {
}
//----------------------------------------------------------

/// Check can the given disk color be placed in the given position
bool Othello::canPlaceToSquare(const int rx, const int ry, const Color color) {
    if (getSquare(rx, ry) == EMPTY) {
        Color other = getOtherColor(color);
        for (auto dir : directions) {
            int x = rx + dir[0];
            int y = ry + dir[1];
            if (getSquare(x, y) == other) {
                while (getSquare(x, y) == other) {
                    x += dir[0];
                    y += dir[1];
                }
                if (getSquare(x, y) == color) {
                    return true;
                }
            }
        }
    }
    return false;
}
//----------------------------------------------------------

/// Check if there are possible moves for the given color
bool Othello::canPlaceDisc(const Color color) {
    for (int y = 0; y < dim; y++) {
        for (int x = 0; x < dim; x++) {
            if (canPlaceToSquare(x, y, color)) {
                return true;
            }
        }
    }
    return false;
}
//----------------------------------------------------------

/// Check that the given coordinates are inside the board
bool Othello::checkCoordinates(const int x, const int y) {
    return ( x >= 0 && x < dim) && ( y >= 0 && y < dim );
}
//----------------------------------------------------------

/// Returns a list of possible moves with their value
std::vector<Move>  Othello::getPossibleMoves(const Color color) {
    std::vector<Move> moves;
    Color other = getOtherColor(color);
    for (int ry = 0; ry < dim; ++ry) {
        for (int rx = 0; rx < dim; ++rx) {
            if (getSquare(rx, ry) == EMPTY) {
                int value = 0;
                for (auto dir : directions) {
                    int x = rx + dir[0];
                    int y = ry + dir[1];
                    if (getSquare(x, y) == other) {
                        int steps = 0;
                        while (getSquare(x, y) == other) {
                            x += dir[0];
                            y += dir[1];
                            ++steps;
                        }
                        if (getSquare(x, y) == color) {
                            value += steps;
                        }
                    }
                }
                if (value > 0) {
                    moves.emplace_back(Move(rx, ry, value));
                }
            }
        }
    }
    return moves;
}
//----------------------------------------------------------

/// Calculates the final score and returns the winner color
Color Othello::getResult() {
    int sum = std::accumulate(board.begin(), board.end(), 0);
    if (sum > 0) {
        return WHITE;
    }
    else if (sum < 0) {
        return BLACK;
    }
    return EMPTY;
}
//----------------------------------------------------------

/// Returns the state of the board (empty, white, black) at the given coordinates
Color Othello::getSquare(const int x, const int y) {
    if (checkCoordinates(x,y)) {
        return board[y * dim + x];
    } else {
        return ERROR;
    }
}
//----------------------------------------------------------

/// Sets the given square to given value
bool Othello::setSquare(const int x, const int y, const Color disk) {
    if (checkCoordinates(x, y)) {
        board[y * dim + x] = disk;
        return true;
    }
    return false;
}
//----------------------------------------------------------

/// Returns the opponents color
Color Othello::getOtherColor(const Color color) {
    return color == WHITE ? BLACK : WHITE;
}
//----------------------------------------------------------

/// Tries to place the given disk color to the given square
bool Othello::placeDisc(const int rx, const int ry, const Color color) {
    if (canPlaceToSquare(rx, ry, color)) {
        Color other = getOtherColor(color);
        for (auto dir : directions) {
            int x = rx + dir[0]; 
            int y = ry + dir[1];
            if (getSquare(x, y) == other) {
                while (getSquare(x, y) == other) {
                    x += dir[0];
                    y += dir[1];
                }
                if (getSquare(x, y) == color) {
                    while ( (x != rx) || (y != ry) ) {
                        x -= dir[0];
                        y -= dir[1];
                        board[y * dim + x] = color;
                    }
                }
            }
        }
        return true;
    }
    return false;
}
//----------------------------------------------------------

/// Print current situation
void Othello::printBoard() {
    std::cout << *this;
}
//----------------------------------------------------------

/// Print current score for both playes
void Othello::printScore() {
    int black = 0; // std::accumulate(board.begin(), board.end(), 0, [](Color col){return col == WHITE; })
    int white = 0; // std::accumulate(board.begin(), board.end(), 0, [](Color col){return col == BLACK; })
    for (auto& col : board) {
        if (col == WHITE) {
            ++white;
        }
        else if (col == BLACK) {
            ++black;
        }
    }
    std::cout << "Score: W = " << white << ", B = " << black << "\n" << std::endl;
}
//----------------------------------------------------------

/// Print board
std::ostream& operator<<(std::ostream& out, const Othello& othello)
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
            out << " " + getBoardString(color);
        }
        out << "\n";
    }
    out << "\n";
    return out;
}
//----------------------------------------------------------
