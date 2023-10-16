#include "utils.hpp"

namespace othello
{
std::string Move::to_log_entry()
{
    return fmt::format("{}:{},{}", board_char(this->disk, false), this->square, this->value);
}

fmt::terminal_color disk_color(const Disk& disk)
{
    using enum fmt::terminal_color;
    if (disk == Disk::empty) {
        return white;
    }
    return disk == Disk::white ? cyan : magenta;
}

std::string board_char(const Disk& disk, bool color)
{
    if (disk == Disk::empty) {
        return "_";
    }
    std::string identifier = disk == Disk::white ? "W" : "B";
    return color ? get_color(identifier, disk_color(disk)) : identifier;
}

std::string disk_string(const Disk& disk)
{
    auto color = disk_color(disk);
    switch (disk) {
        case Disk::empty:
            return get_color("EMPTY", color);
        case Disk::black:
            return get_color("BLACK", color);
        case Disk::white:
            return get_color("WHITE", color);
        default:
            return "UNKNOWN";
    }
}

Disk opponent(const Disk& disk)
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

}  // namespace othello
