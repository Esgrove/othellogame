//==========================================================
// Class Player source
// Defines one player for Othello
// Akseli Lukkarila
//==========================================================

#include <algorithm>  // sort, transform
#include <chrono>
#include <thread>     // sleep_for

#include "colorprint.hpp"
#include "player.hpp"

void othello::Player::play_one_move(Board& board) {
    std::cout << "Turn: ";
    print_color(disk_string_upper(color_) + "\n", disk_color(color_));
    auto moves = board.possible_moves(color_);
    if (!moves.empty()) {
        can_play_ = true;
        if (human_) {
            othello::Board::print_possible_moves(moves);
            std::string input;
            while (true) {
                std::cout << "  Give disk position (x,y): ";
                std::cin >> input;
                if (input.size() == 3 && input[1] == ',') {
                    int x = input[0] - '0'; // ascii hack
                    int y = input[2] - '0'; // std::stoi(input.substr(2,1));
                    if (board.place_disc(x, y, color_)) {
                        break;
                    }
                    //print_error(fmt::format("  Error: can't place a {} disk in square ({}, {}).\n", disk_string(color_), x, y));
                } else {
                    print_error("  Error: give coordinates in the form 'x,y'.");
                }
            }
        } else {
            // computer plays: pick a random move
            print("  Computer plays...");
            std::uniform_int_distribution<int> rand_time(1000, 2000);
            auto sleep_duration = std::chrono::milliseconds(rand_time(this->rand_gen_));
            std::this_thread::sleep_for(sleep_duration);

            std::uniform_int_distribution<int> rand_item(0, moves.size() - 1);
            // C++17 std::sample is even more convoluted here :(
            Square pos = moves[rand_item(this->rand_gen_)].square;
            std::cout << "  -> " << pos << "\n";
            board.place_disc(pos.x, pos.y, color_);
        }
        ++rounds_played_;
        print("");
        print(board);
        board.print_score();
    } else {
        print_color("  No moves available...", fmt::color::yellow);
        can_play_ = false;
    }
}

std::ostream& othello::operator<<(std::ostream& out, Player& p)
{
    out << fmt::format(fmt::fg(disk_color(p.color_)), disk_string_upper(p.color_));
    out << " | " << p.type_string() << " | " << "moves: " << p.rounds_played_;
    return out;
}
