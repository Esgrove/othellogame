//==========================================================
// Class Othello source
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#include <algorithm> // clamp, transform
#include <iostream>

#include <fmt/ostream.h>
#include <fmt/color.h>

#include "colorprint.hpp"
#include "othello.hpp"

/// Initialize board for a new game
void othello::Othello::init_game() {
    int board_size = get_board_size();
    board_ = Board(board_size);
    player_black_ = Player(Disk::BLACK);
    player_white_ = Player(Disk::WHITE);

    if (get_answer("Would you like to play against the computer")) {
        if (get_answer("Would you like to play as black or white", "b", "w")) {
            player_white_.set_human(false);
        } else {
            player_black_.set_human(false);
        }
    }
    print_bold("\nPlayers:\n");
    print(player_black_);
    print(player_white_);
    print("");
    print(board_);
}

/// Read user input for yes/no question and return bool.
bool othello::Othello::get_answer(const std::string& text, const std::string& yes, const std::string& no)
{
    std::string input;
    std::cout << text << " (" << yes << "/" << no << ")? ";
    std::cin >> input;
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    return input == yes;
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
void othello::Othello::play()
{
    while (true) {
        init_game();
        while (board_.can_play() && (player_black_.can_play() || player_white_.can_play())) {
            fmt::print(fmt::emphasis::bold, "\n=========== ROUND: {} ===========\n", rounds_played_ + 1);
            player_black_.play_one_move(board_);
            print("--------------------------------");
            player_white_.play_one_move(board_);
            ++rounds_played_;
        }

        print_color("\nThe game is finished!\n", fmt::color::green);
        print("Result:");
        print(board_);
        print(player_black_);
        print(player_white_);
        print("\n");

        Disk winner = board_.result();
        if (winner == Disk::EMPTY) {
            print("The game ended in a tie...");
        } else {
            fmt::print(fmt::fg(disk_color(winner)), "The {} player won!\n", disk_string(winner));
        }

        if (!get_answer("\nWould you like to play again")) {
            break;
        }
    }
}

int main() {
    print_bold("OTHELLO GAME: C++\n", fmt::color::green);
    othello::Othello othello;
    othello.play();
}
