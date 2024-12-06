//==========================================================
// Class Player header
// Defines one player for Othello
// Akseli Lukkarila
// 2019-2024
//==========================================================

#pragma once
#include "board.hpp"

#include <random>

namespace othello
{
enum class PlayerType { Human, Computer };

/// Defines one player (human or computer).
class Player
{
public:
    explicit Player(const Disk disk, const PlayerSettings player_settings)
        : disk(disk)
        , settings(player_settings)
    {}

    explicit Player(
        const Disk disk,
        const PlayerSettings player_settings,
        const PlayerType player_type)
        : disk(disk)
        , settings(player_settings)
        , player_type(player_type)
    {}

    /// Shorthand to initialize a new player for black disks.
    static Player black(const PlayerSettings player_settings)
    {
        return Player(Disk::black, player_settings, PlayerType::Human);
    }

    /// Shorthand to initialize a new player for white disks.
    static Player white(const PlayerSettings player_settings)
    {
        return Player(Disk::white, player_settings, PlayerType::Computer);
    }

    [[nodiscard]] bool computer() const;
    [[nodiscard]] bool human() const;
    [[nodiscard]] std::optional<std::string> play_one_move(Board& board);
    [[nodiscard]] std::string type_string() const;

    void reset();
    void set_computer();
    void set_human();
    void set_player_type(PlayerType type);

    // String formatting
    friend std::ostream& operator<<(std::ostream& out, Player& player);

    bool can_play {true};

private:
    static Square get_square();

    [[nodiscard]] Move get_computer_move(const std::vector<Move>& moves);
    [[nodiscard]] Move get_human_move(const std::vector<Move>& moves) const;

    Disk disk;
    PlayerSettings settings;
    PlayerType player_type {PlayerType::Human};
    int rounds_played {0};

    std::mt19937 rand_gen {std::mt19937 {std::random_device {}()}};
};

}  // namespace othello

template<> struct fmt::formatter<othello::Player> : ostream_formatter {};
