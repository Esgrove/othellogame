const std = @import("std");

pub const AnsiColor = enum {
    cyan,
    green,
    magenta,
    red,
    white,
    yellow,
    bold,
    reset,

    pub fn code(self: AnsiColor) []const u8 {
        return switch (self) {
            .cyan => "\x1b[36m",
            .green => "\x1b[32m",
            .magenta => "\x1b[35m",
            .red => "\x1b[31m",
            .white => "\x1b[37m",
            .yellow => "\x1b[33m",
            .bold => "\x1b[1m",
            .reset => "\x1b[0m",
        };
    }
};

pub const ColorPrint = struct {
    const allocator = std.heap.page_allocator;

    pub fn getColor(text: []const u8, color: AnsiColor, bold: bool) ![]u8 {
        const resetCode = AnsiColor.reset.code();
        const coloredText = try std.fmt.allocPrint(ColorPrint.allocator, "{s}{s}{s}", .{ color.code(), text, resetCode });

        if (bold) {
            return std.fmt.allocPrint(ColorPrint.allocator, "{s}{s}{s}{s}", .{ color.code(), AnsiColor.bold.code(), coloredText, resetCode });
        }

        return coloredText;
    }

    pub fn printColor(text: []const u8, color: AnsiColor) void {
        const output = ColorPrint.getColor(text, color, false) catch return;
        std.io.getStdOut().writer().print("{s}\n", .{output}) catch {};
    }

    pub fn printBold(text: []const u8, color: AnsiColor) void {
        const output = ColorPrint.getColor(text, color, true) catch return;
        std.io.getStdOut().writer().print("{s}\n", .{output}) catch {};
    }

    pub fn printError(message: []const u8) void {
        const fullMessage = std.fmt.allocPrint(ColorPrint.allocator, "Error: {s}", .{message}) catch {
            std.io.getStdOut().writer().print("Error: Failed to allocate memory for error message\n", .{}) catch {};
            return;
        };
        const output = ColorPrint.getColor(fullMessage, AnsiColor.red, true) catch {
            std.io.getStdOut().writer().print("Error: Failed to apply color formatting\n", .{}) catch {};
            ColorPrint.allocator.free(fullMessage);
            return;
        };
        std.io.getStdOut().writer().print("{s}\n", .{output}) catch {};
        ColorPrint.allocator.free(fullMessage);
    }

    pub fn printWarn(message: []const u8) void {
        const fullMessage = std.fmt.allocPrint(ColorPrint.allocator, "Warning: {s}", .{message}) catch {
            std.io.getStdOut().writer().print("Warning: Failed to allocate memory for warning message\n", .{}) catch {};
            return;
        };
        const output = ColorPrint.getColor(fullMessage, AnsiColor.yellow, true) catch {
            std.io.getStdOut().writer().print("Warning: Failed to apply color formatting\n", .{}) catch {};
            ColorPrint.allocator.free(fullMessage);
            return;
        };
        std.io.getStdOut().writer().print("{s}\n", .{output}) catch {};
        ColorPrint.allocator.free(fullMessage);
    }
};
