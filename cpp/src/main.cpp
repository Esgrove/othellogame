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

inline cxxopts::Options cli_arguments()
{
    cxxopts::Options options("othello_cpp", "A simple Othello CLI game implementation in C++");
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

    options.add_options("Optional")("a,autoplay", "Enable autoplay mode", cxxopts::value<bool>())(
        "d,default", "Play with default settings", cxxopts::value<bool>())(
        "l,log", "Show log after a game", cxxopts::value<bool>())(
        "n,no-helpers", "Hide disk placement hints", cxxopts::value<bool>())(
        "t,test", "Enable test mode", cxxopts::value<bool>())(
        "c,check", "Only print hash to check the result", cxxopts::value<bool>())(
        "h,help", "Print help and exit", cxxopts::value<bool>())(
        "v,version", "Print version and exit", cxxopts::value<bool>());

    options.parse_positional({"size"});

    return options;
}

int main(const int argc, const char* argv[])
{
    try {
        auto args = cli_arguments();
        const auto parsed_args = args.parse(argc, argv);

        if (parsed_args["version"].as<bool>()) {
            fmt::print(
                "{} {} {} {} {}\n",
                version::APP_NAME,
                version::VERSION_NUMBER,
                version::TIMESTAMP,
                version::BRANCH,
                version::COMMIT);
            return 0;
        }

        if (parsed_args["help"].as<bool>()) {
            fmt::print("{}", args.help({"Optional"}));
            return 1;
        }

        const bool autoplay = parsed_args["autoplay"].as<bool>();
        const bool check_mode = parsed_args["check"].as<bool>();
        const bool show_helpers = !parsed_args["no-helpers"].as<bool>();
        const bool show_log = parsed_args["log"].as<bool>();
        const bool test_mode = parsed_args["test"].as<bool>();
        const bool use_defaults = parsed_args["default"].as<bool>();

        print_green("OTHELLO GAME - C++\n", true);

        size_t board_size;
        if (parsed_args.count("size") > 0) {
            board_size = parsed_args["size"].as<size_t>();
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
            board_size,
            autoplay || check_mode,
            check_mode,
            show_helpers,
            show_log,
            test_mode || check_mode,
            use_defaults);

        othello::Othello(settings).play();
    } catch (const cxxopts::exceptions::exception& e) {
        print_error(e.what());
        return 1;
    }

    return 0;
}
