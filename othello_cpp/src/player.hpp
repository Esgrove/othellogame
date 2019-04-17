//==========================================================
// Class Player header
// Defines one player for Othello
// Akseli Lukkarila
//==========================================================

#pragma once
#include "othello.hpp"
#include "util.hpp"
//==========================================================

enum PlayType {
    HUMAN = 100,
    COMPUTER
};
//==========================================================

class Player {
public:
    Player(Color col) : rounds(0), m_color(col), m_type(HUMAN) {};
    Player(Color col, PlayType type) : rounds(0), m_color(col), m_type(type) {};
    ~Player() {};
    //------------------------------------------------------

    Color           color() const { return m_color; }
    std::string     colorString() const { return getColorString(this->m_color); }
    std::string     colorStringUpper() const;

    PlayType        type() const { return m_type; }
    std::string     typeString() const;
    void            setType(PlayType type) { m_type = type; }
    bool            human() const { return m_type == HUMAN; }

    void            playOneMove(Othello& game);
    void            printMoves(const std::vector<Move>& moves);
    //------------------------------------------------------

    int             rounds;
    bool            canPlay = true;
    //------------------------------------------------------

    friend std::ostream& operator<< (std::ostream& out, Player& p);
    //------------------------------------------------------

private:
    Color           m_color;
    PlayType        m_type;
};