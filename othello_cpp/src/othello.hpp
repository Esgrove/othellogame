//==========================================================
// Class Othello header
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#pragma once
#include <vector>
#include "util.hpp"
//==========================================================

class Othello {

public:
    static const int directions[8][2];

    Othello(int dim);
    ~Othello();

    bool                canPlaceDisc(const Color color);
    bool                canPlaceToSquare(const int rx, const int ry, const Color color);
    bool                checkCoordinates(const int x, const int y);

    Color               getOtherColor(const Color color);
    std::vector<Move>   getPossibleMoves(const Color color);
    Color               getResult();
    Color               getSquare(const int x, const int y);
    bool                setSquare(const int x, const int y, const Color disk);

    bool                placeDisc(const int rx, const int ry, const Color color);
    void                printBoard();
    void                printScore();

    friend std::ostream& operator<< (std::ostream& out, const Othello& othello);

private:
    std::vector<Color>  board;
    int        dim;
    int        size;
};
