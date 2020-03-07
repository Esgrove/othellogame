//==========================================================
// Class Othello source
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#include <algorithm> // std::clamp
#include <iostream>

#include "colorprint.hpp"
#include "othello.hpp"

game::Othello::Othello() : rounds{0}
{
    int board_size = get_board_size();
    board = Board(board_size);
    player_black = Player(BLACK);
    player_white = Player(WHITE);

    auto play_against_computer = get_answer("Would you like to play against the computer (y/n)? ");
    if (play_against_computer == "y") {
        auto side = get_answer("Would you like to play as black or white (b/w)? ");
        if (side == "b") {
            player_white.set_human(false);
        } else {
            player_black.set_human(false);
        }
    }
    print("\nPlayers:");
    print(player_black, false);
    print(player_white);
    print(board);
}

/// Read user input.
std::string game::Othello::get_answer(const std::string& text)
{
    std::string input;
    std::cout << text;
    std::cin >> input;
    return input;
}

/// Ask the desired board size from user.
int game::Othello::get_board_size()
{
    int size;
    std::cout << "Choose board size (default is 8): ";
    std::cin >> size;
    return std::clamp(size, 4, 8); // C++17, size = std::max(4, std::min(size, 8));
}

/// Play one full game of Othello.
void game::Othello::play_game()
{
    while (player_black.can_play || player_white.can_play) {
        std::cout << "=========== ROUND: " << std::to_string(rounds + 1) << " ===========\n";
        player_black.play_one_move(board);
        player_white.play_one_move(board);
        board.print_score();
        ++rounds;
    }

    print_color("The game is finished!", Color::GREEN);
    print("total rounds played: ", false);
    print(std::to_string(rounds));
    print("Result:");
    print(board);
    print(player_black, false);
    print(player_white);

    Disk winner = board.get_result();
    if (winner == WHITE) {
        print_color("The white player won!", Color::MAGENTA);
    } else if (winner == BLACK) {
        print_color("The black player won!", Color::CYAN);
    } else {
        print("The game ended in a tie...");
    }
}

int main() {
    print_color("OTHELLO GAME - C++\n", Color::GREEN);
    game::Othello othello;
    othello.play_game();
}
