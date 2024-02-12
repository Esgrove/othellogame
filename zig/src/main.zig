const std = @import("std");
const ColorPrint = @import("colorprint.zig").ColorPrint;
const AnsiColor = @import("colorprint.zig").AnsiColor;

const clap = @import("clap");

const debug = std.debug;
const io = std.io;

const version = @import("version.zig").version;

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    // First we specify what parameters our program can take.
    // We can use `parseParamsComptime` to parse a string into an array of `Param(Help)`
    const params = comptime clap.parseParamsComptime(
        \\-h, --help             Print help and exit
        \\-v, --version          Print version and exit
        \\-n, --number <usize>   An option parameter, which takes a value.
        \\-s, --string <str>...  An option parameter which can be specified multiple times.
        \\
    );

    ColorPrint.printBold("OTHELLO GAME - ZIG", AnsiColor.green);

    // Initialize our diagnostics, which can be used for reporting useful errors.
    // This is optional. You can also pass `.{}` to `clap.parse` if you don't
    // care about the extra information `Diagnostics` provides.
    var diag = clap.Diagnostic{};
    var res = clap.parse(clap.Help, &params, clap.parsers.default, .{
        .diagnostic = &diag,
        .allocator = gpa.allocator(),
    }) catch |err| {
        // Report useful error and exit
        diag.report(io.getStdErr().writer(), err) catch {};
        return err;
    };
    defer res.deinit();

    if (res.args.help != 0) {
        debug.print("A simple Othello CLI game implementation.\n\n", .{});
        debug.print("Usage: othello_zig [OPTIONS] [SIZE]\n\n", .{});
        debug.print("Arguments:\n", .{});
        debug.print("[SIZE]  Optional board size\n\n", .{});
        debug.print("Options:\n", .{});
        return clap.help(std.io.getStdErr().writer(), clap.Help, &params, .{});
    }
    if (res.args.version != 0) {
        debug.print("Othello Zig {s}\n", .{version});
        std.os.exit(0);
    }
    if (res.args.number) |n|
        debug.print("--number = {}\n", .{n});
    for (res.args.string) |s|
        debug.print("--string = {s}\n", .{s});
    for (res.positionals) |pos|
        debug.print("{s}\n", .{pos});

    ColorPrint.printColor("This is a standard message", AnsiColor.white);
    ColorPrint.printColor("Hello in Cyan", AnsiColor.cyan);
    ColorPrint.printBold("Hello in Bold Red", AnsiColor.red);
    ColorPrint.printError("This is an error message");
    ColorPrint.printWarn("This is a warning message");
}
