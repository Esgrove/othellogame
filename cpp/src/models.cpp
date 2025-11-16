//==========================================================
// Models source
// Basic types and methods.
// Akseli Lukkarila
// 2019-2025
//==========================================================

#include "models.hpp"

#include "utils.hpp"

#include <numeric>  // std::accumulate

namespace othello
{
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

std::string Move::log_entry() const
{
    return fmt::format("{}:{},{}", board_char(this->disk, false), this->square, this->value);
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
