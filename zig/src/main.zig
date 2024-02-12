const std = @import("std");
const ColorPrint = @import("colorprint.zig").ColorPrint;
const AnsiColor = @import("colorprint.zig").AnsiColor;

pub fn main() !void {
    // Print the game title in bold green
    ColorPrint.printBold("OTHELLO GAME - ZIG", AnsiColor.green);

    // Prints to stderr (it's a shortcut based on `std.io.getStdErr()`)
    std.debug.print("All your {s} are belong to us.\n", .{"codebase"});

    // stdout is for the actual output of your application, for example if you
    // are implementing gzip, then only the compressed bytes should be sent to
    // stdout, not any debugging messages.
    const stdout_file = std.io.getStdOut().writer();
    var bw = std.io.bufferedWriter(stdout_file);
    const stdout = bw.writer();

    try stdout.print("Run `zig build test` to run the tests.\n", .{});

    try bw.flush(); // don't forget to flush!

    ColorPrint.printColor("This is a standard message", AnsiColor.white);
    ColorPrint.printColor("Hello in Cyan", AnsiColor.cyan);
    ColorPrint.printBold("Hello in Bold Red", AnsiColor.red);
    ColorPrint.printError("This is an error message");
    ColorPrint.printWarn("This is a warning message");
}
