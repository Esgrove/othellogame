const std = @import("std");

pub const MIN_BOARD_SIZE: usize = 4;
pub const MAX_BOARD_SIZE: usize = 10;
pub const DEFAULT_BOARD_SIZE: usize = 8;

pub const Disk = enum(isize) {
    Black = -1,
    Empty = 0,
    White = 1,
};

pub const Step = struct {
    x: isize,
    y: isize,
};

pub const Square = struct {
    x: isize,
    y: isize,
};

pub const PlayerSettings = struct {
    show_helpers: bool,
    test_mode: bool,
};

pub const Settings = struct {
    board_size: usize,
    check_mode: bool,
    test_mode: bool,
    autoplay_mode: bool,
    use_defaults: bool,
    show_helpers: bool,
    show_log: bool,

    pub fn toPlayerSettings(self: Settings) PlayerSettings {
        return PlayerSettings{
            .show_helpers = self.show_helpers,
            .test_mode = self.test_mode,
        };
    }

    pub fn toString(self: Settings) ![]u8 {
        var buf = std.ArrayList(u8).init(std.heap.page_allocator);
        defer buf.deinit();

        var writer = buf.writer();

        try writer.print("Settings:\n", .{});
        try writer.print("  board_size: {d},\n", .{self.board_size});
        try writer.print("  check_mode: {},\n", .{self.check_mode});
        try writer.print("  test_mode: {},\n", .{self.test_mode});
        try writer.print("  autoplay_mode: {},\n", .{self.autoplay_mode});
        try writer.print("  use_defaults: {},\n", .{self.use_defaults});
        try writer.print("  show_helpers: {},\n", .{self.show_helpers});
        try writer.print("  show_log: {}\n", .{self.show_log});
        try writer.print("\n", .{});

        return buf.toOwnedSlice();
    }
};
