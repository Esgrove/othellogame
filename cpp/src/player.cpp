//==========================================================
// Class Player source
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2026
//==========================================================

#include "player.hpp"

#include "colorprint.hpp"

#include <chrono>
#include <optional>  // std::optional
#include <ranges>
#include <stdexcept>  // exceptions
#include <thread>     // sleep_for

namespace othello
{
/// Play one round as this player.
std::optional<std::string> Player::play_one_move(Board& board)
{
    if (!this->settings.check_mode) {
        print("Turn: " + disk_string(disk));
    }
    const auto moves = board.possible_moves(disk);
    if (moves.empty()) {
        can_play = false;
        if (!this->settings.check_mode) {
            print_yellow("  No moves available...\n");
        }
        return std::nullopt;
    }
    can_play = true;
    if (this->human() && this->settings.show_helpers && !this->settings.check_mode) {
        board.print_possible_moves(moves);
    }
    const auto chosen_move = human() ? get_human_move(moves) : get_computer_move(moves);
    board.place_disk(chosen_move);
    if (!this->settings.check_mode) {
        board.print_score();
    }
    ++rounds_played;
    if (!this->settings.test_mode) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return chosen_move.log_entry();
}

/// Reset player status for a new game.
void Player::reset()
{
    this->can_play = true;
    this->rounds_played = 0;
}

/// Returns true if player is controlled by a human player.
bool Player::human() const
{
    return othello::human(this->player_type);
}

/// Returns true if player is controlled by computer.
bool Player::computer() const
{
    return othello::computer(this->player_type);
}

/// Set the player as human or computer controlled.
void Player::set_player_type(const PlayerType type)
{
    this->player_type = type;
}

/// Set the player as human controlled.
void Player::set_human()
{
    set_player_type(PlayerType::Human);
}

/// Set the player as computer controlled.
void Player::set_computer()
{
    set_player_type(PlayerType::Computer);
}

/// Return move chosen by computer.
Move Player::get_computer_move(const std::vector<Move>& moves)
{
    if (!this->settings.check_mode) {
        print("  Computer plays...");
    }
    Move chosen_move;
    if (this->settings.test_mode) {
        chosen_move = moves[0];
    } else {
        // Wait a bit and pick a random move
        std::uniform_int_distribution rand_time(1000, 2000);
        const auto sleep_duration = std::chrono::milliseconds(rand_time(this->random));
        std::this_thread::sleep_for(sleep_duration);

        std::uniform_int_distribution<size_t> random_item(0, moves.size() - 1);
        // C++17 std::sample is even more convoluted here :(
        chosen_move = moves[random_item(this->random)];
    }
    if (!this->settings.check_mode) {
        fmt::print("  {} -> {}\n", chosen_move.square, chosen_move.value);
    }
    return chosen_move;
}

/// Return move chosen by a human player.
Move Player::get_human_move(const std::vector<Move>& moves) const
{
    while (true) {
        const auto square = get_square();
        // Check that the chosen square is actually one of the possible moves
        const auto valid_move
            = std::ranges::find_if(moves, [&square](const Move& m) { return m.square == square; });
        if (valid_move != moves.end()) {
            // Dereference iterator to get value
            return *valid_move;
        }
        print_error(
            fmt::format("  Can't place a {} disk in square {}!", disk_string(disk), square)
        );
    }
}

/// Ask human player for square coordinates.
Square Player::get_square()
{
    while (true) {
        print("  Give disk position (x,y): ", false);
        // Flush to ensure the message is displayed before reading input.
        std::cout << std::flush;

        std::string input;
        if (!std::getline(std::cin, input)) {
            print_error("  Input failed. Please try again.");
            std::cin.clear();
            continue;
        }

        // Read coordinates, supporting multi-digit values.
        // Parse failures and negative values are rejected, and the player is asked again.
        const auto trimmed = trim(input);
        if (const auto comma = trimmed.find(','); comma != std::string::npos) {
            const auto parse_coordinate = [](const std::string& text) -> int {
                try {
                    size_t pos = 0;
                    const int value = std::stoi(text, &pos);
                    return pos == text.size() ? value : -1;
                } catch (const std::exception&) {
                    return -1;
                }
            };
            const int x = parse_coordinate(trimmed.substr(0, comma));
            const int y = parse_coordinate(trimmed.substr(comma + 1));
            if (x >= 0 && y >= 0) {
                return {x, y};
            }
        }
        print_error("  Give coordinates in the form 'x,y'!");
    }
}

/// Return player type description string.
std::string Player::type_string() const
{
    return to_string(this->player_type);
}

std::ostream& operator<<(std::ostream& out, const Player& player)
{
    return out << fmt::format(
               "{} | {} | Moves: {}",
               disk_string(player.disk),
               player.type_string(),
               player.rounds_played
           );
}
}  // namespace othello
