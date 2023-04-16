//==========================================================
// Class Othello source
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
//==========================================================

#include "othello.hpp"

#include "colorprint.hpp"
#include "version.hpp"

#include <algorithm>  // clamp, transform
#include <iostream>

othello::Othello::Othello(size_t board_size) : board(Board(board_size)), board_size(board_size) {}

/// Play one full game of Othello.
void othello::Othello::play()
{
    while (true) {
        init_game();
        game_loop();
        print_result();
        if (!get_answer("\nWould you like to play again")) {
            break;
        }
    }
}

/// Initialize game board and players for a new game.
void othello::Othello::init_game()
{
    if (games_played > 0) {
        board = Board(this->board_size);
        player_black.reset();
        player_white.reset();
        rounds_played = 0;
    }

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

/// Keep making moves until both players can't make a move anymore.
void othello::Othello::game_loop()
{
    while (board.can_play() && (player_black.can_play || player_white.can_play)) {
        ++rounds_played;
        fmt::print(fmt::emphasis::bold, "\n=========== ROUND: {} ===========\n", rounds_played);
        player_black.play_one_move(board);
        print("--------------------------------");
        player_white.play_one_move(board);
    }
    ++games_played;
}

/// Print ending status and winner info.
void othello::Othello::print_result() const
{
    print_bold("\n================================\n");
    print_color("The game is finished!\n", fmt::color::green);
    print("Result:");
    print_status();
    print("");

    Disk winner = board.result();
    if (winner == Disk::empty) {
        print("The game ended in a tie...");
    } else {
        print(fmt::format("The winner is {}!", disk_string(winner)));
    }
}

/// Print current board and player info.
void othello::Othello::print_status() const
{
    print(player_black);
    print(player_white);
    print("");
    print(board);
}

/// Read user input for yes/no question and return bool.
bool othello::Othello::get_answer(
    const std::string& question,
    const std::string& yes,
    const std::string& no)
{
    // fmt library enables nice modern) string formatting,
    // instead of having to use the horrible stringstream version:
    // `std::cout << question << " (" << yes << "/" << no << ")? ";`
    fmt::print("{} ({}/{})? ", question, yes, no);
    std::string input;
    std::cin >> input;
    // TODO: replace with std::ranges:transform
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    return input == yes;
}

/// Ask the desired board size from user.
size_t othello::Othello::get_board_size()
{
    // TODO: error handling for invalid input
    print("Choose board size (default is 8): ", false);
    int size;
    std::cin >> size;
    return static_cast<size_t>(std::clamp(size, 4, 16));
}

int main(int argc, const char* argv[])
{
    print_bold("OTHELLO GAME - C++\n", fmt::color::green);
    std::vector<std::string> arguments(argv, argv + argc);

    // Handle 'help' and 'version' arguments
    for (const auto& arg : arguments) {
        if (arg == "--help" || arg == "-h") {
            fmt::print(
                "{} {} {}\n\n"
                "USAGE: othello_cpp [board size]\n\n"
                "Optional arguments:\n"
                "    -h | --help          Print usage and exit\n"
                "    -v | --version       Print version info and exit\n",
                version::APP_NAME,
                version::VERSION_NUMBER,
                version::DATE);
            return 1;
        } else if (arg == "--version" || arg == "-v") {
            fmt::print(
                "{} {} {} {} {}\n",
                version::APP_NAME,
                version::VERSION_NUMBER,
                version::DATE,
                version::BRANCH,
                version::COMMIT);
            return 0;
        }
    }

    // Try to read board size from command line args
    size_t board_size;
    try {
        if (arguments.size() >= 2) {
            board_size = std::stoi(arguments[1]);
            if (board_size < 4 || board_size > 16) {
                print_error(fmt::format("Unsupported board size: {}", board_size));
                return 1;
            }
            fmt::print("Using board size: {}\n", board_size);
        } else {
            throw std::invalid_argument("Invalid board size");
        }
    } catch (const std::invalid_argument&) {
        // Otherwise ask user for size
        board_size = othello::Othello::get_board_size();
    }

    othello::Othello game {board_size};
    game.play();
    return 0;
}
