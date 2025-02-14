//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2024
//==========================================================

#include "cxxopts.hpp"
#include "othello.hpp"
#include "utils.hpp"

#include <optional>

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

    // clang-format off
    options.add_options("Optional")
        ("a,autoplay", "Enable autoplay mode", cxxopts::value<bool>())
        ("c,check", "Only print hash to check the result", cxxopts::value<bool>())
        ("d,default", "Play with default settings", cxxopts::value<bool>())
        ("h,help", "Print help and exit", cxxopts::value<bool>())
        ("l,log", "Show log after a game", cxxopts::value<bool>())
        ("n,no-helpers", "Hide disk placement hints", cxxopts::value<bool>())
        ("t,test", "Enable test mode", cxxopts::value<bool>())
        ("v,version", "Print version and exit", cxxopts::value<bool>());
    // clang-format on
    options.parse_positional({"size"});

    return options;
}

/// CLI arguments
struct Args {
    std::optional<size_t> size;
    bool version;
    bool help;
    bool autoplay;
    bool check_mode;
    bool show_helpers;
    bool show_log;
    bool test_mode;
    bool use_defaults;
    std::string usage;

    /// Parse arguments directly with constructor
    Args(const int argc, const char* argv[])
    {
        auto options = cli_arguments();
        const auto parsed_args = options.parse(argc, argv);
        if (parsed_args.count("size") > 0) {
            size = parsed_args["size"].as<size_t>();
        }
        version = parsed_args["version"].as<bool>();
        help = parsed_args["help"].as<bool>();
        autoplay = parsed_args["autoplay"].as<bool>();
        check_mode = parsed_args["check"].as<bool>();
        show_helpers = !parsed_args["no-helpers"].as<bool>();
        show_log = parsed_args["log"].as<bool>();
        test_mode = parsed_args["test"].as<bool>();
        use_defaults = parsed_args["default"].as<bool>();
        usage = options.help({"Optional"});
    }
};

int main(const int argc, const char* argv[])
{
    try {
        const auto
            [size,
             version,
             help,
             autoplay,
             check_mode,
             show_helpers,
             show_log,
             test_mode,
             use_defaults,
             usage]
            = Args(argc, argv);

        if (version) {
            othello::print_version();
            return 0;
        }
        if (help) {
            fmt::print("{}", usage);
            return 1;
        }

        print_green("OTHELLO GAME - C++\n", true);

        size_t board_size;
        if (size.has_value()) {
            board_size = size.value();
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
