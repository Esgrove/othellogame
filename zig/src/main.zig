const std = @import("std");
const ColorPrint = @import("colorprint.zig").ColorPrint;
const AnsiColor = @import("colorprint.zig").AnsiColor;

const clap = @import("clap");

const debug = std.debug;
const io = std.io;

const version = @import("version.zig");
const utils = @import("utils.zig");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    // First we specify what parameters our program can take.
    // We can use `parseParamsComptime` to parse a string into an array of `Param(Help)`
    const params = comptime clap.parseParamsComptime(
        \\-a, --autoplay         Enable autoplay mode with both players controlled by computer
        \\-c, --check            Only print hash to check result (implies `autoplay` and `test`)
        \\-d, --default          Play with default settings
        \\-h, --help             Print help and exit
        \\-l, --log              Show game log at the end
        \\-n, --nohelp           Hide disk placement hints
        \\-t, --testmode         Enable test mode with deterministic computer move selection
        \\-v, --version          Print version and exit
        \\<usize>
        \\
    );

    ColorPrint.printBold("OTHELLO GAME - ZIG", AnsiColor.green);

    // Initialize our diagnostics, which can be used for reporting useful errors.
    // This is optional. You can also pass `.{}` to `clap.parse` if you don't
    // care about the extra information `Diagnostics` provides.
    var diag = clap.Diagnostic{};
    var cli = clap.parse(clap.Help, &params, clap.parsers.default, .{
        .diagnostic = &diag,
        .allocator = gpa.allocator(),
    }) catch |err| {
        // Report useful error and exit
        diag.report(io.getStdErr().writer(), err) catch {};
        return err;
    };
    defer cli.deinit();

    if (cli.args.help != 0) {
        debug.print("A simple Othello CLI game implementation.\n\n", .{});
        debug.print("Usage: othello_zig [OPTIONS] [SIZE]\n\n", .{});
        ColorPrint.printBold("Arguments:", AnsiColor.white);
        debug.print("    [SIZE]  Optional board size\n\n", .{});
        ColorPrint.printBold("Options:", AnsiColor.white);
        return clap.help(std.io.getStdErr().writer(), clap.Help, &params, .{});
    }
    if (cli.args.version != 0) {
        debug.print("Othello Zig {s} {s} {s} {s}\n", .{
            version.VERSION,
            version.BUILD_TIME,
            version.GIT_HASH,
            version.GIT_BRANCH,
        });
        std.os.exit(0);
    }

    var board_size: usize = utils.DEFAULT_BOARD_SIZE;
    if (cli.positionals.len > 0) {
        board_size = cli.positionals[0];
        std.debug.print("Using board size: {d}\n", .{board_size});
    }

    var settings = utils.Settings{
        .board_size = board_size,
        .check_mode = cli.args.check != 0,
        .test_mode = cli.args.testmode != 0 or cli.args.check != 0,
        .autoplay_mode = cli.args.autoplay != 0 or cli.args.check != 0,
        .use_defaults = cli.args.default != 0,
        .show_helpers = cli.args.nohelp == 0,
        .show_log = cli.args.log != 0,
    };

    const settingsStr = try settings.toString();
    std.debug.print("{s}\n", .{settingsStr});

    // ColorPrint.printColor("This is a standard message", AnsiColor.white);
    // ColorPrint.printColor("Hello in Cyan", AnsiColor.cyan);
    // ColorPrint.printBold("Hello in Bold Red", AnsiColor.red);
    // ColorPrint.printError("This is an error message");
    // ColorPrint.printWarn("This is a warning message");
}
