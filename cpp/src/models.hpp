//==========================================================
// Models header
// Basic types and methods.
// Akseli Lukkarila
// 2019-2026
//==========================================================

#pragma once

#include "colorprint.hpp"

#include <compare>  // three-way comparison
#include <string>   // string
#include <utility>  // move
#include <vector>

namespace othello
{
/// Represents one game piece or lack of one.
enum class Disk { black = -1, empty = 0, white = 1 };

/// Represents one step direction on the board.
struct Step {
    constexpr Step(const int x, const int y) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& out, const Step& step);

    // Ordered by x, then y. Also provides the implicitly defaulted `==`.
    auto operator<=>(const Step& other) const = default;

    Step operator+(const Step& other) const
    {
        return {x + other.x, y + other.y};
    }
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

    /// Get the index of this square on the board.
    [[nodiscard]] constexpr size_t board_index(const size_t board_size) const
    {
        return static_cast<size_t>(y) * board_size + static_cast<size_t>(x);
    }

    friend std::ostream& operator<<(std::ostream& out, const Square& square);

    // Ordered by x, then y. Also provides the implicitly defaulted `==`.
    auto operator<=>(const Square& other) const = default;

    Square operator+(const Square& other) const
    {
        return {x + other.x, y + other.y};
    }
    Square& operator+=(const Square& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    Square operator+(const Step& other) const
    {
        return {x + other.x, y + other.y};
    }
    Square& operator+=(const Step& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    int x;
    int y;
};

/// Represents a continuous line of squares in one direction.
///
/// The `step` field determines the direction on the board,
/// and `count` describes how many consecutive squares in that direction there are.
struct Direction {
    constexpr Direction(const Step step, const size_t count) : step(step), count(count) {}

    // Ordered by step, then count. Also provides the implicitly defaulted `==`.
    auto operator<=>(const Direction& other) const = default;

    /// Direction of travel on the board
    Step step;
    /// Number of consecutive same colour squares along this direction
    size_t count;
};

/// Represents one possible disk placement for the given disk colour.
struct Move {
    Move() : square(0, 0), disk(Disk::empty), value(0) {}
    Move(
        const Square square,
        const Disk disk,
        const size_t value,
        std::vector<Direction> directions
    ) :
        square(square),
        disk(disk),
        value(value),
        directions(std::move(directions))
    {}

    [[nodiscard]] std::string log_entry() const;
    [[nodiscard]] std::vector<Square> affected_squares() const;

    friend std::ostream& operator<<(std::ostream& out, const Move& move);

    // Largest value with the smallest coordinates first.
    // Weak ordering since moves with equal value and square may still differ.
    std::weak_ordering operator<=>(const Move& other) const
    {
        if (const auto cmp = other.value <=> value; cmp != 0) {
            return cmp;
        }
        return square <=> other.square;
    }
    // Equality compares square, value, and disk, excluding directions.
    bool operator==(const Move& other) const
    {
        return square == other.square && value == other.value && disk == other.disk;
    }

    Square square;
    Disk disk;
    size_t value;
    std::vector<Direction> directions;
};

/// Returns a single character identifier string for the given disk.
[[nodiscard]] std::string board_char(Disk disk);

/// Returns a coloured single character identifier string for the given disk.
[[nodiscard]] std::string board_char_with_color(Disk disk);

/// Return the associated colour for this disk.
[[nodiscard]] fmt::terminal_color disk_color(Disk disk);

/// Returns the disk formatted as a coloured string.
[[nodiscard]] std::string disk_string(Disk disk);

/// Return the opposing disk colour for this disk.
[[nodiscard]] constexpr Disk opponent(const Disk disk)
{
    switch (disk) {
        case Disk::black:
            return Disk::white;
        case Disk::empty:
            return Disk::empty;
        case Disk::white:
            return Disk::black;
        default:
            return Disk::empty;
    }
}

inline std::ostream& operator<<(std::ostream& out, const Disk disk)
{
    return out << disk_string(disk);
}

}  // namespace othello

// Formatter specialization: simply use overridden ostream operator
template<>
struct fmt::formatter<othello::Step> : ostream_formatter {};
template<>
struct fmt::formatter<othello::Square> : ostream_formatter {};
template<>
struct fmt::formatter<othello::Move> : ostream_formatter {};

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
