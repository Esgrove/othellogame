//==========================================================
// Class Othello source
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2023
//==========================================================

#include "othello.hpp"

#include "colorprint.hpp"
#include "utils.hpp"

#include <algorithm>  // clamp, transform
#include <iostream>
#include <ranges>

namespace othello
{

Othello::Othello(const Settings settings)
    : board(Board(settings.board_size))
    , settings(settings)
    , player_black(Player::black(settings.to_player_settings()))
    , player_white(Player::white(settings.to_player_settings()))
{}

/// Play one full game of Othello.
void Othello::play()
{
    while (true) {
        init_game();
        game_loop();
        print_result();
        if (this->settings.show_log) {
            print_log();
        }
        if (this->settings.autoplay_mode || !get_answer("Would you like to play again")) {
            break;
        }
    }
}

/// Initialize game board and players for a new game.
void Othello::init_game()
{
    if (games_played > 0) {
        board = Board(this->settings.board_size);
        player_black.reset();
        player_white.reset();
        rounds_played = 0;
    }

    if (this->settings.autoplay_mode) {
        // Computer plays both
        player_black.set_human(false);
        player_white.set_human(false);
    } else if (this->settings.use_defaults) {
        // Default: play as black against white computer player
        player_white.set_human(false);
    } else if (get_answer("Would you like to play against the computer")) {
        if (get_answer("Would you like to play as black or white", "b", "w")) {
            player_white.set_human(false);
        } else {
            player_black.set_human(false);
        }
    }
    print_bold("\nPlayers:\n");
    print_status();
}

/// Keep making moves until both players can't make a move any more.
void Othello::game_loop()
{
    while (board.can_play() && (player_black.can_play || player_white.can_play)) {
        ++rounds_played;
        fmt::print(fmt::emphasis::bold, "\n=========== ROUND: {} ===========\n", rounds_played);

        for (Player* player : {&player_black, &player_white}) {
            if (auto result = player->play_one_move(board); result.has_value()) {
                game_log.push_back(fmt::format("{};{}", result.value(), board.to_log_entry()));
            }
            fmt::print("--------------------------------\n");
        }
    }
    ++games_played;
}

void Othello::print_log() const
{
    std::string formatted_log;
    size_t index = 1;
    for (const auto& line : game_log) {
        formatted_log += fmt::format("{:02}: {}", index++, line);
        if (index <= game_log.size()) {
            formatted_log += "\n";
        }
    }

    const auto hex_hash = calculate_sha256(formatted_log);

    print_bold("Game log:\n", fmt::terminal_color::yellow);
    print(formatted_log);
    print(hex_hash);
}

/// Print ending status and winner info.
void Othello::print_result() const
{
    print_bold("\n================================\n");
    print_color("The game is finished!\n", fmt::terminal_color::green);
    print("Result:");
    print_status();
    print("");

    if (const Disk winner = board.result(); winner == Disk::empty) {
        print("The game ended in a tie...\n");
    } else {
        print(fmt::format("The winner is {}!\n", disk_string(winner)));
    }
}

/// Print current board and player info.
void Othello::print_status() const
{
    print(player_black);
    print(player_white);
    print("");
    print(board);
}

/// Read user input for yes/no question and return bool.
bool Othello::get_answer(const std::string& question, const std::string& yes, const std::string& no)
{
    // fmt library enables nice, modern string formatting,
    // instead of having to use the horrible stringstream system:
    // `std::cout << question << " (" << yes << "/" << no << ")? ";`
    fmt::print("{} ({}/{})? ", question, yes, no);
    std::string input;
    std::cin >> input;
    std::ranges::transform(
        input, input.begin(), [](const unsigned char c) { return std::tolower(c); });
    return input == yes;
}

/// Ask and return the desired board size.
size_t Othello::get_board_size()
{
    fmt::print("Choose board size (default is {}): ", DEFAULT_BOARD_SIZE);
    std::string input;
    std::cin >> input;
    try {
        const size_t size = std::stoi(input);
        if (size < MIN_BOARD_SIZE || size > MAX_BOARD_SIZE) {
            print_warn(fmt::format(
                "Limiting board size to valid range {}...{}\n", MIN_BOARD_SIZE, MAX_BOARD_SIZE));
        }
        return std::clamp(size, MIN_BOARD_SIZE, MAX_BOARD_SIZE);
    } catch (const std::exception&) {
        print_warn(fmt::format("Invalid size, defaulting to {}...\n", DEFAULT_BOARD_SIZE));
    }
    return DEFAULT_BOARD_SIZE;
}
}  // namespace othello
