#include "player.hpp"

#include <gtest/gtest.h>

namespace othello
{

/// Test fixture that has access to the private members of `Player`.
class PlayerTest : public ::testing::Test
{
protected:
    static Disk disk(const Player& player)
    {
        return player.disk;
    }

    static int rounds_played(const Player& player)
    {
        return player.rounds_played;
    }

    static void set_rounds_played(Player& player, const int rounds)
    {
        player.rounds_played = rounds;
    }

    static PlayerSettings settings(const Player& player)
    {
        return player.settings;
    }
};

TEST_F(PlayerTest, new_player)
{
    const Player player = Player::black(PlayerSettings {});
    EXPECT_EQ(disk(player), Disk::black);
    EXPECT_TRUE(player.human());
    EXPECT_TRUE(player.can_play);
    EXPECT_EQ(rounds_played(player), 0);
    EXPECT_EQ(settings(player), PlayerSettings {});
}

TEST_F(PlayerTest, reset_player)
{
    Player player = Player::white(PlayerSettings {});
    player.can_play = false;
    set_rounds_played(player, 10);
    player.reset();
    EXPECT_TRUE(player.can_play);
    EXPECT_EQ(rounds_played(player), 0);
}

TEST_F(PlayerTest, set_human)
{
    Player player = Player::black(PlayerSettings {});
    EXPECT_TRUE(player.human());

    player.set_computer();
    EXPECT_TRUE(player.computer());
    EXPECT_FALSE(player.human());

    player.set_human();
    EXPECT_TRUE(player.human());
    EXPECT_FALSE(player.computer());
}

TEST_F(PlayerTest, player_type_string)
{
    Player player = Player::black(PlayerSettings {});
    EXPECT_EQ(player.type_string(), "Human   ");

    player.set_computer();
    EXPECT_EQ(player.type_string(), "Computer");
}

}  // namespace othello
