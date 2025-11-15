//==========================================================
// Models header
// Basic types and methods.
// Akseli Lukkarila
// 2019-2025
//==========================================================

#pragma once

#include "colorprint.hpp"

#include <string>   // string
#include <utility>  // move, pair
#include <vector>

namespace othello
{
/// Represents one game piece or lack of one.
enum class Disk { black = -1, empty = 0, white = 1 };

/// Represents one step direction on the board.
struct Step {
    constexpr Step(const int x, const int y) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& out, const Step& step);

    bool operator<(const Step& other) const
    {
        return x < other.x || (x <= other.x && y < other.y);
    }
    bool operator==(const Step& other) const
    {
        return x == other.x && y == other.y;
    }
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

/// Represents a continuous line of squares in one direction.
///
/// The step component determines the direction on the board,
/// and count describes how many consecutive squares in that direction there are.
struct Direction {
    constexpr Direction(const Step step, const size_t count) : step(step), count(count) {}

    bool operator<(const Direction& other) const
    {
        return step < other.step || (step == other.step && count < other.count);
    }
    bool operator==(const Direction& other) const
    {
        return step == other.step && count == other.count;
    }

    Step step;
    size_t count;
};

/// Represents one square location on the board.
struct Square {
    Square() : x(0), y(0) {}
    Square(const int x, const int y) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& out, const Square& square);

    bool operator<(const Square& other) const
    {
        return x < other.x || (x <= other.x && y < other.y);
    }
    bool operator==(const Square& other) const
    {
        return x == other.x && y == other.y;
    }
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

    bool operator<(const Move& other) const
    {
        // Largest value with the smallest coordinates first
        return value > other.value || (value == other.value && square < other.square);
    }

    Square square;
    Disk disk;
    size_t value;
    std::vector<Direction> directions;
};

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
