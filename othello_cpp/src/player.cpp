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

std::string othello::Player::color_string_upper() const
{
    std::string disk = get_disk_string(color);
    std::transform(disk.begin(), disk.end(), disk.begin(), ::toupper);
    return disk;
}

std::ostream& othello::operator<<(std::ostream& out, Player& p)
{
    out << p.color_string_upper() << " | " << p.type_string() << " | " << "moves: " << p.rounds << std::endl;
    return out;
}

void othello::Player::play_one_move(Board& game) {
    print("Turn: ", false);  // std::cout << "Turn: ";
    print_color(this->color_string_upper() + "\n", disk_color(this->color));
    auto moves = game.get_possible_moves(this->color);
    if (!moves.empty()) {
        this->can_play = true;
        if (this->human) {
            // sort moves in descending order according to value
            std::sort(moves.begin(), moves.end());
            print_moves(moves);
            std::string input;
            while (true) {
                std::cout << "  Give disk position (x,y): ";
                std::cin >> input;
                if (input.size() == 3 && input[1] == ',') {
                    int x = input[0] - '0'; // ascii hack
                    int y = input[2] - '0'; // std::stoi(input.substr(2,1));
                    if (game.place_disc(x, y, this->color)) {
                        break;
                    } else {
                        std::cout << "  Error: can't place a " << this->color_string() << " disk in square (" << x << "," << y << ").\n";
                    }
                } else {
                    print("  Error: give coordinates in the form \"x,y\".");
                }
            }
        } else {
            // computer plays: pick a random move
            print("  Computer plays...");
            std::uniform_int_distribution<int> rand_time(1000, 3000);
            auto sleep_duration = std::chrono::milliseconds(rand_time(this->rand_gen));
            std::this_thread::sleep_for(sleep_duration);

            std::uniform_int_distribution<int> rand_item(0, moves.size() - 1);
            auto pos = moves[rand_item(this->rand_gen)].square;  // C++17 std::sample is even more convoluted here :(
            std::cout << "  -> " << pos << "\n";
            game.place_disc(pos.x, pos.y, color);
        }
        ++this->rounds;
        print("\nResult:");
        std::cout << game;
    } else {
        print("  No moves available...");
        this->can_play = false;
    }
    print("--------------------------------");
}

void othello::Player::print_moves(const std::vector<Move>& moves) {
    print_color("  Possible plays (" + std::to_string(moves.size()) + "):\n", Color::YELLOW);
    for (const Move& move : moves) {
        std::cout << "  " << move << "\n";
    }
}

std::string othello::Player::type_string() const
{
    return this->human ? "Human   " : "Computer";
}
