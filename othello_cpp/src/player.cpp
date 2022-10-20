//==========================================================
// Class Player source
// Defines one player for Othello
// Akseli Lukkarila
//==========================================================

#include "player.hpp"

#include "colorprint.hpp"

#include <algorithm>  // sort, transform
#include <chrono>
#include <stdexcept>  // exceptions
#include <thread>     // sleep_for

/// Play one round as this player.
void othello::Player::play_one_move(Board& board)
{
    print("Turn: " + disk_string(disk));
    auto moves = board.possible_moves(disk);
    if (!moves.empty()) {
        can_play_ = true;
        if (human && show_helpers) {
            board.print_moves(moves);
        }
        auto chosen_move = human ? get_human_move(moves) : get_computer_move(moves);
        board.place_disc(chosen_move);
        board.print_score();
        ++rounds_played;
    } else {
        can_play_ = false;
        print_color("  No moves available...\n", fmt::color::yellow);
    }
}

/// Return move chosen by computer.
othello::Move othello::Player::get_computer_move(const std::vector<Move>& moves)
{
    print("  Computer plays...");
    // wait a bit and pick a random move
    std::uniform_int_distribution<int> rand_time(1000, 2000);
    auto sleep_duration = std::chrono::milliseconds(rand_time(this->rand_gen));
    std::this_thread::sleep_for(sleep_duration);

    std::uniform_int_distribution<int> rand_item(0, static_cast<int>(moves.size() - 1));
    // C++17 std::sample is even more convoluted here :(
    auto move = moves[rand_item(this->rand_gen)];
    std::cout << "  -> " << move.square << "\n";
    return move;
}

/// Return move chosen by a human player.
othello::Move othello::Player::get_human_move(const std::vector<Move>& moves)
{
    while (true) {
        auto square = get_square();
        // check if given square is one of the possible moves
        auto move_iter
            = std::find_if(moves.begin(), moves.end(), [&square](const Move& move) { return move.square == square; });
        if (move_iter != moves.end()) {
            // dereference iterator to get value
            return *move_iter;
        }
        print_error(fmt::format("can't place a {} disk in square {}!\n", disk_string(disk), square), 2);
    }
}

/// Ask human player for square coordinates.
othello::Square othello::Player::get_square()
{
    std::string input;
    while (true) {
        try {
            std::cout << "  Give disk position (x,y): ";
            std::cin >> input;
            if (input.size() != 3 || input[1] != ',') {
                throw std::invalid_argument("");
            }
            int x = std::stoi(input.substr(0, 1));
            int y = std::stoi(input.substr(2, 1));
            return {x, y};
        } catch (const std::invalid_argument&) {
            print_error("give coordinates in the form 'x,y'!\n", 2);
        }
    }
}

std::ostream& othello::operator<<(std::ostream& out, othello::Player& p)
{
    return out << fmt::format("{} | {} | {}", disk_string(p.disk), p.type_string(), p.rounds_played);
}
