//==========================================================
// Class Player source
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2025
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
    if (const auto moves = board.possible_moves(disk); !moves.empty()) {
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

    can_play = false;
    if (!this->settings.check_mode) {
        print_color("  No moves available...\n", fmt::terminal_color::yellow);
    }
    return std::nullopt;
}

/// Check if the player is human.
bool Player::human() const
{
    return this->player_type == PlayerType::Human;
}

/// Check if the player is a computer.
bool Player::computer() const
{
    return this->player_type == PlayerType::Computer;
}

/// Set player to be controlled by human or computer.
void Player::set_player_type(const PlayerType type)
{
    this->player_type = type;
}

/// Set the player to be controlled by a human.
void Player::set_human()
{
    set_player_type(PlayerType::Human);
}

/// Set the player to be controlled by a computer.
void Player::set_computer()
{
    set_player_type(PlayerType::Computer);
}

/// Reset player status for a new game.
void Player::reset()
{
    this->rounds_played = 0;
    this->can_play = true;
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
        const auto sleep_duration = std::chrono::milliseconds(rand_time(this->rand_gen));
        std::this_thread::sleep_for(sleep_duration);

        std::uniform_int_distribution<size_t> rand_item(0, moves.size() - 1);
        // C++17 std::sample is even more convoluted here :(
        chosen_move = moves[rand_item(this->rand_gen)];
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
        auto square = get_square();
        // Check if given square is one of the possible moves
        if (auto move
            = std::ranges::find_if(moves, [&square](const Move& m) { return m.square == square; });
            move != moves.end())
        {
            // Dereference iterator to get value
            return *move;
        }
        print_error(
            fmt::format("  Can't place a {} disk in square {}!\n", disk_string(disk), square)
        );
    }
}

/// Ask human player for square coordinates.
Square Player::get_square()
{
    std::string input;
    while (true) {
        try {
            print("  Give disk position (x,y): ", false);
            std::cin >> input;
            if (input.size() != 3 || input[1] != ',') {
                throw std::invalid_argument("Invalid coordinates");
            }
            int x = std::stoi(input.substr(0, 1));
            int y = std::stoi(input.substr(2, 1));
            return {x, y};
        } catch (const std::invalid_argument&) {
            print_error("  Give coordinates in the form 'x,y'\n");
        }
    }
}

std::string Player::type_string() const
{
    switch (this->player_type) {
        case PlayerType::Human:
            return "Human   ";
        case PlayerType::Computer:
            return "Computer";
        default:
            return "Unknown";
    }
}

std::ostream& operator<<(std::ostream& out, Player& player)
{
    return out << fmt::format(
               "{} | {} | Moves: {}",
               disk_string(player.disk),
               player.type_string(),
               player.rounds_played
           );
}
}  // namespace othello
