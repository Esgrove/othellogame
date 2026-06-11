//==========================================================
// Models source
// Basic types and methods.
// Akseli Lukkarila
// 2019-2026
//==========================================================

#include "models.hpp"

#include <algorithm>  // std::sort
#include <numeric>    // std::accumulate

namespace othello
{
/// Returns a single character identifier string for the given disk.
std::string board_char(const Disk& disk)
{
    switch (disk) {
        case Disk::black:
            return "B";
        case Disk::empty:
            return "_";
        case Disk::white:
            return "W";
        default:
            return "?";
    }
}

/// Returns a single character identifier string for the given disk.
std::string board_char_with_color(const Disk& disk)
{
    return get_color(board_char(disk), disk_color(disk));
}

/// Return the associated colour for this disk.
fmt::terminal_color disk_color(const Disk& disk)
{
    using enum fmt::terminal_color;
    switch (disk) {
        case Disk::black:
            return magenta;
        case Disk::empty:
            return white;
        case Disk::white:
            return cyan;
        default:
            return white;
    }
}

/// Returns the disk formatted as a coloured string.
std::string disk_string(const Disk& disk)
{
    const auto color = disk_color(disk);
    switch (disk) {
        case Disk::black:
            return get_color("BLACK", color);
        case Disk::empty:
            return get_color("EMPTY", color);
        case Disk::white:
            return get_color("WHITE", color);
        default:
            return "UNKNOWN";
    }
}

/// Format move for log entry
std::string Move::log_entry() const
{
    return fmt::format("{}:{},{}", board_char(this->disk), this->square, this->value);
}

/// Get all the squares playing this move will change.
std::vector<Square> Move::affected_squares() const
{
    // Calculate the required size for the vector.
    // Need to specify the initial size type explicitly to make MSVC happy.
    const size_t total_size = std::accumulate(
        this->directions.begin(),
        this->directions.end(),
        size_t {0},
        [](const size_t sum, const Direction& dir) { return sum + dir.count; }
    );

    std::vector<Square> paths(total_size);

    // Insert directly to container since it has been pre-allocated to the correct size.
    // This way avoids the overhead of dynamically resizing the container.
    auto paths_iterator = paths.begin();
    for (const auto& [step, count] : directions) {
        Square pos = square + step;
        for (auto i = 0u; i < count; ++i) {
            *paths_iterator++ = pos;
            pos += step;
        }
    }
    std::sort(paths.begin(), paths.end());
    return paths;
}

std::ostream& operator<<(std::ostream& out, const Step& step)
{
    return out << fmt::format("({},{})", step.x, step.y);
}

std::ostream& operator<<(std::ostream& out, const Square& square)
{
    return out << fmt::format("({},{})", square.x, square.y);
}

std::ostream& operator<<(std::ostream& out, const Move& move)
{
    return out << fmt::format("Square: {} -> value: {}", move.square, move.value);
}

}  // namespace othello
