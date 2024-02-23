//==========================================================
// Utils header
// Helper utilities for Othello game
// Akseli Lukkarila
// 2019-2024
//==========================================================

#pragma once
#include "colorprint.hpp"

#include <algorithm>  // transform
#include <iostream>   // cout, cin
#include <sstream>
#include <string>   // string
#include <utility>  // move, pair
#include <vector>

namespace othello
{
static constexpr size_t MIN_BOARD_SIZE = 4;
static constexpr size_t MAX_BOARD_SIZE = 10;
static constexpr size_t DEFAULT_BOARD_SIZE = 8;

/// Represents one game piece or lack of one.
enum class Disk { black = -1, empty = 0, white = 1 };

/// Represents one step direction on the board.
struct Step {
    Step(const int x, const int y) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& out, const Step& step)
    {
        return out << fmt::format("({},{})", step.x, step.y);
    }

    bool operator<(const Step& other) const { return x < other.x || (x <= other.x && y < other.y); }
    bool operator==(const Step& other) const { return x == other.x && y == other.y; }
    Step operator+(const Step& other) const { return {x + other.x, y + other.y}; }
    Step& operator+=(const Step& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    int x;
    int y;
};

/// Represents one square location on the board.
struct Square {
    Square() : x(0), y(0) {}
    Square(const int x, const int y) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& out, const Square& square)
    {
        return out << fmt::format("({},{})", square.x, square.y);
    }

    bool operator<(const Square& other) const
    {
        return x < other.x || (x <= other.x && y < other.y);
    }
    bool operator==(const Square& other) const { return x == other.x && y == other.y; }
    Square operator+(const Square& other) const { return {x + other.x, y + other.y}; }
    Square& operator+=(const Square& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    Square operator+(const Step& other) const { return {x + other.x, y + other.y}; }
    Square& operator+=(const Step& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    int x;
    int y;
};

/// Player settings.
struct PlayerSettings {
    explicit PlayerSettings(const bool show_helpers, const bool test_mode)
        : show_helpers(show_helpers)
        , test_mode(test_mode)
    {}

    PlayerSettings() : show_helpers(true), test_mode(false) {}

    bool show_helpers;
    bool test_mode;
};

/// Game settings.
struct Settings {
    Settings(
        const size_t board_size,
        const bool autoplay_mode,
        const bool use_defaults,
        const bool show_helpers,
        const bool show_log,
        const bool test_mode)
        : board_size(board_size)
        , autoplay_mode(autoplay_mode)
        , use_defaults(use_defaults)
        , show_helpers(show_helpers)
        , show_log(show_log)
        , test_mode(test_mode)
    {}

    Settings()
        : board_size(8)

        , autoplay_mode(false)
        , use_defaults(false)
        , show_helpers(true)
        , show_log(false)
        , test_mode(false)
    {}

    /// Get player setting values from overall game settings.
    [[nodiscard]] PlayerSettings to_player_settings() const
    {
        PlayerSettings player_settings;
        player_settings.show_helpers = show_helpers;
        player_settings.test_mode = test_mode;
        return player_settings;
    }

    size_t board_size;
    bool autoplay_mode;
    bool use_defaults;
    bool show_helpers;
    bool show_log;
    bool test_mode;
};
}  // namespace othello

// Formatter specialization:
// For some reason MSVC needs to have this here before fmt::format is called below in Move.
// Having it at the end of the file works fine with Clang like it should.
// Bug in fmt lib with C++20 maybe? ¯\_(ツ)_/¯
template<> struct fmt::formatter<othello::Square> : ostream_formatter {};

namespace othello
{
/// Represents one possible disk placement for the given disk color.
struct Move {
    Move() : square(0, 0), disk(Disk::empty), value(0) {}
    Move(
        const Square square,
        const Disk disk,
        const size_t value,
        std::vector<std::pair<Step, size_t>> directions)
        : square(square)
        , disk(disk)
        , value(value)
        , directions(std::move(directions))
    {}

    [[nodiscard]] std::string log_entry() const;
    [[nodiscard]] std::vector<Square> affected_squares() const;

    friend std::ostream& operator<<(std::ostream& out, const Move& move)
    {
        return out << fmt::format("Square: {} -> value: {}", move.square, move.value);
    }

    bool operator<(const Move& other) const
    {
        // Largest value with the smallest coordinates first
        return value > other.value || (value == other.value && square < other.square);
    }

    Square square;
    Disk disk;
    size_t value;
    std::vector<std::pair<Step, size_t>> directions;
};

/// Returns the print colour for the given Disk.
fmt::terminal_color disk_color(const Disk& disk);

/// Returns string character representing board status (black, white, empty).
std::string board_char(const Disk& disk, bool color = true);

/// Returns the disk formatted as a coloured string.
std::string disk_string(const Disk& disk);

/// Returns the opposing disk colour.
inline Disk opponent(const Disk& disk)
{
    switch (disk) {
        case Disk::white:
            return Disk::black;
        case Disk::black:
            return Disk::white;
        case Disk::empty:
            [[fallthrough]];
        default:
            return Disk::empty;
    }
}

inline std::ostream& operator<<(std::ostream& out, const Disk& disk)
{
    return out << disk_string(disk);
}

/// Print an object to stream (default is std::cout).
template<typename T> void print(T object, const bool newline = true, std::ostream& out = std::cout)
{
    out << object;
    if (newline) {
        out << "\n";
    }
}

/// Convert object to string using a stringstream.
/// Requires that the stream insertion operator `<<` has been implemented for the given object.
/// Workaround for custom types without formatter specialization for fmt.
template<typename T> std::string to_string(const T& object)
{
    std::ostringstream stream;
    stream << object;
    return stream.str();
}

/// Calculate SHA256 hash for the given string.
std::string calculate_sha256(const std::string& text);

}  // namespace othello

// Formatter specialization: simply use overridden ostream operator
template<> struct fmt::formatter<othello::Move> : ostream_formatter {};

// Other option would be to customize based on std::string for example:
// template<> struct fmt::formatter<othello::Square> : fmt::formatter<std::string> {
//    auto format(othello::Square s, format_context& ctx)
//    {
//        return formatter<std::string>::format(fmt::format("({}, {})", s.x, s.y), ctx);
//    }
//};
//
// template<> struct fmt::formatter<othello::Move> : fmt::formatter<std::string> {
//    auto format(othello::Move m, format_context& ctx)
//    {
//        return formatter<std::string>::format(
//            fmt::format("Square: {} -> value: {}", m.square, m.value), ctx);
//    }
//};
