//==========================================================
// Class Player source
// Defines one player for Othello
// Akseli Lukkarila
//==========================================================

#include <algorithm>
#include <iterator>
#include <random>

#include "player.hpp"
//==========================================================

std::string Player::colorStringUpper() const
{
    std::string color = getColorString(m_color);
    std::transform(color.begin(), color.end(), color.begin(), ::toupper);
    return color;
}
//----------------------------------------------------------

std::string Player::typeString() const
{
    return m_type == HUMAN ? std::string("Human") : std::string("Computer");
}
//----------------------------------------------------------

void Player::playOneMove(Othello& game) {
    auto moves = game.getPossibleMoves(m_color);
    if (moves.size() > 0) {
        this->canPlay = true;
        if (this->human()) {
            // sort moves in descending order according to value
            std::sort(moves.begin(), moves.end());
            printMoves(moves);
            std::string input;
            while (true) {
                std::cout << "  Give disk position (x,y): ";
                std::cin >> input;
                if (input.size() == 3 && input[1] == ',') {
                    int x = input[0] - '0'; // ascii hack
                    int y = input[2] - '0'; // std::stoi(input.substr(2,1));
                    if (game.placeDisc(x, y, this->color())) {
                        break;
                    }
                    else {
                        std::cout << "  Error: can't place a " << colorString() << " disk in square (" << x << "," << y << ").\n";
                    }
                }
                else {
                    print("  Error: give coordinates in the form \"x,y\".");
                }
            }
        }
        else {
            // computer plays: pick a random move
            print("  Computer plays...");
            std::random_device random_device;
            std::mt19937 engine{ random_device() };
            std::uniform_int_distribution<int> dist(0, moves.size() - 1);
            auto pos = moves[dist(engine)].square;
            game.placeDisc(pos.x, pos.y, m_color);
        }
        ++this->rounds;
        print("\nResult:");
        std::cout << game;
    }
    else {
        print("  No moves available...");
        this->canPlay = false;
    }
    print("-------------------------------");
}
//----------------------------------------------------------

void Player::printMoves(const std::vector<Move>& moves) {
    std::cout << "  Possible plays (" << moves.size() << "):\n";
    for (const Move& move : moves) {
        std::cout << "  " << move << "\n";
    }
}
//----------------------------------------------------------

std::ostream& operator<<(std::ostream& out, Player& p) {
    out << p.colorStringUpper() << " (" << p.typeString() << ")" << " // moves: " << p.rounds << std::endl;
    return out;
}
//----------------------------------------------------------
