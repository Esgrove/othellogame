//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================
#include <algorithm>

#include "othello.hpp"
#include "player.hpp"
//==========================================================

int main() {
    print("OTHELLO GAME\n");

    int size;
    std::cout << "Choose board size (4...8, default is 8): ";
    std::cin >> size;
    size = std::max(4, std::min(size, 8));
    Othello board(size);

    Player playerOne(BLACK);
    Player playerTwo(WHITE);
    std::cout << "Would you like to play against the computer (y/n)? ";
    std::string input;
    std::cin >> input;
    if (input == "y") {
        std::cout << "Would you like to play as black or white (b/w)? ";
        std::cin >> input;
        if (input == "b") {
            playerTwo.setType(COMPUTER);
        }
        else {
            playerOne.setType(COMPUTER);
        }
    }

    print("\nPlayers:");
    std::cout << playerOne;
    std::cout << playerTwo << "\n";

    print(board);
    while (playerOne.canPlay || playerTwo.canPlay) {
        std::cout << "Turn: " << playerOne.colorStringUpper() << "\n";
        playerOne.playOneMove(board);

        std::cout << "Turn: " << playerTwo.colorStringUpper() << "\n";
        playerTwo.playOneMove(board);

        board.printScore();
    }
    print("The game is finished!");
    print("Result:");
    print(board);

    std::cout << playerOne;
    std::cout << playerTwo << "\n";

    Color winner = board.getResult();
    if (winner == WHITE) { 
        print("The white player won!");
    } else if (winner == BLACK) { 
        print("The black player won!");
    } else { 
        print("The game endend in a tie...");
    }
}
//==========================================================
