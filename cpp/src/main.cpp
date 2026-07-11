//==========================================================
// Main
// Play Othello (Reversi) on the command line
// https://en.wikipedia.org/wiki/Reversi
// Akseli Lukkarila
// 2019-2026
//==========================================================

#include "colorprint.hpp"
#include "cxxopts.hpp"
#include "othello.hpp"
#include "version.hpp"

#include <optional>

inline cxxopts::Options cli_arguments()
{
    cxxopts::Options options("othello_cpp", "A simple Othello CLI game implementation in C++");
    options.custom_help("[OPTIONS]");
    options.positional_help(
        fmt::format(
            "[SIZE]\n\nArguments:\n  [SIZE]            Optional board size ({}..{})",
            othello::MIN_BOARD_SIZE,
            othello::MAX_BOARD_SIZE
        )
    );

    options.add_options("Positional")(
        "size",
        fmt::format(
            "Optional board size ({}..{})", othello::MIN_BOARD_SIZE, othello::MAX_BOARD_SIZE
        ),
        cxxopts::value<size_t>()
    );

    // clang-format off
    options.add_options("Optional")
        ("a,autoplay", "Enable autoplay mode with computer control", cxxopts::value<bool>())
        ("c,check", "Autoplay and only print result", cxxopts::value<bool>())
        ("d,default", "Play with default settings", cxxopts::value<bool>())
        ("l,log", "Show game log at the end", cxxopts::value<bool>())
        ("n,no-helpers", "Hide disk placement hints", cxxopts::value<bool>())
        ("t,test", "Enable test mode with deterministic computer moves", cxxopts::value<bool>())
        ("v,version", "Print version and exit", cxxopts::value<bool>())
        ("h,help", "Print help and exit", cxxopts::value<bool>());
    // clang-format on
    options.parse_positional({"size"});

    return options;
}

/// Command line arguments
struct Args {
    std::optional<size_t> size;
    bool autoplay;
    bool check;
    bool use_defaults;
    bool log;
    bool no_helpers;
    bool test;
    bool version;
    bool help;
    std::string usage;

    /// Parse arguments directly with constructor
    Args(const int argc, const char* argv[])
    {
        auto options = cli_arguments();
        const auto parsed_args = options.parse(argc, argv);
        if (parsed_args.count("size") > 0) {
            size = parsed_args["size"].as<size_t>();
        }
        autoplay = parsed_args["autoplay"].as<bool>();
        check = parsed_args["check"].as<bool>();
        use_defaults = parsed_args["default"].as<bool>();
        log = parsed_args["log"].as<bool>();
        no_helpers = parsed_args["no-helpers"].as<bool>();
        test = parsed_args["test"].as<bool>();
        version = parsed_args["version"].as<bool>();
        help = parsed_args["help"].as<bool>();
        usage = options.help({"Optional"});
    }
};

size_t resolve_board_size(const Args& args)
{
    // Try to read board size from command line args
    if (args.size.has_value()) {
        const auto size = args.size.value();
        if (size < othello::MIN_BOARD_SIZE || size > othello::MAX_BOARD_SIZE) {
            throw std::invalid_argument(fmt::format("Unsupported board size: {}", size));
        }
        fmt::println("Using board size: {}", size);
        return size;
    }
    if (args.autoplay || args.use_defaults) {
        return othello::DEFAULT_BOARD_SIZE;
    }
    // Otherwise ask user for board size
    return othello::Othello::get_board_size();
}

int main(const int argc, const char* argv[])
{
    try {
        const Args args(argc, argv);

        if (args.version) {
            fmt::print("{}\n", version::version_info());
            return 0;
        }
        if (args.help) {
            fmt::print("{}", args.usage);
            return 0;
        }
        // `autoplay` conflicts with `default`
        if (args.autoplay && args.use_defaults) {
            print_error("the argument '-a/--autoplay' cannot be used with '-d/--default'");
            return 1;
        }

        print_green_bold("OTHELLO GAME - C++\n");

        const size_t board_size = resolve_board_size(args);

        const othello::Settings settings(
            board_size,
            args.autoplay || args.check,
            args.check,
            !args.no_helpers,
            args.log || args.check,
            args.test || args.check,
            args.use_defaults
        );

        othello::Othello(settings).play();
    } catch (const cxxopts::exceptions::exception& e) {
        print_error(e.what());
        return 1;
    } catch (const std::exception& e) {
        print_error(e.what());
        return 1;
    }

    return 0;
}
