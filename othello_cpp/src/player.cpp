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

std::string othello::Player::disk_string_upper() const
{
    std::string disk = disk_string(color_);
    std::transform(disk.begin(), disk.end(), disk.begin(), ::toupper);
    return disk;
}

void othello::Player::play_one_move(Board& board) {
    std::cout << "Turn: ";
    print_color(this->disk_string_upper() + "\n", disk_color(this->color_));
    auto moves = board.possible_moves(this->color_);
    if (!moves.empty()) {
        this->can_play_ = true;
        if (this->human_) {
            board.print_possible_moves(moves);
            std::string input;
            while (true) {
                std::cout << "  Give disk position (x,y): ";
                std::cin >> input;
                if (input.size() == 3 && input[1] == ',') {
                    int x = input[0] - '0'; // ascii hack
                    int y = input[2] - '0'; // std::stoi(input.substr(2,1));
                    if (board.place_disc(x, y, this->color_)) {
                        break;
                    }
                    print_error("  Error: can't place a " + disk_string() + " disk in square (" + std::to_string(x) + "," + std::to_string(y) + ").\n");
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
        ++this->rounds_played_;
        print("");
        print(board);
        board.print_score();
    } else {
        print_color("  No moves available...", Color::YELLOW);
        this->can_play_ = false;
    }
}

std::ostream& othello::operator<<(std::ostream& out, Player& p)
{
    print_color(p.disk_string_upper(), disk_color(p.color_), out);
    out << " | " << p.type_string() << " | " << "moves: " << p.rounds_played_;
    return out;
}
