//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2024
//==========================================================

#include "colorprint.hpp"
#include "cxxopts.hpp"
#include "othello.hpp"
#include "utils.hpp"
#include "version.hpp"

int main(const int argc, const char* argv[])
{
    cxxopts::Options options("othello_cpp", "A simple Othello CLI game implementation.");
    options.custom_help("[OPTIONS]");
    options.positional_help(fmt::format(
        "[SIZE]\n\nArguments:\n  [SIZE]            Optional board size ({}..{})",
        othello::MIN_BOARD_SIZE,
        othello::MAX_BOARD_SIZE));

    options.add_options("Positional")(
        "size",
        fmt::format(
            "Optional board size ({}..{})", othello::MIN_BOARD_SIZE, othello::MAX_BOARD_SIZE),
        cxxopts::value<size_t>());

    options.add_options("Optional")("h,help", "Print help and exit", cxxopts::value<bool>())(
        "a,autoplay", "Enable autoplay mode", cxxopts::value<bool>())(
        "d,default", "Play with default settings", cxxopts::value<bool>())(
        "l,log", "Show log after a game", cxxopts::value<bool>())(
        "n,no-helpers", "Hide disk placement hints", cxxopts::value<bool>())(
        "t,test", "Enable test mode", cxxopts::value<bool>())(
        "v,version", "Print version and exit", cxxopts::value<bool>());

    options.parse_positional({"size"});

    try {
        const auto result = options.parse(argc, argv);

        print_bold("OTHELLO GAME - C++\n", fmt::terminal_color::green);

        if (result["version"].as<bool>()) {
            fmt::print(
                "{} {} {} {} {}\n",
                version::APP_NAME,
                version::VERSION_NUMBER,
                version::TIMESTAMP,
                version::BRANCH,
                version::COMMIT);
            return 0;
        }

        if (result["help"].as<bool>()) {
            std::cout << options.help({"Optional"}) << std::endl;
            return 1;
        }

        const bool autoplay = result["autoplay"].as<bool>();
        const bool use_defaults = result["default"].as<bool>();
        const bool show_helpers = !result["no-helpers"].as<bool>();
        const bool show_log = result["log"].as<bool>();
        const bool test_mode = result["test"].as<bool>();

        size_t board_size;
        if (result.count("size") > 0) {
            board_size = result["size"].as<size_t>();
            if (board_size < othello::MIN_BOARD_SIZE || board_size > othello::MAX_BOARD_SIZE) {
                print_error(fmt::format("Unsupported board size: {}", board_size));
                return 1;
            }
            fmt::println("Using board size: {}", board_size);
        } else if (autoplay || use_defaults) {
            board_size = othello::DEFAULT_BOARD_SIZE;
        } else {
            board_size = othello::Othello::get_board_size();
        }

        const othello::Settings settings(
            board_size, autoplay, use_defaults, show_helpers, show_log, test_mode);

        othello::Othello game {settings};
        game.play();
    } catch (const cxxopts::exceptions::exception& e) {
        print_error(e.what());
        return 1;
    }
    return 0;
}
