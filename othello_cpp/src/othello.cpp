//==========================================================
// Class Othello source
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#include <algorithm> // clamp, transform
#include <iostream>

#include <fmt/color.h>

#include "colorprint.hpp"
#include "othello.hpp"

/// Read user input for yes/no question and return bool.
bool othello::Othello::get_answer(const std::string& question, const std::string& yes, const std::string& no) {
    // fmt lib enables nice (modern) string formatting instead of having to use the horrible stringstream stuff
    // std::cout << question << " (" << yes << "/" << no << ")? ";
    fmt::print("{} ({}/{})? ", question, yes, no);
    std::string input;
    std::cin >> input;
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    return input == yes;
}

/// Ask the desired board size from user.
int othello::Othello::get_board_size() {
    print("Choose board size (default is 8): ", false);
    int size;
    std::cin >> size;
    return std::clamp(size, 4, 8);  // C++17, size = std::max(4, std::min(size, 8));
}

/// Keep making moves until both players can't make a move anymore.
void othello::Othello::game_loop() {
    while (board.can_play() && (player_black.can_play() || player_white.can_play())) {
        ++rounds_played;
        fmt::print(fmt::emphasis::bold, "\n=========== ROUND: {} ===========\n", rounds_played);
        player_black.play_one_move(board);
        print("--------------------------------");
        player_white.play_one_move(board);
    }
}

/// Initialize game board and players for a new game.
void othello::Othello::init_game() {
    board = Board(board_size);
    player_black = Player(Disk::Black);
    player_white = Player(Disk::White);
    rounds_played = 0;

    if (get_answer("Would you like to play against the computer")) {
        if (get_answer("Would you like to play as black or white", "b", "w")) {
            player_white.set_human(false);
        } else {
            player_black.set_human(false);
        }
    }
    print_bold("\nPlayers:\n");
    print_status();
}

/// Play one full game of Othello.
void othello::Othello::play() {
    init_game();
    game_loop();
    print_result();
    if (get_answer("\nWould you like to play again")) {
        play();
    }
}

/// Print ending status and winner info.
void othello::Othello::print_result() {
    print_bold("\n================================\n");
    print_color("The game is finished!\n", fmt::color::green);
    print("Result:");
    print_status();
    print("");

    Disk winner = board.result();
    if (winner == Disk::Empty) {
        print("The game ended in a tie...");
    } else {
        print(fmt::format("The winner is {}!", disk_string(winner)));
    }
}

/// Print current board and player info.
void othello::Othello::print_status() {
    print(player_black);
    print(player_white);
    print("");
    print(board);
}

int main(int argc, const char* argv[]) {
    print_bold("OTHELLO GAME - C++\n", fmt::color::green);
    // try to read board size from command line args
    int board_size;
    try {
        if (argc >= 2) {
            board_size = std::stoi(argv[1]);
            fmt::print("Using board size: {}\n", board_size);
        } else {
            throw std::invalid_argument("");
        }
    } catch (const std::invalid_argument&) {
        board_size = othello::Othello::get_board_size();
    }
    othello::Othello game{board_size};
    game.play();
}
