//==========================================================
// Class Player source
// Defines one player for Othello
// Akseli Lukkarila
//==========================================================

#include <algorithm>  // sort, transform
#include <chrono>
#include <thread>     // sleep_for
#include <stdexcept>  // exceptions

#include "colorprint.hpp"
#include "player.hpp"

void othello::Player::play_one_move(Board& board) {
    std::cout << "Turn: ";
    print_color(disk_string_upper(disk) + "\n", disk_color(disk));
    auto moves = board.possible_moves(disk);
    if (!moves.empty()) {
        can_play_ = true;
        if(show_helpers && human) {
            board.print_possible_moves(moves);
        }
        auto move = human ? get_human_move(moves) : get_computer_move(moves);
        board.place_disc(move);
        ++rounds_played;
        board.print_score();
    } else {
        print_color("  No moves available...\n", fmt::color::yellow);
        can_play_ = false;
    }
}

/// Return move chosen by computer.
othello::Move othello::Player::get_computer_move(std::vector<Move> moves) {
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
othello::Move othello::Player::get_human_move(std::vector<Move> moves) {
    while (true) {
        Square square = get_square();
        auto move_iter = std::find_if(moves.begin(), moves.end(), [&square](const Move& move){ return move.square == square; });
        if (move_iter != moves.end()) {
            // dereference iterator to get value
            return *move_iter;
        } else {
            print_error(fmt::format("can't place a {} disk in square {}.\n", disk_string(disk), square));
        }
    }
}

/// Ask human player for square coordinates.
othello::Square othello::Player::get_square() {
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
            return Square(x, y);
        } catch (const std::invalid_argument& e) {
            print_error("give coordinates in the form 'x,y'!\n");
        }
    }
}

std::ostream& othello::operator<<(std::ostream& out, Player& p)
{
    out << fmt::format(fmt::fg(disk_color(p.disk)), disk_string_upper(p.disk));
    out << " | " << p.type_string() << " | " << "moves: " << p.rounds_played;
    return out;
}
