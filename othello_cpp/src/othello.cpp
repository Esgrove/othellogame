//==========================================================
// Class Othello source
// Othello (Reversi) game
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#include <algorithm> // std::clamp
#include <iostream>

#include "colorprint.hpp"
#include "othello.hpp"

game::Othello::Othello()
{
    int boardSize = getBoardSize();
    board = Board(boardSize);
    player_black = Player(BLACK);
    player_white = Player(WHITE);

    auto answer = getAnswer("Would you like to play against the computer (y/n)? ");
    if (answer == "y") {
        answer = getAnswer("Would you like to play as black or white (b/w)? ");
        if (answer == "b") {
            player_white.set_human(false);
        }
        else {
            player_black.set_human(false);
        }
    }
    print("\nPlayers:");
    print(player_black, false);
    print(player_white);
    print(board);
}

std::string game::Othello::getAnswer(const std::string& text)
{
    std::string input;
    std::cout << text;
    std::cin >> input;
    return input;
}

int game::Othello::getBoardSize()
{
    int size;
    std::cout << "Choose board size (default is 8): ";
    std::cin >> size;
    return std::clamp(size, 4, 8); // C++17, size = std::max(4, std::min(size, 8));
}

void game::Othello::playGame()
{
    while (player_black.canPlay || player_white.canPlay) {
        player_black.play_one_move(board);
        player_white.play_one_move(board);
        board.print_score();
    }

    print("The game is finished!");
    print("Result:");
    print(board);
    print(player_black, false);
    print(player_white);

    Disk winner = board.get_result();
    if (winner == WHITE) {
        print_color("The white player won!", Color::MAGENTA);
    }
    else if (winner == BLACK) {
        print_color("The black player won!", Color::CYAN);
    }
    else {
        print("The game ended in a tie...");
    }
}
