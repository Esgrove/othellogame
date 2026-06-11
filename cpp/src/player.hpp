//==========================================================
// Class Player header
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2026
//==========================================================

#pragma once
#include "board.hpp"
#include "settings.hpp"
#include "utils.hpp"

#include <random>

namespace othello
{
/// Player can be controlled either by a human or computer.
enum class PlayerType { Human, Computer };

/// Check if the player is controlled by a human.
[[nodiscard]] constexpr bool human(const PlayerType player_type)
{
    return player_type == PlayerType::Human;
}

/// Check if the player is controlled by a computer.
[[nodiscard]] constexpr bool computer(const PlayerType player_type)
{
    return player_type == PlayerType::Computer;
}

inline std::ostream& operator<<(std::ostream& out, const PlayerType& player_type)
{
    return out << (human(player_type) ? "Human   " : "Computer");
}

/// Defines one player that can be either human or computer controlled.
class Player
{
public:
    /// Initialize new player for the given disk color.
    explicit Player(const Disk disk, const PlayerSettings settings) : disk(disk), settings(settings)
    {}

    /// Shorthand to initialize a new player for black disks.
    static Player black(const PlayerSettings settings)
    {
        return Player(Disk::black, settings);
    }

    /// Shorthand to initialize a new player for white disks.
    static Player white(const PlayerSettings settings)
    {
        return Player(Disk::white, settings);
    }

    [[nodiscard]] std::optional<std::string> play_one_move(Board& board);
    void reset();
    [[nodiscard]] bool human() const;
    [[nodiscard]] bool computer() const;
    void set_player_type(PlayerType type);
    void set_human();
    void set_computer();
    [[nodiscard]] std::string type_string() const;

    // String formatting
    friend std::ostream& operator<<(std::ostream& out, const Player& player);

    bool can_play {true};

private:
    [[nodiscard]] Move get_computer_move(const std::vector<Move>& moves);
    [[nodiscard]] Move get_human_move(const std::vector<Move>& moves) const;
    static Square get_square();

    friend class PlayerTest;

    Disk disk;
    PlayerType player_type {PlayerType::Human};
    int rounds_played {0};
    PlayerSettings settings;

    std::mt19937 random {std::mt19937 {std::random_device {}()}};
};

}  // namespace othello

template<>
struct fmt::formatter<othello::Player> : ostream_formatter {};
