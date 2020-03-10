//==========================================================
// Class Othello source
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#include <algorithm> // clamp, transform
#include <iostream>

#include "colorprint.hpp"
#include "othello.hpp"

/// Initialize board for a new game
void othello::Othello::init_game() {
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

/// Read user input and return it in lowercase.
std::string othello::Othello::get_answer(const std::string& text)
{
    std::string input;
    std::cout << text;
    std::cin >> input;
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    return input;
}

/// Ask the desired board size from user.
int othello::Othello::get_board_size()
{
    int size;
    std::cout << "Choose board size (default is 8): ";
    std::cin >> size;
    return std::clamp(size, 4, 8); // C++17, size = std::max(4, std::min(size, 8));
}

/// Play one full game of Othello.
void othello::Othello::play_game()
{
    init_game();
    while (player_black.can_play || player_white.can_play) {
        print_bold("=========== ROUND: " + std::to_string(rounds + 1) + " ===========\n");
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
        print_color("The white player won!", disk_color(WHITE));
    } else if (winner == BLACK) {
        print_color("The black player won!", disk_color(BLACK));
    } else {
        print("The game ended in a tie...");
    }
    auto play_again = get_answer("\nWould you like to play again (y/n)? ");
    if (play_again == "y") {
        play_game();
    }
}

int main() {
    print_bold("OTHELLO GAME - C++\n", Color::GREEN);
    othello::Othello othello;
    othello.play_game();
}
