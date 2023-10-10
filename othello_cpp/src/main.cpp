//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2023
//==========================================================

#include "colorprint.hpp"
#include "othello.hpp"
#include "version.hpp"

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
                version::TIMESTAMP);
            return 1;
        }
        if (arg == "--version" || arg == "-v") {
            fmt::print(
                "{} {} {} {} {}\n",
                version::APP_NAME,
                version::VERSION_NUMBER,
                version::TIMESTAMP,
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
            if (board_size < othello::MIN_BOARD_SIZE || board_size > othello::MAX_BOARD_SIZE) {
                print_error(fmt::format("Unsupported board size: {}", board_size));
                return 1;
            }
            fmt::print("Using board size: {}\n", board_size);
        } else {
            throw std::invalid_argument("Invalid board size");
        }
    } catch (const std::invalid_argument&) {
        // Otherwise ask user for board size
        board_size = othello::Othello::get_board_size();
    }

    othello::Othello game {board_size};
    game.play();
    return 0;
}
