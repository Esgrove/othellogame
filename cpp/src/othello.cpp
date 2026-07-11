//==========================================================
// Class Othello source
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2026
//==========================================================

#include "othello.hpp"

#include "colorprint.hpp"
#include "utils.hpp"

#include <algorithm>  // clamp, transform
#include <iostream>
#include <ranges>

namespace othello
{

/// Initialize Othello game.
Othello::Othello(const Settings settings) :
    board(Board(settings.board_size)),
    settings(settings),
    player_black(Player::black(settings.to_player_settings())),
    player_white(Player::white(settings.to_player_settings()))
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
    // Re-use existing objects instead of initializing new ones
    if (games_played > 0) {
        board = Board(this->settings.board_size);
        player_black.reset();
        player_white.reset();
        rounds_played = 0;
        game_log.clear();
    }
    if (this->settings.autoplay_mode) {
        // Computer plays both
        player_black.set_computer();
        player_white.set_computer();
    } else if (this->settings.use_defaults) {
        // Default: play as black against white computer player
        player_white.set_computer();
    } else if (get_answer("Would you like to play against the computer")) {
        if (get_answer("Would you like to play as black or white", "b", "w")) {
            player_white.set_computer();
        } else {
            player_black.set_computer();
        }
    }
    if (!this->settings.check_mode) {
        print_bold("\nPlayers:\n");
        print_status();
    }
}

/// Keep making moves until both players can't make a move any more.
void Othello::game_loop()
{
    while (board.can_play() && (player_black.can_play || player_white.can_play)) {
        ++rounds_played;
        print_round_header();
        for (Player* player : {&player_black, &player_white}) {
            if (auto result = player->play_one_move(board); result.has_value()) {
                game_log.push_back(fmt::format("{};{}", result.value(), board.log_entry()));
            }
            if (!this->settings.check_mode) {
                fmt::print("--------------------------------\n");
            }
        }
    }
    ++games_played;
    print_game_end_footer();
}

std::string Othello::format_game_log() const
{
    std::string formatted_log;
    size_t index = 1;
    for (const auto& line : game_log) {
        formatted_log += fmt::format("{:02}: {}", index++, line);
        if (index <= game_log.size()) {
            formatted_log += "\n";
        }
    }
    return formatted_log;
}

void Othello::print_round_header() const
{
    if (!this->settings.check_mode) {
        print_bold("\n=========== ROUND: {} ===========\n", rounds_played);
    }
}

void Othello::print_game_end_footer() const
{
    if (!this->settings.check_mode) {
        print_bold("\n================================\n");
        print_color_bold(fmt::terminal_color::green, "The game is finished!\n\n");
    }
}

/// Print game log which shows all moves made and the game board state after each move.
void Othello::print_log() const
{
    const auto formatted_log = format_game_log();
    if (!this->settings.check_mode) {
        print_yellow_bold("Game log:\n");
        print(formatted_log);
    }
    const auto hex_hash = calculate_sha256(formatted_log);
    print(hex_hash);
}

/// Print ending status and winner info.
void Othello::print_result() const
{
    print_bold("Result:\n");
    print_status();
    fmt::print("\n");

    if (const Disk winner = board.result(); winner == Disk::empty) {
        fmt::print("The game ended in a tie...\n\n");
    } else {
        fmt::print("The winner is {}!\n\n", disk_string(winner));
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

/// Ask a question with two options, and return bool from user answer.
bool Othello::get_answer(const std::string& question, const std::string& yes, const std::string& no)
{
    fmt::print("{} ({}/{})? ", question, yes, no);
    // Flush to ensure the question is displayed before reading input.
    std::cout << std::flush;
    std::string input;
    std::getline(std::cin, input);
    auto answer = trim(input);
    std::ranges::transform(answer, answer.begin(), [](const unsigned char c) {
        return std::tolower(c);
    });
    return answer == yes;
}

/// Ask and return the desired board size.
size_t Othello::get_board_size()
{
    fmt::print("Choose board size (default is {}): ", DEFAULT_BOARD_SIZE);
    // Flush to ensure the question is displayed before reading input.
    std::cout << std::flush;
    std::string input;
    std::getline(std::cin, input);
    try {
        const int size = std::stoi(trim(input));
        if (size < 0) {
            throw std::invalid_argument("Negative board size");
        }
        const auto board_size = static_cast<size_t>(size);
        if (board_size < MIN_BOARD_SIZE || board_size > MAX_BOARD_SIZE) {
            print_yellow(
                "Limiting board size to valid range {}..{}\n", MIN_BOARD_SIZE, MAX_BOARD_SIZE
            );
        }
        return std::clamp(board_size, MIN_BOARD_SIZE, MAX_BOARD_SIZE);
    } catch (const std::exception&) {
        print_warn(fmt::format("Invalid size, defaulting to {}...", DEFAULT_BOARD_SIZE));
    }
    return DEFAULT_BOARD_SIZE;
}
}  // namespace othello
